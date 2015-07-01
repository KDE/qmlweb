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

#include "component.h"

using namespace QmlJSc::IR;

Component::Component()
    : Object()
{
    m_kind = Type::Kind_Component;
}

Object *Component::object(const QString &id)
{
    if (m_ids.contains(id)) {
        return m_ids[id];
    }
    if (m_parentComponent) {
        return m_parentComponent->object(id);
    }
    return 0;
}
