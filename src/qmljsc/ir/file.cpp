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

#include "file.h"
#include "module.h"
#include "../error.h"
#include "../compiler.h"

using namespace QmlJSc;
using namespace QmlJSc::IR;

ShortSymbolName::ShortSymbolName(char first)
    : QString(first)
{}

ShortSymbolName::ShortSymbolName(QString first)
    : QString(first)
{}

ShortSymbolName &ShortSymbolName::operator++()
{
    Q_ASSERT(!isEmpty());

    iterator i = end();
    i--;

    char borrow = 1;

    while (i != begin()) {
        if ((*i) == '9') {
            *i = 'A';
            borrow = 0;
            break;
        } else if ((*i) == 'Z') {
            *i = 'a';
            borrow = 0;
            break;
        } else if ((*i) == 'z') {
            // We need to add a borrow of 1 to the previous digit
            *i = '0';
            i--;
            borrow = 1;
            continue;
        } else {
            *i = i->toLatin1() + 1;
            borrow = 0;
            break;
        }
    }

    if (borrow == 1) {
        if (*i <= 'Z') { // the first letter is a capital one, so it should remain so.
            if (*i == 'Z') { // We need to prepend a new digit
                *i = '0';
                prepend('A');
            } else {
                *i = i->toLatin1() + 1;
            }
        } else { // the first letter is a small one, so it should remain so.
            if (*i == 'z') { // We need to prepend a new digit
                *i = '0';
                prepend('a');
            } else {
                *i = i->toLatin1() + 1;
            }
        }
    }

    return *this;
}

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
    if (m_modules.contains({ module, QString() }))
        return;

    m_modules.append({
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

Component *File::rootObject() const
{
    return m_rootObject;
}

void File::setRootObject(Component *rootObject)
{
    m_rootObject = rootObject;
}

const File::ModuleData *File::moduleForType(const QString &typeName) const
{
    Type *foundType = 0;
    const ModuleData *moduleData = 0;
    foreach (const ModuleData &data, m_modules) {
        if (Type * type = data.module->type(typeName)) {
            if (foundType) {
                throw Error(
                            QmlJSc::Error::SymbolLookupError,
                            QString("Ambitious type name. Type %1 was defined by module %2 and %3.")
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
