/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
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

#ifndef QMLWEB_VISITOR_H
#define QMLWEB_VISITOR_H


namespace QmlJSc {
    namespace IR {

class Node;
class File;
class Object;
class ValueAssignment;
class BindingAssignment;

class Visitor {

public:
    virtual void visit(File *file) {}
    virtual void visit(Object *object) {}
    virtual void visit(ValueAssignment *valueAssignment) {}
    virtual void visit(BindingAssignment *bindingAssignment) {}

    virtual void endVisit(File *file) {}
    virtual void endVisit(Object *object) {}
    virtual void endVisit(ValueAssignment *valueAssignment) {}
    virtual void endVisit(BindingAssignment *bindingAssignment) {}
};

} // namespace IR
} // namespace QmlJSc

#endif //QMLWEB_VISITOR_H
