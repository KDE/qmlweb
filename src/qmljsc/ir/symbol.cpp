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

#include "symbol.h"

using namespace QmlJSc::IR;

Property::Property()
    : type(0)
    , objectValue(0)
    , jsValue(0)
{
    kind = Symbol::Kind_Property;
}

Property::Property(const QString &name)
    : type(0)
    , name(name)
    , objectValue(0)
    , jsValue(0)
{
    kind = Symbol::Kind_Property;
}

Property::Property(Type *type, QString name)
    : type(type)
    , name(name)
    , objectValue(0)
    , jsValue(0)
{
    kind = Symbol::Kind_Property;
}

void Property::accept(Visitor *visitor) {
    visitor->visit(this);
    if (objectValue) {
        objectValue->accept(visitor);
    }
    visitor->endVisit(this);
}

Method::Method()
    : returnType(0)
{
    kind = Symbol::Kind_Method;
}

Method::Method(const QString &name)
    : returnType(0)
    , name(name)
{
    kind = Symbol::Kind_Method;
}

Method::Method(Type *returnType, QString name)
    : returnType(returnType)
    , name(name)
{
    kind = Symbol::Kind_Method;
}

void Method::accept(Visitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}

Signal::Signal()
{
    kind = Symbol::Kind_Signal;
}

Signal::Signal(QString name)
    : name(name)
{
    kind = Symbol::Kind_Signal;
}

void Signal::accept(Visitor *visitor) {
    visitor->visit(this);
    visitor->endVisit(this);
}
