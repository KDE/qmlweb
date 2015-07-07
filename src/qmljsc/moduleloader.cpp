/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Anton Kreuzkamp <kde-development@akreuzkamp.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Own
#include "moduleloader.h"
#include "ir/module.h"
#include "ir/ir.h"
#include "compiler.h"
#include "error.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QThread>
#include <QtCore/QThreadPool>

// Qt private
#include <private/qqmljsengine_p.h>
#include <private/qqmljslexer_p.h>
#include <private/qqmljsparser_p.h>

using namespace QmlJSc;
using namespace QQmlJS;

QHash<IR::ImportDescription, IR::Module*> ModuleLoader::s_loadedModules;

IR::Module *ModuleLoader::loadModule(IR::ImportDescription import)
{
    Q_ASSERT(import.kind == IR::ImportDescription::Kind_ModuleImport);
    if (IR::Module *module = s_loadedModules.value(import)) {
        return module;
    }

    IR::Module *module = new IR::Module(import, compiler);
    s_loadedModules.insert(import, module);
    ModuleLoader *loader = new ModuleLoader(module); // will delete itself when done

    QThreadPool::globalInstance()->start(loader);

    return module;
}


ModuleLoader::ModuleLoader(IR::Module *module)
    : QRunnable()
    , Visitor()
    , m_module(module)
{
}

ModuleLoader::~ModuleLoader()
{
}

void ModuleLoader::run()
{
    try {
        doLoad();
    } catch (const Error &e) {
        qWarning() << e.what();
        m_module->m_status = IR::Module::ErrorState;
        m_module->m_waitCondition.wakeAll();
    }
}

void ModuleLoader::doLoad()
{
    // === Load Module File ===
    QFile moduleFile;
    m_moduleFileName = QStringLiteral("%1.%2.%3.js").arg(m_module->m_import.name)
                                                                .arg(m_module->m_import.versionMajor)
                                                                .arg(m_module->m_import.versionMinor);

    // For now we only support local files.
    const QStringList &includePaths = compiler->includePaths();
    foreach (QString includePath, includePaths) {
        QDir includeDir(includePath);
        if (includeDir.exists(m_moduleFileName)) {
            moduleFile.setFileName(includeDir.absoluteFilePath(m_moduleFileName));
            break;
        }
    }

    if (!moduleFile.exists()) {
        throw Error(Error::ModuleImportError, QStringLiteral("Could not find file %1 in path.").arg(m_moduleFileName));
    }

    // Read file
    moduleFile.open(QFile::ReadOnly);
    QTextStream modueFileStream(&moduleFile);
    QString moduleSource = modueFileStream.readAll();

    // === Parse file ===

    // parsing happens in three steps: Calling the QQmlJS-parser to parse the
    // file and return an AST, then using the visit functions of this class to
    // collect the data we need and and third calling finalizeParse() to
    // evaluate the parse data and transform it to actual type information.

    QQmlJS::Engine* engine = new QQmlJS::Engine();
    QQmlJS::Lexer* lexer = new QQmlJS::Lexer(engine);

    lexer->setCode(moduleSource, 1, true);

    QQmlJS::Parser* parser = new QQmlJS::Parser(engine);


    bool successfullyParsed = parser->parseProgram();
    if (!successfullyParsed) {
        Error *err = new Error(Error::ParseError, parser->errorMessage());
        err->setColumn(parser->errorColumnNumber());
        err->setLine(parser->errorLineNumber());
        throw Error(Error::ModuleImportError,
                    QStringLiteral("Error while processing module %1 %2.%3")
                        .arg(m_module->m_import.name)
                        .arg(m_module->m_import.versionMajor)
                        .arg(m_module->m_import.versionMinor),
                    err);
    }

    AST::Program *ast = AST::cast<AST::Program *>(parser->rootNode());

    ast->accept(this);

    finalizeParse();

    m_module->m_status = IR::Module::Successful;
    m_module->m_waitCondition.wakeAll(); // Wake up threads, that wait to access this module.
}

bool ModuleLoader::visit(AST::FunctionExpression* func)
{
    m_currentFunctionStack << func;
    return true;
}
void ModuleLoader::endVisit(AST::FunctionExpression* func)
{
    m_currentFunctionStack.removeLast();
}

bool ModuleLoader::visit(AST::FunctionDeclaration* func)
{
    m_currentFunctionStack << func;
    return true;
}
void ModuleLoader::endVisit(AST::FunctionDeclaration* func)
{
    m_currentFunctionStack.removeLast();
}

