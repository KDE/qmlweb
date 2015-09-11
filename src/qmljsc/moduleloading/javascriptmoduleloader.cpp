/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
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
#include "javascriptmoduleloader.h"
#include "moduleloading.h"
#include "ir/module.h"
#include "ir/typesystem.h"
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

#define assert(condition, token, errorMessage) \
if (!condition) { \
    assertFailed(token, errorMessage); \
}


using namespace QmlJSc;
using namespace QQmlJS;


static void assertFailed(const AST::SourceLocation &token, QString errorMessage)
{
    // Normally you wouldn't throw by reference but by value. We need to create
    // the error object on the stack and throw by pointer, because we'll need it
    // to be available on another thread.
    Error *error = new Error(Error::ModuleImportError, errorMessage);
    error->setLine(token.startLine);
    error->setColumn(token.startColumn);
    throw error;
}

RegisterModuleVisitor::RegisterModuleVisitor(IR::Module* module)
    : QQmlJS::AST::Visitor()
    , m_module(module)
{
}

bool RegisterModuleVisitor::visit(AST::CallExpression *call)
{
    if (call->base->kind != AST::Node::Kind_FieldMemberExpression) {
        return true;
    }

    AST::FieldMemberExpression *base = AST::cast<AST::FieldMemberExpression *>(call->base);
    if (!base || base->name != QStringLiteral("registerModule")) {
        return true;
    }

    AST::IdentifierExpression *probablyEngine = AST::cast<AST::IdentifierExpression *>(base->base);
    if (!probablyEngine || probablyEngine->name != QStringLiteral("__engine")) {
        return true;
    }

    // Ok, apparently, it's the __engine.registerModule expression, we're looking for.
    // If now still some assumption fails, throw an error.

    if (!call->arguments) {
        Error *error = new Error(Error::ModuleImportError, "Malformed registerModule call: No argument provided.");
        error->setLine(call->lparenToken.startLine);
        error->setColumn(call->lparenToken.startColumn);
        throw error;
    }

    AST::ObjectLiteral *moduleInfoLiteral = AST::cast<AST::ObjectLiteral *>(call->arguments->expression);
    assert(moduleInfoLiteral, call->lparenToken,
           "Malformed registerModule call: Wrong argument type provided. Expected Object Literal.");

    AST::PropertyAssignmentList *assignment = moduleInfoLiteral->properties;

    while(assignment) {
        AST::PropertyNameAndValue *nameAndValue = AST::cast<AST::PropertyNameAndValue*>(assignment->assignment);

        assert(nameAndValue, assignment->assignment->firstSourceLocation(),
            "Malformed registerModule call: Invalid type specification."
        );

        AST::IdentifierExpression *functionId = AST::cast<AST::IdentifierExpression*>(nameAndValue->value);
        assert(functionId, nameAndValue->value->firstSourceLocation(),
            "Malformed registerModule call: Can't recognize function identifier. "
            "Please use a simple identifier as value on type object."
        );


        IR::Type *c = new IR::Type;
        c->setName(nameAndValue->name->asString());
        c->setJavaScriptName(functionId->name.toString());
        m_module->addType(c);

        assignment = assignment->next;
    }

    return true;
}

TypeDefinitionVisitor::TypeDefinitionVisitor(IR::Module* module)
    : QQmlJS::AST::Visitor()
    , m_module(module)
{
}

bool TypeDefinitionVisitor::visit(AST::FunctionExpression* func)
{
    m_currentFunctionStack << func;
    return true;
}
void TypeDefinitionVisitor::endVisit(AST::FunctionExpression* func)
{
    m_currentFunctionStack.removeLast();
}

bool TypeDefinitionVisitor::visit(AST::FunctionDeclaration* func)
{
    m_currentFunctionStack << func;
    return true;
}
void TypeDefinitionVisitor::endVisit(AST::FunctionDeclaration* func)
{
    m_currentFunctionStack.removeLast();
}

bool TypeDefinitionVisitor::visit(AST::BinaryExpression* expr)
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

bool TypeDefinitionVisitor::visit(AST::VariableDeclaration *var)
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

bool TypeDefinitionVisitor::visit(AST::CallExpression *call)
{
    if (call->base->kind != AST::Node::Kind_IdentifierExpression) {
        return true;
    }

    AST::IdentifierExpression *maybeInheritance = AST::cast<AST::IdentifierExpression *>(call->base);
    if (!maybeInheritance || maybeInheritance->name != QStringLiteral("QW_INHERIT")) {
        return true;
    }

    // Apparently this is a QW_INHERIT call, we're looking for.
    // If now still some assumption fails, throw an error.

    assert(call->arguments && call->arguments->next, call->lparenToken,
        "Malformed QW_INHERIT call: One or no argument provided. Expected two."
    );

    AST::IdentifierExpression *constructor = AST::cast<AST::IdentifierExpression *>(call->arguments->expression);
    AST::IdentifierExpression *baseClass = AST::cast<AST::IdentifierExpression *>(call->arguments->next->expression);
    assert(constructor && baseClass, call->lparenToken,
        "Malformed QW_INHERIT call: Wrong argument types provided. Expected two identifier expressions."
    );

    IR::Type *t = m_module->typeFromJSName(constructor->name.toString());
    assert(t, constructor->firstSourceLocation(), "Using a type that won't get registered.");
    t->setSuper(getType(baseClass->name));

    return true;
}

