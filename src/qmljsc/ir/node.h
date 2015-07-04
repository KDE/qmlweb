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

#ifndef QMLWEB_NODE_H
#define QMLWEB_NODE_H

namespace QmlJSc {
    namespace IR {

class Visitor;

class Node {

public:
    enum Kind {
        Kind_Invalid,
        Kind_Property,
        Kind_Method,
        Kind_Signal,
        Kind_BasicType,
        Kind_List,
        Kind_Class,
        Kind_Component,
        Kind_Object
    };

    template<typename T> T *as()
    {
        if (T::kind == kind) {
            return reinterpret_cast<T>(this);
        }
        return 0;
    }

    virtual void accept(Visitor *visitor) = 0;

    Kind kind;
};

} // namespace IR
} // namespace QmlJSc


#endif //QMLWEB_NODE_H
