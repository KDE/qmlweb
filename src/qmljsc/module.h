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

// #include <hashtable.h>
#include <private/qqmljsastvisitor_p.h>
#include <private/qqmljsast_p.h>

#include "symboltable.h"

namespace QmlJSc {

struct Type {
    QString name;
};

/**
 * This class provides API representing a Qml.js module and allows to learn
 * about the modules API.
 *
 * Therefore it parses the javascript source of the module and analyses it for
 * class definitions, properties, methods, etc.
 */
class Module : public QObject, public QQmlJS::AST::Visitor
{
    Q_OBJECT

public:
    enum Status {
        Loading,
        Successful,
        ErrorState
    };

    static QHash<ModuleImport, Module*> loadedModules;

    explicit Module(ModuleImport import, QObject *parent = 0);

    Status status();
    void waitForLoaded();

    inline Type *type(QString name)
    {
        waitForLoaded();
        return m_types.value(name);
    }

    const QString &name();

public slots:
    void doLoad();

signals:
    void importError(QmlJSc::Error error);




private:
    bool visit(QQmlJS::AST::FunctionExpression*) override;
    void endVisit(QQmlJS::AST::FunctionExpression*) override;
    bool visit(QQmlJS::AST::FunctionDeclaration*) override;
    void endVisit(QQmlJS::AST::FunctionDeclaration*) override;
    bool visit(QQmlJS::AST::ReturnStatement *returnStatement);
    void error(QString message, Error *reason = 0);
    void finalizeParse();

    struct {
        int functionDepth;
        QStringRef currentFunction;
        QHash<QString, QVector<QString>> functionProperties;
        QHash<QString, QStringRef> typesToFunctionsMap;
    } m_parseData;

    QHash<QString, Type*> m_types;
    ModuleImport m_import;
    Status m_status;
    QWaitCondition m_waitCondition;
    QMutex m_loadMutex;
};

}

#endif // MODULELOADER_H
