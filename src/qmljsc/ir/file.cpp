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

#include "file.h"
#include "module.h"
#include "../error.h"
#include "../compiler.h"
#include "visitor.h"

using namespace QmlJSc;
using namespace QmlJSc::IR;

bool File::ModuleData::operator==(const File::ModuleData& other) const
{
    return module == other.module;
}

File::File()
    : m_prefix('A' - 1) // we want the prefix to be 'A' after the first preincrement
{
}

File::~File()
{
}

void File::addModule(Module *module)
{
    if (m_importedModules.contains({ module, QString() }))
        return;

    m_importedModules.append({
        module,
        ++m_prefix
    });
}

Type *File::type(const QString &typeName) const
{
    const ModuleData *data = moduleForType(typeName);
    if (data && data->module)
        return data->module->type(typeName);

    return 0;
}

QString File::fullyQualifiedName(const QString &typeName)
{
    return QStringLiteral("%1.%2").arg(moduleForType(typeName)->localPrefix, typeName);
}

Object *File::rootObject() const
{
    return m_rootObject;
}

void File::setRootObject(Object *rootObject)
{
    m_rootObject = rootObject;
}

const File::ModuleData *File::moduleForType(const QString &typeName) const
{
    Type *foundType = 0;
    const ModuleData *moduleData = 0;
    foreach (const ModuleData &data, m_importedModules) {
        if (Type * type = data.module->type(typeName)) {
            if (foundType) {
                throw Error(
                    QmlJSc::Error::SymbolLookupError,
                    QString("Ambigious type name. Type %1 was defined by module %2 and %3.")
                    .arg(typeName, moduleData->module->name(), data.module->name())
                );
                return 0;
            } else {
                foundType = type;
                moduleData = &data;
            }
        }
    }
    return moduleData;
}

void QmlJSc::IR::File::accept(QmlJSc::IR::Visitor* visitor)
{
    visitor->visit(this);
    m_rootObject->accept(visitor);
    visitor->endVisit(this);
}
