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

    /**
     * Returns the preliminary class object with the given name.
     * If it doesn't exist yet, it creates one and returns that.
     *
     * This is used inside class definitions.
     */
    IR::Class *getPreliminaryClass(const QStringRef &name);

    /**
     * Looks for the type with the given name at several places and returns it,
     * if found or, if not, creates a preliminary class, marks it as referenced
     * and then returns it.
     *
     * This is used when seeing a referenced type.
     */
    IR::Type *getType(const QStringRef &name, QQmlJS::AST::SourceLocation location);
    void assertFailed(const QQmlJS::AST::SourceLocation &token, QString errorMessage);

    static QHash<IR::ImportDescription, IR::Module*> s_loadedModules;

    IR::Module *m_module;
    QString m_moduleFileName;

    /**
     * This set contains pointers to types that are referenced elsewhere.
     * If we see a function definition, we only can speculate, whether this is a
     * constructor function of a type to be registered with QML. The same
     * applies if we see a reference to a type. We can't be certain if it will
     * be defined and registered lateron.
     *
     * Thus, we simply create a type object for it, as soon as we need it the
     * first time and destroy it, if it wasn't registered. This is totaly valid,
     * if it was created because we saw a function definition. It isn't ok
     * anymore, though, if it was referenced somewhere. So we collect those
     * referenced types here, to check, that all of them have been registered.
     */
    QHash<IR::Type *, QQmlJS::AST::SourceLocation> m_referencedTypes;
    /**
     * Stack of nested functions (closures) we're in.
     * Each function is represented by it's AST subtree.
     */
    QVector<QQmlJS::AST::FunctionExpression *> m_currentFunctionStack;
    /**
     * Contains functions that *might* be a constructor function for a class as
     * key and IR::Class instances as value.
     * If an entry turns out not to contain a valid class, we simply destroy it
     * again.
     */
    QHash<QStringRef, IR::Class *> m_preliminaryClasses;
    /**
     * Maps the name, a type will have in QML to the name of the constructor
     * function in JavaScript. All other occurancies of constructor functions
     * use the js-name.
     */
    QHash<QString, QStringRef> m_typesToFunctionsMap;
};

} // namespace QMLJSc

#endif // MODULELOADER_H
