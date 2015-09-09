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

#ifndef QTQMLMODULELOADER_H
#define QTQMLMODULELOADER_H

#include "abstractmoduleloader.h"

//Qt
#include <QtCore/QRunnable>

namespace QmlJSc {

namespace IR {
    class Module;
}

/**
 * This class creates the QtQml module, including all the basic types that exist
 * in QML and ECMAScript. Especially as there's no module for the ECMAScript
 * API, we have an extra loader class for it.
 */
class QtQmlModuleLoader : public AbstractModuleLoader
{
public:
    static QtQmlModuleLoader *create(IR::Module *module);

    bool canLoad() override;
    void doLoad() override;

private:
    QtQmlModuleLoader(IR::Module *module);
};

} // namespace QMLJSc

#endif // QTQMLMODULELOADER_H
