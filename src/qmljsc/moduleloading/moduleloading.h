/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#ifndef MODULELOADING_H
#define MODULELOADING_H

// Qt
#include <QtCore/QRunnable>

// std
#include <functional>

// own
#include "../ir/file.h"

namespace QmlJSc {

namespace IR {
    class Class;
}

class AbstractModuleLoader;

class ModuleLoading {
public:
    /**
     * Initializes module loading.
     * Creates a AbstractModuleLoader object and runs it through QThreadPool.
     * Beware, that it is an asynchronous function and returns a valid but
     * uncompleted Module as soon as loading *started* (not when finished).
     *
     * This function is to be called from the compile-unit thread.
     */
    static IR::Module *loadModule(IR::ImportDescription import);

    static void registerModuleLoader(std::function<AbstractModuleLoader *()> factory);

private:
    static QHash<IR::ImportDescription, IR::Module*> s_loadedModules;
    static QVector<std::function<AbstractModuleLoader *()>> s_moduleLoaders;
};

} // namespace QMLJSc

#endif // MODULELOADING_H

