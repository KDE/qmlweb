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

#include "typesystem.h"

using namespace QmlJSc::IR;

Type::Type()
        : m_super(0)
{
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

Type *Type::super() {
    return m_super;
}

void Type::setSuper(Type *superType) {
    m_super = superType;
}

LibraryClass::LibraryClass()
        : Type()
{
}

Method::Method()
        : returnType(0)
{
}

Method::Method(const QString &name)
        : returnType(0)
        , name(name)
{
}

Method::Method(Type *returnType, QString name)
        : returnType(returnType)
        , name(name)
{
}

Signal::Signal()
{
}

Signal::Signal(QString name)
        : name(name)
{
}

Property::Property()
        : type(0)
{
}

Property::Property(const QString &name)
        : type(0)
        , name(name)
{
}

Property::Property(Type *type, QString name)
        : type(type)
        , name(name)
{
}