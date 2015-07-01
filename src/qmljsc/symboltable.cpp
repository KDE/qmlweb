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

#include "symboltable.h"
#include "module.h"

using namespace QmlJSc;

SymbolTable::SymbolTable(QObject* parent)
    : QObject(parent)
    , m_prefixCount(0)
{
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::loadModule(ModuleImport import)
{
    if (m_modules.contains(import))
        return;

    if (Module *module = Module::loadedModules.value(import)) {
        m_modules.insert(import, { module, getNewPrefix(true) });
        return;
    }

    Module *module = new Module(import);
    Module::loadedModules.insert(import, module);
    m_modules.insert(import, { module, getNewPrefix(true) });

    connect(module, SIGNAL(importError(QmlJSc::Error)), this, SIGNAL(importError(QmlJSc::Error)));

    QMetaObject::invokeMethod(module, "doLoad");
}

Type *SymbolTable::type(const QString &typeName)
{
    const ModuleData *data = moduleForType(typeName);
    if (data && data->module)
        return data->module->type(typeName);

    return 0;
}

QString SymbolTable::fullyQualifiedName(const QString &typeName)
{
    return QStringLiteral("%1.%2").arg(moduleForType(typeName)->localPrefix, typeName);
}

const SymbolTable::ModuleData *SymbolTable::moduleForType(const QString &typeName)
{
    Type *foundType = 0;
    const ModuleData *moduleData = 0;
    for (auto i = m_modules.constBegin(); i != m_modules.constEnd(); i++) {
        if (Type * type = i->module->type(typeName)) {
            if (foundType) {
                emit symbolLookupError({ Error::SymbolLookupError,
                                         QString("Ambitious type name. Type %1 was defined by module %2 and %3.")
                                                .arg(typeName, moduleData->module->name(), i->module->name())
                                       });
                return 0;
            } else {
                foundType = type;
                moduleData = &(*i);
            }
        }
    }
    return moduleData;
}

QString SymbolTable::getNewPrefix(bool capital)
{
    QString prefix;
    int number = ++m_prefixCount;

    do {
        number -= 1;
        prefix.prepend((capital ? 'A' : 'a') + (number % 26));
        number = number / 26;
    } while(number > 0);

    return prefix;
}

// QString SymbolTable::findType(ModuleImport module, QString typeName)
// {
//     if (!m_modules.contains(module))
//         return QString();
//
//     Module *data = m_modules.value(module);
//     if (data->status() == Module::Loading) { // Wait for data to be available.
//         data->waitForLoaded();
//
//         if (data->status() == Module::ErrorState) // If an error occurred, the module won't exist anymore
//             return QString();
//     }
//
// //     return data->typeToFQIHash.value(typeName);
// }
//
// QString SymbolTable::findType(ModuleImports modules, QString typeName)
// {
//     if (!modules.contains({"QtQml", 2, 0})) {
//         modules.append({"QtQml", 2, 0});
//     }
//     foreach (ModuleImport module, modules) {
//         QString result = findType(module, typeName);
//         if (!result.isEmpty()) {
//             return result;
//         }
//     }
//     return QString();
// }

void SymbolTable::doLoadModule(ModuleImport import)
{

}
