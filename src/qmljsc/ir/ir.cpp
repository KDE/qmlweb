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

#include "ir.h"

using namespace QmlJSc::IR;

Type::Type()
        : m_super(0)
{
}

Type::Kind Type::kind()
{
    return m_kind;
}

const QString &Type::name()
{
    return m_name;
}

const QString &Type::javaScriptName()
{
    return m_javaScriptName;
}

Property *Type::addProperty(const QString &name)
{
    return &m_properties.insert(name, Property(name)).value();
}

Method *Type::addMethod(const QString &name)
{
    return &m_methods.insert(name, Method(name)).value();
}

Signal *Type::addSignal(const QString &name)
{
    return &m_signals.insert(name, Signal(name)).value();
}

void Type::setName(const QString &name)
{
    m_name = name;
}

void Type::setJavaScriptName(const QString &jsName)
{
    m_javaScriptName = jsName;
}

Property *Type::property(const QString &name)
{
    if (m_properties.contains(name)) {
        return &m_properties[name];
    }
    if (m_super) {
        return m_super->property(name);
    }
    return 0;
}

Method *Type::method(const QString &name)
{
    if (m_methods.contains(name)) {
        return &m_methods[name];
    }
    if (m_super) {
        return m_super->method(name);
    }
    return 0;
}

Signal *Type::signal(const QString &name)
{
    if (m_signals.contains(name)) {
        return &m_signals[name];
    }
    if (m_super) {
        return m_super->signal(name);
    }
    return 0;
}

Node *Type::member(const QString &name)
{
    if (m_properties.contains(name)) {
        return &m_properties[name];
    }
    if (m_methods.contains(name)) {
        return &m_methods[name];
    }
    if (m_signals.contains(name)) {
        return &m_signals[name];
    }
    if (m_super) {
        return m_super->member(name);
    }
    return 0;
}

void Type::accept(Visitor *visitor) {
    visitor->visit(this);
    visitChildren(visitor);
    visitor->endVisit(this);
}

void Type::visitChildren(Visitor *visitor) {
    for (auto i = m_properties.begin(); i != m_properties.end(); i++) {
        i.value().accept(visitor);
    }
    for (auto i = m_methods.begin(); i != m_methods.end(); i++) {
        i.value().accept(visitor);
    }
    for (auto i = m_signals.begin(); i != m_signals.end(); i++) {
        i.value().accept(visitor);
    }
}

Type *Type::super() {
    return m_super;
}

void Type::setSuper(Type *superType) {
    m_super = superType;
}

Class::Class()
        : Type()
{
    m_kind = Type::Kind_Class;
}

void Class::accept(Visitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}

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
        : property(0)
        , binding(0)
{
}

BindingAssignment::BindingAssignment()
        : property(property)
        , binding(binding)
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
    Type::visitChildren(visitor);

    for (auto i = m_valueAssignments.begin(); i != m_valueAssignments.end(); i++) {
        i->accept(visitor);
    }
    for (auto i = m_bindingAssignments.begin(); i != m_bindingAssignments.end(); i++) {
        i->accept(visitor);
    }
}

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

void Component::accept(Visitor *visitor) {
    visitor->visit(this);
    visitChildren(visitor);
    visitor->endVisit(this);
}

void Component::visitChildren(Visitor *visitor) {
    Object::visitChildren(visitor);

    for (auto i = m_ids.begin(); i != m_ids.end(); i++) {
        i.value()->accept(visitor);
    }
}

Method::Method()
        : returnType(0)
{
    kind = Kind_Method;
}

Method::Method(const QString &name)
        : returnType(0)
        , name(name)
{
    kind = Kind_Method;
}

Method::Method(QmlJSc::IR::Type *returnType, QString name)
        : returnType(returnType)
        , name(name)
{
    kind = Kind_Method;
}

void Method::accept(QmlJSc::IR::Visitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}

Signal::Signal()
{
    kind = Kind_Signal;
}

Signal::Signal(QString name)
        : name(name)
{
    kind = Kind_Signal;
}

void Signal::accept(QmlJSc::IR::Visitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}

Property::Property()
        : type(0)
        , objectValue(0)
        , jsValue(0)
{
    kind = Kind_Property;
}

Property::Property(const QString &name)
        : type(0)
        , name(name)
        , objectValue(0)
        , jsValue(0)
{
    kind = Kind_Property;
}

Property::Property(QmlJSc::IR::Type *type, QString name)
        : type(type)
        , name(name)
        , objectValue(0)
        , jsValue(0)
{
    kind = Kind_Property;
}

void Property::accept(QmlJSc::IR::Visitor *visitor) {
    visitor->visit(this);
    if (objectValue) {
        objectValue->accept(visitor);
    }
    visitor->endVisit(this);
}