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

File::File()
    : m_prefix('A' - 1) // we want the prefix to be 'A' after the first preincrement
{
}

File::~File()
{
}

void File::addImport(ImportDescription import)
{
    switch (import.kind) {
        case ImportDescription::Kind_ModuleImport:
        if (m_modules.contains(import))
            return;

        m_modules.insert(import, {
            ModuleLoader::loadModule(import, compiler),
            ++m_prefix
        });
        break;
    }
}

Type *File::type(const QString &typeName)
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

const File::ModuleData *File::moduleForType(const QString &typeName)
{
    Type *foundType = 0;
    const ModuleData *moduleData = 0;
    for (auto i = m_modules.constBegin(); i != m_modules.constEnd(); i++) {
        if (Type * type = i->module->type(typeName)) {
            if (foundType) {
                throw Error(
                            QmlJSc::Error::SymbolLookupError,
                            QString("Ambitious type name. Type %1 was defined by module %2 and %3.")
                                .arg(typeName, moduleData->module->name(), i->module->name())
                           );
                return 0;
            } else {
                foundType = type;
                moduleData = &(*i);
            }
        }
    }
    return moduleData;
}
