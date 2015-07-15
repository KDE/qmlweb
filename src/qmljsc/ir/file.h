/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#ifndef FILE_H
#define FILE_H

#include "ir.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QStringList>

namespace QmlJSc {
namespace IR {

struct ImportDescription {
    enum Kind {
        Kind_ModuleImport,
        Kind_FileImport,
        Kind_DirectoryImport
    };

    Kind kind;
    QString name;
    int versionMajor;
    int versionMinor;

    inline bool operator==(const ImportDescription& other) const
    {
        return name == other.name && versionMajor == other.versionMajor && versionMinor == other.versionMinor;
    }
};

class Module;
struct Type;

inline uint qHash(const ImportDescription &key, uint seed)
{
    return qHash(key.name, seed) ^ key.versionMajor ^ key.versionMinor;
}

typedef QList<ImportDescription> ImportDescriptions;

/**
 * This class represents a minified symbol name like "a", "b", "aa", "A8",...
 *
 * Initialize it with the first character you want to use as symbol name MINUS
 * ONE, as you normally will use the preincrement operator before accessing it.
 * If you won't, feel free to initialize it with the first symbol name directly,
 * of course.
 *
 * To get the next valid symbol name, use the preincrement operator.
 *
 * This is a subclass of QString, so you can just use it as a string.
 */
class ShortSymbolName : public QString
{
public:
    ShortSymbolName(char first);
    ShortSymbolName(QString first);

    ShortSymbolName &operator++();
};

class File
{
    struct ModuleData {
        Module *module;
        QString localPrefix;

        bool operator==(const ModuleData &other) const;
    };

public:
    explicit File();
    virtual ~File();

    void addModule(Module *module);

    Type* type(const QString &typeName) const;
    QString fullyQualifiedName(const QString &typeName);

    Component *rootObject() const;
    void setRootObject(Component *root);

private:
    const ModuleData *moduleForType(const QString &typeName) const;

    QVector<ModuleData> m_modules;
    ShortSymbolName m_prefix;
    Component *m_rootObject;
};

} // namespace IR
} // namespace QMLJSc

Q_DECLARE_METATYPE(QmlJSc::IR::ImportDescription);

#endif // FILE_H