bool ModuleLoader::visit(AST::BinaryExpression* expr)
{
    // --- Might be a property, a method, a signal, a class or uninteresting.

    if (expr->right->kind == AST::Node::Kind_FunctionExpression) {
        findMethodDefinition(expr);


        // Each function expression is a potential class definition. Give it
        // a name, so we may reference it later on.
        QStringRef name;
        if (expr->left->kind == AST::Node::Kind_FieldMemberExpression) {
            name = AST::cast<AST::FieldMemberExpression *>(expr->left)->name;
        } else if (expr->left->kind == AST::Node::Kind_FunctionExpression) {
            name = AST::cast<AST::IdentifierExpression *>(expr->left)->name;
        } else {
            return true;
        }

        AST::cast<AST::FunctionExpression *>(expr->right)->name = name;

    } else if (expr->right->kind == AST::Node::Kind_CallExpression) {
        findSignalDefinition(expr);
    } else if (expr->right->kind == AST::Node::Kind_NewMemberExpression) {
        findPropertyDefinition(expr);
    }

    return true;
}

bool ModuleLoader::visit(AST::VariableDeclaration *var)
{
    AST::FunctionExpression *func = AST::cast<AST::FunctionExpression *>(var->expression);
    if (!func) {
        return true;
    }
    // This is a potential class definition. Give the function a name, so we may
    // reference it later on.
    func->name = var->name;

    return true;
}

bool ModuleLoader::visit(AST::CallExpression *call)
{
    if (call->base->kind == AST::Node::Kind_FieldMemberExpression) {
        findModuleRegistration(call);
    } else if (call->base->kind == AST::Node::Kind_IdentifierExpression) {
        findInheritance(call);
    }

    return true;
}

void ModuleLoader::findModuleRegistration(AST::CallExpression *call)
{
    AST::FieldMemberExpression *base = AST::cast<AST::FieldMemberExpression *>(call->base);
    if (!base || base->name != QStringLiteral("registerModule")) {
        return;
    }

    AST::IdentifierExpression *probablyEngine = AST::cast<AST::IdentifierExpression *>(base->base);
    if (!probablyEngine || probablyEngine->name != QStringLiteral("__engine")) {
        return;
    }

    // Ok, apparently, it's the __engine.registerModule expression, we're looking for.
    // If now still some assumption fails, throw an error.

    if (!call->arguments) {
        Error error(Error::ModuleImportError, "Malformed registerModule call: No argument provided.");
        error.setFile(m_moduleFileName);
        error.setLine(call->lparenToken.startLine);
        error.setColumn(call->lparenToken.startColumn);
        throw error;
    }

    AST::ObjectLiteral *moduleInfoLiteral = AST::cast<AST::ObjectLiteral *>(call->arguments->expression);
    if (!moduleInfoLiteral) {
        Error error(Error::ModuleImportError, "Malformed registerModule call: Wrong argument type provided. Expected Object Literal.");
        error.setFile(m_moduleFileName);
        error.setLine(call->lparenToken.startLine);
        error.setColumn(call->lparenToken.startColumn);
        throw error;
    }

    AST::PropertyAssignmentList *assignment = moduleInfoLiteral->properties;

    while(assignment) {
        AST::PropertyNameAndValue *nameAndValue = AST::cast<AST::PropertyNameAndValue*>(assignment->assignment);

        if (!nameAndValue) {
            Error error(Error::ModuleImportError, "Malformed registerModule call: Invalid type specification.");
            error.setFile(m_moduleFileName);
            error.setLine(assignment->assignment->firstSourceLocation().startLine);
            error.setColumn(assignment->assignment->firstSourceLocation().startColumn);
            throw error;
        }

        AST::IdentifierExpression *functionId = AST::cast<AST::IdentifierExpression*>(nameAndValue->value);
        if (!functionId) {
            Error error(Error::ModuleImportError, "Malformed registerModule call: Can't recognize function identifier. Please use a simple identifier as value on type object.");
            error.setFile(m_moduleFileName);
            error.setLine(nameAndValue->value->firstSourceLocation().startLine);
            error.setColumn(nameAndValue->value->firstSourceLocation().startColumn);
            throw error;
        }

        m_typesToFunctionsMap.insert(nameAndValue->name->asString(), functionId->name);
        assignment = assignment->next;
    }
}

void ModuleLoader::findInheritance(AST::CallExpression *call)
{
    AST::IdentifierExpression *maybeInheritance = AST::cast<AST::IdentifierExpression *>(call->base);
    if (!maybeInheritance || maybeInheritance->name != QStringLiteral("QW_INHERIT")) {
        return;
    }

    // Apparently this is a QW_INHERIT call, we're looking for.
    // If now still some assumption fails, throw an error.

    if (!call->arguments || !call->arguments->next) {
        Error error(Error::ModuleImportError, "Malformed QW_INHERIT call: One or no argument provided. Expected two.");
        error.setFile(m_moduleFileName);
        error.setLine(call->lparenToken.startLine);
        error.setColumn(call->lparenToken.startColumn);
        throw error;
    }

    AST::IdentifierExpression *constructor = AST::cast<AST::IdentifierExpression *>(call->arguments->expression);
    AST::IdentifierExpression *baseClass = AST::cast<AST::IdentifierExpression *>(call->arguments->next->expression);
    if (!constructor || !baseClass) {
        Error error(Error::ModuleImportError, "Malformed QW_INHERIT call: Wrong argument types provided. Expected two identifier expressions.");
        error.setFile(m_moduleFileName);
        error.setLine(call->lparenToken.startLine);
        error.setColumn(call->lparenToken.startColumn);
        throw error;
    }

    m_inheritancies.insert(constructor->name, baseClass->name);
}

