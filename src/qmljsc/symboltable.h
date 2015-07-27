/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
 * Copyright (C) 2015 Anton Kreuzkamp <akreuzkamp@web.de>
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

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "error.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QStringList>

class QJSEngine;

namespace QmlJSc {

struct ModuleImport {
    QString name;
    int versionMajor;
    int versionMinor;

    inline bool operator==(const ModuleImport& other) const
    {
        return name == other.name && versionMajor == other.versionMajor && versionMinor == other.versionMinor;
    }
};

inline uint qHash(const ModuleImport &key, uint seed)
{
    return qHash(key.name, seed) ^ key.versionMajor ^ key.versionMinor;
}

struct ModuleData {
    QHash<QString, QString> typeToFQIHash;
    bool loaded;
    QWaitCondition waitForLoaded;
    QMutex loadMutex;
};

typedef QList<ModuleImport> ModuleImports;

class SymbolTable : public QObject
{
    Q_OBJECT

public:
    explicit SymbolTable(QObject *parent = 0);
    virtual ~SymbolTable();

    void addIncludePath(QString path);

    void loadModule(ModuleImport import);

    QString findType(ModuleImport module, QString typeName);
    QString findType(ModuleImports modules, QString typeName);

signals:
    void importError(QmlJSc::Error error);

private slots:
    void doLoadModule(ModuleImport import);

private:
    void error(ModuleImport import, QString message, Error *reason = 0);

    QHash<ModuleImport, ModuleData*> m_modules;
    QStringList m_includePaths;
    QJSEngine *m_jsEngine;
};

}

Q_DECLARE_METATYPE(QmlJSc::ModuleImport);

#endif // SYMBOLTABLE_H
