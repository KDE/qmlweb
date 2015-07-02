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

#include "file.h"

namespace QmlJSc {
namespace IR {

/**
 * This class provides API representing a Qml.js module and allows to learn
 * about the modules API.
 *
 * Therefore it parses the javascript source of the module and analyses it for
 * class definitions, properties, methods, etc.
 */
class Module
{
public:
    enum Status {
        Loading,
        Successful,
        ErrorState
    };

    explicit Module(ImportDescription import, QObject *parent = 0);

    Status status();
    void waitForLoaded();

    Type *type(QString name);

    const QString &name();

private:
    QHash<QString, Type*> m_types;
    ImportDescription m_import;
    Status m_status;
    QWaitCondition m_waitCondition;
    QMutex m_loadMutex;

    friend class ModuleLoader;
};

class ModuleLoader : public QRunnable, public QQmlJS::AST::Visitor
{
public:
    static Module *loadModule(ImportDescription import, QObject *parent = 0);

    void run() override;

private:
    ModuleLoader(Module *module);
    virtual ~ModuleLoader();

    void doLoad();
    bool visit(QQmlJS::AST::FunctionExpression*) override;
    void endVisit(QQmlJS::AST::FunctionExpression*) override;
    bool visit(QQmlJS::AST::FunctionDeclaration*) override;
    void endVisit(QQmlJS::AST::FunctionDeclaration*) override;
    bool visit(QQmlJS::AST::ReturnStatement *returnStatement) override;

    void finalizeParse();

    static QHash<ImportDescription, Module*> s_loadedModules;

    Module *m_module;
    int m_functionDepth;
    QStringRef m_currentFunction;
    QHash<QString, QVector<QString>> m_functionProperties;
    QHash<QString, QStringRef> m_typesToFunctionsMap;
};

} // namespace IR
} // namespace QMLJSc

#endif // MODULELOADER_H
