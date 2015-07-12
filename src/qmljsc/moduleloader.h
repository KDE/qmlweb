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

#ifndef MODULELOADER_H
#define MODULELOADER_H

//Qt
#include <QtCore/QRunnable>

// private Qt
#include <private/qqmljsastvisitor_p.h>
#include <private/qqmljsast_p.h>

#include "ir/file.h"

namespace QmlJSc {

namespace IR {
    class Class;
}

/**
 * This class parses the javascript source of a module and analyses it for
 * class definitions, properties, methods, etc. to create a Module object.
 *
 * It will always run asynchronously in its own thread.
 *
 * To load a module, use the static function ModuleLoader::loadModule().
 */
class ModuleLoader : public QRunnable, public QQmlJS::AST::Visitor
{
public:
    /**
     * Initializes module loading.
     * Creates a ModuleLoader object and runs it through QThreadPool.
     * Beware, that it is an asynchronous function and returns a valid but
     * uncompleted Module as soon as loading *started* (not when finished).
     *
     * This function is to be called from the compile-unit thread.
     */
    static IR::Module *loadModule(IR::ImportDescription import);

private:
    ModuleLoader(IR::Module *module);
    virtual ~ModuleLoader();

    void run() override;

    void doLoad();
    bool visit(QQmlJS::AST::FunctionExpression*) override;
    void endVisit(QQmlJS::AST::FunctionExpression*) override;
    bool visit(QQmlJS::AST::FunctionDeclaration*) override;
    void endVisit(QQmlJS::AST::FunctionDeclaration*) override;
    bool visit(QQmlJS::AST::BinaryExpression*) override;
    bool visit(QQmlJS::AST::VariableDeclaration*) override;
    bool visit(QQmlJS::AST::CallExpression *call) override;

    void findModuleRegistration(QQmlJS::AST::CallExpression *call);
    void findInheritance(QQmlJS::AST::CallExpression *call);

    void findPropertyDefinition(QQmlJS::AST::BinaryExpression *expr);
    void findMethodDefinition(QQmlJS::AST::BinaryExpression *expr);
    void findSignalDefinition(QQmlJS::AST::BinaryExpression *expr);

    void finalizeParse();

    IR::Class *getPreliminaryClass(const QStringRef &name);
    void assert(bool condition, const QQmlJS::AST::SourceLocation &token, QString errorMessage);

    static QHash<IR::ImportDescription, IR::Module*> s_loadedModules;

    IR::Module *m_module;
    QHash<QStringRef, QStringRef> m_inheritancies;
    QVector<QQmlJS::AST::FunctionExpression *> m_currentFunctionStack;
    QHash<QStringRef, IR::Class *> m_preliminaryClasses;
    QHash<QString, QStringRef> m_typesToFunctionsMap;
    QString m_moduleFileName;
};

} // namespace QMLJSc

#endif // MODULELOADER_H

