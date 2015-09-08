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

#ifndef MODULE_H
#define MODULE_H

#include "file.h"

namespace QmlJSc {

class ModuleLoader;

namespace IR {

/**
 * This class provides API representing a Qml.js module and allows to learn
 * about the modules API.
 *
 * Therefore it parses the javascript source of the module and analyses it for
 * class definitions, properties, methods, etc.
 */
class Module
{
public:
    enum Status {
        Loading,
        Successful,
        ErrorState
    };

    explicit Module(ImportDescription import, QObject *parent = 0);
    ~Module();

    Status status();
    void setStatus(Status status);

    void waitForLoaded();

    /**
     * Returns the type object for the (QML-) name.
     *
     * If the data isn't available, yet, because the module is still loading,
     * this function will block until the data is available.
     */
    Type *type(QString name);

    /**
     * Returns the type object for the name it has in JS code.
     *
     * This function won't wait for the data to be available, only use this
     * function if you're certain about it!
     */
    Type *typeFromJSName(QString name);

    const QString &name();

    void addType(Type *type);

    ImportDescription importDescription();

private:
    QHash<QString, Type*> m_types;
    QHash<QString, Type*> m_jsNameToTypeHash;
    ImportDescription m_import;
    Status m_status;
    QWaitCondition m_waitCondition;
    QMutex m_loadMutex;
};

} // namespace IR
} // namespace QMLJSc

#endif // MODULE_H
