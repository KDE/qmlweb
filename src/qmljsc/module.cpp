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
#include "module.h"
#include "compiler.h"
#include "error.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QTextStream>

// Qt private
#include <private/qqmljsengine_p.h>
#include <private/qqmljslexer_p.h>
#include <private/qqmljsparser_p.h>

using namespace QmlJSc;
using namespace QQmlJS::AST;

QHash<ModuleImport, Module*> Module::loadedModules;

Module::Module(ModuleImport import, QObject *parent)
    : QObject(parent)
    , m_parseData({0})
    , m_import(import)
    , m_status(Loading)
{
}

void Module::doLoad()
{
    // === Load Module File ===
    QFile moduleFile;
    const QString moduleFileName = QStringLiteral("%1.%2.%3.js").arg(m_import.name)
                                                                .arg(m_import.versionMajor)
                                                                .arg(m_import.versionMinor);

    // For now we only support local files.
    const QStringList &includePaths = compiler->includePaths();
    foreach (QString includePath, includePaths) {
        QDir includeDir(includePath);
        if (includeDir.exists(moduleFileName)) {
            moduleFile.setFileName(includeDir.absoluteFilePath(moduleFileName));
            break;
        }
    }

    if (!moduleFile.exists()) {
        error(QStringLiteral("Could not find file %1 in path.").arg(moduleFileName));
        return;
    }

    // Read file
    moduleFile.open(QFile::ReadOnly);
    QTextStream modueFileStream(&moduleFile);
    QString moduleSource = modueFileStream.readAll();

    // === Parse file ===

    // parsing happens in three steps: Calling the QQmlJS-parser to parse the
    // file and return an AST, then using the visit functions of this class to
    // collect the data we need and put it into m_parseData and third calling
    // finalizeParse() to evaluate the parse data and transform it to actual
    // type information.

    QQmlJS::Engine* engine = new QQmlJS::Engine();
    QQmlJS::Lexer* lexer = new QQmlJS::Lexer(engine);

    lexer->setCode(moduleSource, 1, true);

    QQmlJS::Parser* parser = new QQmlJS::Parser(engine);


    bool successfullyParsed = parser->parseProgram();
    if (!successfullyParsed) {
        Error *err = new Error(Error::ParseError, parser->errorMessage());
        err->setColumn(parser->errorColumnNumber());
        err->setLine(parser->errorLineNumber());
        error(QStringLiteral("Error while processing module %1 %2.%3")
                        .arg(m_import.name).arg(m_import.versionMajor).arg(m_import.versionMinor), err);
        return;
    }

    parser->rootNode()->accept(this);

    finalizeParse();

    m_status = Successful;
    m_waitCondition.wakeAll(); // Wake up threads, that wait to access this module.
}

bool Module::visit(QQmlJS::AST::FunctionDeclaration* func)
{
    m_parseData.functionDepth++;
    if (m_parseData.functionDepth == 2)
        m_parseData.currentFunction = func->name;
    return true;
}
void Module::endVisit(QQmlJS::AST::FunctionDeclaration* func)
{
    m_parseData.functionDepth--;
    if (m_parseData.functionDepth == 1)
        m_parseData.currentFunction.clear();
}

bool Module::visit(QQmlJS::AST::FunctionExpression* func)
{
    m_parseData.functionDepth++;
    return true;
}
void Module::endVisit(QQmlJS::AST::FunctionExpression* func)
{
    m_parseData.functionDepth--;
}

bool Module::visit(QQmlJS::AST::ReturnStatement *returnStatement)
{
    if (m_parseData.functionDepth != 1)
        return false;

    ObjectLiteral *returnLiteral = cast<ObjectLiteral*>(returnStatement->expression);
    if (!returnLiteral)
        return false;

    PropertyAssignmentList *assignment = returnLiteral->properties;

    while(assignment) {
        PropertyNameAndValue *nameAndValue = cast<PropertyNameAndValue*>(assignment->assignment);

        if (!nameAndValue) {
            qWarning() << "Ignoring invalid type specification.";
            continue;
        }

        IdentifierExpression *functionId = cast<IdentifierExpression*>(nameAndValue->value);
        if (!functionId) {
            qWarning() << "Can't recognize function identifier. Please use a simple identifier as value on type object.";
            continue;
        }

        m_parseData.typesToFunctionsMap.insert(nameAndValue->name->asString(), functionId->name);
        assignment = assignment->next;
    }

    return true;
}

void Module::finalizeParse()
{
    for (auto i = m_parseData.typesToFunctionsMap.constBegin(); i != m_parseData.typesToFunctionsMap.constEnd(); i++) {
        // the typesToFunctionsMap contains the most basic information, namely
        // which types exist. The key in this hash is the name of the type, the
        // value is the name of the function that defines it.
        Type *type = new Type();
        type->name = i.key();
        m_types.insert(i.key(), type);
    }

    m_parseData = {0}; // reset parse data
}

Module::Status Module::status()
{
    return m_status;
}


const QString &Module::name()
{
    return m_import.name;
}


void Module::waitForLoaded()
{
    if (m_status != Loading)
        return;

    m_loadMutex.lock();
    m_waitCondition.wait(&m_loadMutex);
    m_loadMutex.unlock();
}

void Module::error(QString message, Error *reason)
{
    m_status = ErrorState;
    m_waitCondition.wakeAll();
    emit importError({ Error::ModuleImportError, message, reason });
}

