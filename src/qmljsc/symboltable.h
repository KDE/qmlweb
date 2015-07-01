/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
 * Copyright (C) 2015 Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

class Module;
struct Type;

inline uint qHash(const ModuleImport &key, uint seed)
{
    return qHash(key.name, seed) ^ key.versionMajor ^ key.versionMinor;
}

typedef QList<ModuleImport> ModuleImports;

class SymbolTable : public QObject
{
    Q_OBJECT

    struct ModuleData {
        Module *module;
        QString localPrefix;
    };

public:
    explicit SymbolTable(QObject *parent = 0);
    virtual ~SymbolTable();

    void loadModule(ModuleImport import);

    Type* type(const QString &typeName);
    QString fullyQualifiedName(const QString &typeName);

signals:
    void importError(QmlJSc::Error error);
    void symbolLookupError(QmlJSc::Error error);

private slots:
    void doLoadModule(ModuleImport import);

private:
    const ModuleData *moduleForType(const QString &typeName);
    QString getNewPrefix(bool capital = false);

    QHash<ModuleImport, ModuleData> m_modules;
    int m_prefixCount;
};

}

Q_DECLARE_METATYPE(QmlJSc::ModuleImport);

#endif // SYMBOLTABLE_H
