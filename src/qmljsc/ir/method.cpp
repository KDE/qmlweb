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

#include "method.h"

using namespace QmlJSc::IR;

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