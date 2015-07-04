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

#include "visitor.h"

#include "type.h"

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