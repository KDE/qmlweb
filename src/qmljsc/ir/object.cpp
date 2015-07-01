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

#include "object.h"

using namespace QmlJSc::IR;

Object::Object()
    : Type()
{
    m_kind = Type::Kind_Object;
}

QVector<ValueAssignment> &Object::valueAssignments()
{
    return m_valueAssignments;
}

QVector<BindingAssignment> &Object::bindingAssignments()
{
    return m_bindingAssignments;
}

ValueAssignment *Object::addValueAssignment()
{
    m_valueAssignments.append(ValueAssignment());
    return &m_valueAssignments.last();
}

BindingAssignment *Object::addBindingAssignment()
{
    m_bindingAssignments.append(BindingAssignment());
    return &m_bindingAssignments.last();
}

void Object::accept(Object::Visitor *visitor)
{
    visitor->visit(this);
    for (auto i = m_properties.begin(); i != m_properties.end(); i++) {
        visitor->visit(&i.value());
        if (i->objectValue) {
            i->objectValue->accept(visitor);
        }
        visitor->endVisit(&i.value());
    }
    for (auto i = m_methods.begin(); i != m_methods.end(); i++) {
        visitor->visit(&i.value());
        visitor->endVisit(&i.value());
    }
    for (auto i = m_signals.begin(); i != m_signals.end(); i++) {
        visitor->visit(&i.value());
        visitor->endVisit(&i.value());
    }
    for (auto i = m_valueAssignments.begin(); i != m_valueAssignments.end(); i++) {
        visitor->visit(i);
        if (i->objectValue) {
            i->objectValue->accept(visitor);
        }
        visitor->endVisit(i);
    }
    for (auto i = m_bindingAssignments.begin(); i != m_bindingAssignments.end(); i++) {
        visitor->visit(i);
        visitor->endVisit(i);
    }
    visitor->endVisit(this);
}
