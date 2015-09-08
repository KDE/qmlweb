/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
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

// Own
#include "moduleloading.h"
#include "abstractmoduleloader.h"
#include "ir/module.h"
#include "ir/ir.h"
#include "compiler.h"
#include "error.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QThread>
#include <QtCore/QThreadPool>

// Qt private
#include <private/qqmljsengine_p.h>
#include <private/qqmljslexer_p.h>
#include <private/qqmljsparser_p.h>

#define assert(condition, token, errorMessage) \
if (!condition) { \
    assertFailed(token, errorMessage); \
}


using namespace QmlJSc;
using namespace QQmlJS;

QHash<IR::ImportDescription, IR::Module*> ModuleLoading::s_loadedModules;
QVector<std::function<AbstractModuleLoader *()>> ModuleLoading::s_moduleLoaders;

IR::Module *ModuleLoading::loadModule(IR::ImportDescription import)
{
    Q_ASSERT(import.kind == IR::ImportDescription::Kind_ModuleImport);
    if (IR::Module *module = s_loadedModules.value(import)) {
        return module;
    }

    IR::Module *module = new IR::Module(import, compiler);
    s_loadedModules.insert(import, module);
    AbstractModuleLoader *loader = 0;
    foreach (auto factory, s_moduleLoaders) {
        loader = factory(); // will delete itself when done
        loader->setModule(module);
        if (loader->canLoad()) {
            break;
        } else {
            delete loader;
            loader = 0;
        }
    }

    if (!loader) {
        throw new Error(Error::ModuleImportError, QStringLiteral("Module %1 %2.%3 was not "
                        "found. Check if it is installed and include paths are correctly set"));
    }

    QThreadPool::globalInstance()->start(loader);

    return module;
}

void ModuleLoading::registerModuleLoader(std::function<AbstractModuleLoader *()> factory)
{
    s_moduleLoaders.append(factory);
}

