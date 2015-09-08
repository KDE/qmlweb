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
 * This class parses the javascript source of a module and analyses it for
 * class definitions, properties, methods, etc. to create a Module object.
 *
 * It will always run asynchronously in its own thread.
 *
 * To load a module, use the static function Modules::loadModule().
 *
 * If you implement a module loader, reimplement doLoad and setModule. If an
 * error occurs during loading, throw an exception. AbstractModuleLoader will
 * take care of communicating it up.
 */
class AbstractModuleLoader : public QRunnable
{
public:

    /**
     * Don't reimplement this in a moduleloader. It's
     */
    void run() override;

    virtual bool canLoad() = 0;
    virtual void doLoad() = 0;

    /**
     * Needs to be called *before* run() is called.
     */
    void setModule(IR::Module *module);
    IR::Module *module();


private:
    IR::Module *m_module;
};

} // namespace QMLJSc

#endif // ABSTRACTMODULELOADER_H