void ModuleLoader::findPropertyDefinition(AST::BinaryExpression *expr)
{
    if (expr->op != 3) { // 3 stands for "="
        return;
    }

    AST::FieldMemberExpression *lValue = AST::cast<AST::FieldMemberExpression *>(expr->left);
    AST::NewMemberExpression *rValue = AST::cast<AST::NewMemberExpression *>(expr->right);
    if (!rValue || !lValue) {
        return;
    }

    AST::IdentifierExpression *constructor = AST::cast<AST::IdentifierExpression *>(rValue->base);
    AST::ThisExpression *maybeThis = AST::cast<AST::ThisExpression *>(lValue->base);
    if (!constructor || constructor->name != QStringLiteral("QWProperty") || !maybeThis) {
        return;
    }

    // Ok, this is a property definition
    IR::Class *c = getPreliminaryClass(m_currentFunctionStack.constLast()->name);
    IR::Property *property = c->addProperty(lValue->name.toString());
}

void ModuleLoader::findMethodDefinition(AST::BinaryExpression *expr)
{
    if (expr->op != 3) { // 3 stands for "="
        return;
    }

    AST::FieldMemberExpression *lValue = AST::cast<AST::FieldMemberExpression *>(expr->left);
    AST::FunctionExpression *rValue = AST::cast<AST::FunctionExpression *>(expr->right);
    IR::Class *c;

    if (lValue->base->kind == AST::Node::Kind_ThisExpression) {
        c = getPreliminaryClass(m_currentFunctionStack.constLast()->name);
    } else {
        AST::FieldMemberExpression *first = AST::cast<AST::FieldMemberExpression *>(lValue->base);
        if (!first || first->name != QStringLiteral("prototype")) {
            return;
        }

        AST::IdentifierExpression *constructor = AST::cast<AST::IdentifierExpression *>(first->base);
        if (!first) {
            return;
        }

        c = getPreliminaryClass(constructor->name);
    }

    // Treat as a method definiton of a class. We can't ever be sure, but let's
    // try and discard later, if it turns out not to be one.
    IR::Method *method = c->addMethod(lValue->name.toString());
}

void ModuleLoader::findSignalDefinition(AST::BinaryExpression *expr)
{
    if (expr->op != 3) { // 3 stands for "="
        return;
    }

    AST::FieldMemberExpression *lValue = AST::cast<AST::FieldMemberExpression *>(expr->left);
    AST::CallExpression *rValue = AST::cast<AST::CallExpression *>(expr->right);
    if (!rValue || !lValue) {
        return;
    }

    AST::IdentifierExpression *maybeSignal = AST::cast<AST::IdentifierExpression *>(rValue->base);
    AST::ThisExpression *maybeThis = AST::cast<AST::ThisExpression *>(lValue->base);
    if (!maybeSignal || maybeSignal->name != QStringLiteral("QWSignal") || !maybeThis) {
        return;
    }

    // Ok, this is a signal definition
    IR::Class *c = getPreliminaryClass(m_currentFunctionStack.constLast()->name);
    IR::Signal *signal = c->addSignal(lValue->name.toString());
}

IR::Class *ModuleLoader::getPreliminaryClass(const QStringRef &name)
{
    IR::Class *&c = m_preliminaryClasses[name];
    if (!c) {
        c = new IR::Class;
        c->setName(name.toString());
    }
    return c;
}

void ModuleLoader::finalizeParse()
{
    for (auto i = m_typesToFunctionsMap.constBegin(); i != m_typesToFunctionsMap.constEnd(); i++) {
        // the typesToFunctionsMap contains the most basic information, namely
        // which types exist. The key in this hash is the name of the type, the
        // value is the name of the function that defines it.
        IR::Class *c = getPreliminaryClass(i.value());
        m_module->m_types.insert(i.key(), c);
    }

    for (auto i = m_inheritancies.constBegin(); i != m_inheritancies.constEnd(); i++) {
        IR::Type *c = m_module->m_types[i.key().toString()];
        c->setSuper(m_module->m_types[i.value().toString()]);
    }
}