void TypeDefinitionVisitor::findPropertyDefinition(AST::BinaryExpression *expr)
{
    if (expr->op != QSOperator::Assign) {
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
    IR::Type *t = m_module->typeFromJSName(m_currentFunctionStack.last()->name.toString());
    assert(t, lValue->firstSourceLocation(), "Registering properties to a type that won't get registered.");
    IR::Property *property = t->addProperty(lValue->name.toString());

    AST::ObjectLiteral *parameters = AST::cast<AST::ObjectLiteral *>(rValue->arguments->expression);
    if (!parameters) {
        return;
    }

    for (AST::PropertyAssignmentList *aList = parameters->properties; aList; aList = aList->next) {
        AST::PropertyNameAndValue *nameAndValue = AST::cast<AST::PropertyNameAndValue *>(aList->assignment);

        assert(nameAndValue, aList->assignment->firstSourceLocation(),
            "Malformed QWProperty call: Expected argument to be a name value pair."
        );

        if (nameAndValue->name->asString() == "type") {
            AST::IdentifierExpression *id = AST::cast<AST::IdentifierExpression *>(nameAndValue->value);
            assert(id, nameAndValue->value->firstSourceLocation(),
                "Malformed QWProperty call: Expected argument 'type' to have an identifier expression as value."
            );
            property->type = getType(id->name);
            continue;
        }
        if (nameAndValue->name->asString() == "typeArg") {
            AST::IdentifierExpression *id = AST::cast<AST::IdentifierExpression *>(nameAndValue->value);
            assert(id, nameAndValue->value->firstSourceLocation(),
                "Malformed QWProperty call: Expected argument 'typeArg' to have an identifier expression as value."
            );
             property->type = getType(id->name);
            continue;
        }
        if (nameAndValue->name->asString() == "readonly") {
            assert(nameAndValue->value->kind == AST::Node::Kind_TrueLiteral
                   || nameAndValue->value->kind == AST::Node::Kind_FalseLiteral,
                nameAndValue->colonToken,
                "Malformed QWProperty call: readonly may only have true or false as value."
            );
            property->readOnly = nameAndValue->value->kind == AST::Node::Kind_TrueLiteral;
            continue;
        }
        if (nameAndValue->name->asString() == "constant") {
            assert(nameAndValue->value->kind == AST::Node::Kind_TrueLiteral
                   || nameAndValue->value->kind == AST::Node::Kind_FalseLiteral,
                nameAndValue->colonToken,
                "Malformed QWProperty call: constant may only have true or false as value."
            );
            property->constant = nameAndValue->value->kind == AST::Node::Kind_TrueLiteral;
            continue;
        }
    }
}

void TypeDefinitionVisitor::findMethodDefinition(AST::BinaryExpression *expr)
{
    if (expr->op != QSOperator::Assign) {
        return;
    }

    AST::FieldMemberExpression *lValue = AST::cast<AST::FieldMemberExpression *>(expr->left);
    AST::FunctionExpression *func = AST::cast<AST::FunctionExpression *>(expr->right);
    IR::Type *t;

    if (lValue->base->kind == AST::Node::Kind_ThisExpression) {
        t = m_module->typeFromJSName(m_currentFunctionStack.last()->name.toString());
    } else {
        AST::FieldMemberExpression *first = AST::cast<AST::FieldMemberExpression *>(lValue->base);
        if (!first || first->name != QStringLiteral("prototype")) {
            return;
        }

        AST::IdentifierExpression *constructor = AST::cast<AST::IdentifierExpression *>(first->base);
        if (!first) {
            return;
        }

        t = m_module->typeFromJSName(constructor->name.toString());
    }

    if (!t) {
        return;
    }

    // Treat as a method definiton of a class. We can't ever be sure, but let's
    // try and discard later, if it turns out not to be one.
    IR::Method *method = t->addMethod(lValue->name.toString());

    AST::FormalParameterList *parameter = func->formals;
    while (parameter) {
        method->parameters << parameter->name.toString();
        parameter = parameter->next;
    }
}

void TypeDefinitionVisitor::findSignalDefinition(AST::BinaryExpression *expr)
{
    if (expr->op != QSOperator::Assign) {
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
    IR::Type *t = m_module->typeFromJSName(m_currentFunctionStack.last()->name.toString());
    assert(t, lValue->firstSourceLocation(),
           "Registering a signal to a type that won't get registered.");
    IR::Signal *signal = t->addSignal(lValue->name.toString());

    AST::ArgumentList *argumentList = rValue->arguments;
    if (!argumentList) {
        return; // Ok, no arguments, so we're done here.
    }

    AST::ArrayLiteral *arrayLit = AST::cast<AST::ArrayLiteral *>(argumentList->expression);
    assert(arrayLit, argumentList->expression->firstSourceLocation(), "Malformed Signal definition:"
           " First argument must be an array literal.");
    AST::ElementList *array = arrayLit->elements;

    // Go through all the elements in the list, each of which is an object
    // literal describing one parameter
    while (array) {
        AST::ObjectLiteral *parameterObject = AST::cast<AST::ObjectLiteral *>(array->expression);
        assert(parameterObject, array->expression->firstSourceLocation(),
               "Malformed Signal definition: Array elements must be object literals.");
        AST::PropertyAssignmentList *parameterData = parameterObject->properties;
        IR::Type *type = 0;
        QString name;

        // Go through all properties of the object literal. Basically we're
        // looking for "type" and "name"
        while (parameterData) {
            AST::PropertyNameAndValue *nameAndValue = AST::cast<AST::PropertyNameAndValue *>(parameterData->assignment);

            assert(nameAndValue, parameterData->assignment->firstSourceLocation(),
                   "Malformed Signal definition: The definition of a parameter "
                   "must only contain name and value.");

            if (nameAndValue->name->asString() == QStringLiteral("type")) {
                AST::IdentifierExpression *t = AST::cast<AST::IdentifierExpression *>(nameAndValue->value);
                assert(t, nameAndValue->value->firstSourceLocation(), "Malformed Signal definition:"
                       "The type definition of a parameter must be an identifier expression"
                       "that refers to a type.");
                type = getType(t->name);
            } else if (nameAndValue->name->asString() == QStringLiteral("name")) {
                AST::StringLiteral *n = AST::cast<AST::StringLiteral *>(nameAndValue->value);
                assert(n, nameAndValue->value->firstSourceLocation(), "Malformed Signal definition:"
                       "The name definition of a parameter must be a string literal.");
                name = n->value.toString();
            }

            parameterData = parameterData->next;
        }

        signal->parameters.append({type, name});

        array = array->next;
    }


}

IR::Type *TypeDefinitionVisitor::getType(const QStringRef& name)
{
    IR::Type *t = 0;
    t = m_module->typeFromJSName(name.toString());
    // TODO: Search different locations (Task T488).

    return t;
}




JavaScriptModuleLoader *JavaScriptModuleLoader::create(IR::Module *module)
{
    return new JavaScriptModuleLoader(module);
}

JavaScriptModuleLoader::JavaScriptModuleLoader(IR::Module *module)
    : AbstractModuleLoader(module)
{}

bool JavaScriptModuleLoader::canLoad()
{
    IR::Module *module = AbstractModuleLoader::module();

    QString moduleFileName = QStringLiteral("%1.%2.%3.js").arg(module->importDescription().name)
                                                          .arg(module->importDescription().versionMajor)
                                                          .arg(module->importDescription().versionMinor);

    // For now we only support local files.
    const QStringList &includePaths = compiler->includePaths();
    foreach (QString includePath, includePaths) {
        QDir includeDir(includePath);
        if (includeDir.exists(moduleFileName)) {
            m_moduleFile.setFileName(includeDir.absoluteFilePath(moduleFileName));
            break;
        }
    }

    return m_moduleFile.exists();
}

void JavaScriptModuleLoader::doLoad()
{
    IR::Module *module = AbstractModuleLoader::module();

    if (!m_moduleFile.exists()) { // We checked that already, so it should never happen.
        throw new Error(Error::ModuleImportError, QStringLiteral("Could not find file %1 in path.").arg(m_moduleFile.fileName()));
    }

    // Read file
    m_moduleFile.open(QFile::ReadOnly);
    QTextStream modueFileStream(&m_moduleFile);
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
        throw new Error(Error::ModuleImportError,
                        QStringLiteral("Error while processing module %1 %2.%3")
                        .arg(module->importDescription().name)
                        .arg(module->importDescription().versionMajor)
                        .arg(module->importDescription().versionMinor),
                        err);
    }

    AST::Program *ast = AST::cast<AST::Program *>(parser->rootNode());

    try {
        RegisterModuleVisitor registerModuleVisitor(module);
        TypeDefinitionVisitor typeDefinitionVisitor(module);
        ast->accept(&registerModuleVisitor);
        ast->accept(&typeDefinitionVisitor);
    } catch (Error *e) {
        e->setFile(m_moduleFile.fileName());
        throw e;
    }

    module->setLoadingState(IR::Module::Successful);
}
