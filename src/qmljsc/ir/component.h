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

#ifndef COMPONENT_H
#define COMPONENT_H

#include "object.h"

namespace QmlJSc {
namespace IR {

class Component : public Object
{
public:
    Component();

    Object *object(const QString &id);

private:
    QHash<QString, Object*> m_ids;

    Component *m_parentComponent;

    friend class TestIR;
};

} // namespace IR
} // namespace QMLJSc

#endif // COMPONENT_H
