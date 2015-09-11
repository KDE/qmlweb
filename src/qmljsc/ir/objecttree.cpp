/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
 * Copyright (C) 2015 Anton Kreuzkamp <kde-development@akreuzkamp.de>
 * Copyright (C) 2015 Jan Marker <jan@jangmarker.de>
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

#include "visitor.h"

#include "objecttree.h"

using namespace QmlJSc;
using namespace QmlJSc::IR;

Object::Object()
    : Node()
    , Type()
{
}

QVector<ValueAssignment> &Object::valueAssignments()
{
    return m_valueAssignments;
}

QVector<BindingAssignment> &Object::bindingAssignments()
{
    return m_bindingAssignments;
}

ValueAssignment::ValueAssignment()
        : property(0)
        , objectValue(0)
        , jsValue(0)
{
}

ValueAssignment::ValueAssignment(Property *property, Object *objectValue, QQmlJS::AST::ExpressionNode *jsValue)
        : property(property)
        , objectValue(objectValue)
        , jsValue(jsValue)
{
}

ValueAssignment *Object::addValueAssignment()
{
    m_valueAssignments.append(ValueAssignment());
    return &m_valueAssignments.last();
}

void ValueAssignment::accept(Visitor *visitor) {
    visitor->visit(this);
    if (objectValue) {
        objectValue->accept(visitor);
    }
    visitor->endVisit(this);
}

BindingAssignment::BindingAssignment(Property *property, QQmlJS::AST::ExpressionNode *binding)
        : property(property)
        , binding(binding)
{
}

BindingAssignment::BindingAssignment()
        : property(0)
        , binding(0)
{
}

void BindingAssignment::accept(Visitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}

BindingAssignment *Object::addBindingAssignment()
{
    m_bindingAssignments.append(BindingAssignment());
    return &m_bindingAssignments.last();
}

void Object::accept(Visitor *visitor)
{
    visitor->visit(this);
    visitChildren(visitor);
    visitor->endVisit(this);
}

void Object::visitChildren(Visitor *visitor) {
    for (auto i = m_valueAssignments.begin(); i != m_valueAssignments.end(); i++) {
        i->accept(visitor);
    }
    for (auto i = m_bindingAssignments.begin(); i != m_bindingAssignments.end(); i++) {
        i->accept(visitor);
    }
}