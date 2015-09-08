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

#ifndef ABSTRACTMODULELOADER_H
#define ABSTRACTMODULELOADER_H

//Qt
#include <QtCore/QRunnable>

namespace QmlJSc {

namespace IR {
    class Module;
}

/**
 * This class provides an abstract base class for classes that can load modules.
 * Examples for base classes are the javascript-module loader and the
 * qtqml-module loader.
 *
 * It brings the basic infrastructure to run it on it's own thread (so module
 * loaders will always run asynchronously in their own thread) and to figure out
 * if it's the right loader for a specific module.
 *
 * If you implement this class, you need to implement canLoad, telling if the
 * class is suitable to load this module and doLoad, to actually load it.
 * Use module() to get the module you're about to load and add types to it.
 */
class AbstractModuleLoader : public QRunnable
{
public:
    AbstractModuleLoader(IR::Module *module);

    /**
     * Don't reimplement this in a moduleloader.
     */
    void run() override;

    virtual bool canLoad() = 0;
    virtual void doLoad() = 0;

    IR::Module *module();


private:
    IR::Module *m_module;
};

} // namespace QMLJSc

#endif // ABSTRACTMODULELOADER_H
