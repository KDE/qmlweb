/*
 * Copyright (C) 2015  Jan Marker <jan@jangmarker.de>
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
#include "object.h"

#include "property.h"

using namespace QmlJSc::IR;

Property::Property()
        : type(0)
        , objectValue(0)
        , jsValue(0)
{
    QmlJSc::IR::Node::kind = QmlJSc::IR::Node::Kind_Property;
}

Property::Property(const QString &name)
        : type(0)
        , name(name)
        , objectValue(0)
        , jsValue(0)
{
    QmlJSc::IR::Node::kind = QmlJSc::IR::Node::Kind_Property;
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