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

using namespace QmlJSc;

SymbolTable::SymbolTable(QObject* parent)
    : QObject(parent)
{
    ModuleData *builtInModule = new ModuleData;
    builtInModule->typeToFQIHash.insert("Component", "QtQml20.Component");
    builtInModule->typeToFQIHash.insert("QtObject", "QtQml20.QtObject");
    builtInModule->typeToFQIHash.insert("Binding", "QtQml20.Binding");
    builtInModule->typeToFQIHash.insert("Connections", "QtQml20.Connections");
    builtInModule->typeToFQIHash.insert("Timer", "QtQml20.Timer");
    builtInModule->loaded = true;
    m_modules.insert({"QtQml", 2, 0}, builtInModule);
}

SymbolTable::~SymbolTable()
{
    foreach (ModuleData *module, m_modules) {
        delete module;
    }
}

void SymbolTable::loadModule(ModuleImport import)
{
    if (m_modules.contains(import))
        return;

    m_modules.insert(import, new ModuleData);

    QMetaObject::invokeMethod(this, SLOT(doLoadModule()), Q_ARG(ModuleImport, import));
}

QString SymbolTable::findType(ModuleImport module, QString typeName)
{
    Q_ASSERT(m_modules.contains(module));

    ModuleData *data = m_modules[module];
    if (!data->loaded) { // Wait for data to be available.
        data->loadMutex.lock();
        data->waitForLoaded.wait(&data->loadMutex);
        data->loadMutex.unlock();
    }

    return data->typeToFQIHash.value(typeName);
}

QString SymbolTable::findType(ModuleImports modules, QString typeName)
{
    if (!modules.contains({"QtQml", 2, 0})) {
        modules.append({"QtQml", 2, 0});
    }
    foreach (ModuleImport module, modules) {
        QString result = findType(module, typeName);
        if (!result.isEmpty()) {
            return result;
        }
    }
    return QString();
}

void SymbolTable::doLoadModule(ModuleImport import)
{

}
