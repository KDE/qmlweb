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

#ifndef JAVASCRIPTMODULELOADER_H
#define JAVASCRIPTMODULELOADER_H

// Qt
#include <QtCore/QRunnable>
#include <QtCore/QFile>

// private Qt
#include <private/qqmljsastvisitor_p.h>
#include <private/qqmljsast_p.h>

// own
#include "abstractmoduleloader.h"
#include "../ir/file.h"

namespace QmlJSc {

namespace IR {
    class Class;
    class Module;
}

class RegisterModuleVisitor : public QQmlJS::AST::Visitor
{
public:
    explicit RegisterModuleVisitor(IR::Module *module);

    bool visit(QQmlJS::AST::CallExpression *call) override;

private:
    IR::Module *m_module;
};

class TypeDefinitionVisitor : public QQmlJS::AST::Visitor
{
public:
    explicit TypeDefinitionVisitor(IR::Module *module);

    bool visit(QQmlJS::AST::FunctionExpression*) override;
    void endVisit(QQmlJS::AST::FunctionExpression*) override;
    bool visit(QQmlJS::AST::FunctionDeclaration*) override;
    void endVisit(QQmlJS::AST::FunctionDeclaration*) override;
    bool visit(QQmlJS::AST::BinaryExpression*) override;
    bool visit(QQmlJS::AST::VariableDeclaration*) override;
    bool visit(QQmlJS::AST::CallExpression *call) override;

    void findPropertyDefinition(QQmlJS::AST::BinaryExpression *expr);
    void findMethodDefinition(QQmlJS::AST::BinaryExpression *expr);
    void findSignalDefinition(QQmlJS::AST::BinaryExpression *expr);

    /**
     * Looks for the type with the given name at several places and returns it,
     * if found or, if not, it will throw an error.
     *
     * This is used when seeing a referenced type.
     */
    IR::Type *getType(const QStringRef &name);

private:
    IR::Module *m_module;

    /**
     * Stack of nested functions (closures) we're in.
     * Each function is represented by it's AST subtree.
     */
    QVector<QQmlJS::AST::FunctionExpression *> m_currentFunctionStack;
};

/**
 * This class parses the javascript source of a module and analyses it for
 * class definitions, properties, methods, etc. to create a Module object.
 *
 * It will always run asynchronously in its own thread.
 *
 * To load a module, use the static function ModuleLoading::loadModule().
 */
class JavaScriptModuleLoader : public AbstractModuleLoader
{
public:
    static JavaScriptModuleLoader *create(IR::Module *module);

    bool canLoad();

    void doLoad();

private:
    JavaScriptModuleLoader(IR::Module *module);
    QFile m_moduleFile;
};

} // namespace QMLJSc

#endif // JAVASCRIPTMODULELOADER_H
