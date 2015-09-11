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

#ifndef FILE_H
#define FILE_H

#include "objecttree.h"
#include "importdefinition.h"
#include "../utils/shortsymbolname.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QStringList>

namespace QmlJSc {
namespace IR {

class Module;
struct Type;

class File : public Node
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

    Object *rootObject() const;
    void setRootObject(Object *root);

    virtual void accept(Visitor * visitor) override;

private:
    const ModuleData *moduleForType(const QString &typeName) const;

    QVector<ModuleData> m_importedModules;
    ShortSymbolName m_prefix;
    Object *m_rootObject;
};

} // namespace IR
} // namespace QMLJSc

#endif // FILE_H
