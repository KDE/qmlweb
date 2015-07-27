/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
 * Copyright (C) 2015 Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

#ifndef OBJECT_H
#define OBJECT_H

#include "type.h"

namespace QQmlJS {
    namespace AST {
        class ExpressionNode;
    }
}

namespace QmlJSc {
namespace IR {

class Object;
class Component;

struct ValueAssignment {
    Property *property;
    Object *objectValue;
    QQmlJS::AST::ExpressionNode *jsValue;
};

struct BindingAssignment {
    Property *property;
    QQmlJS::AST::ExpressionNode *binding;
};

class Object : public Type
{
public:
    class Visitor {
    public:
        virtual void visit(Object *object) {}
        virtual void visit(Property *property) {}
        virtual void visit(Method *method) {}
        virtual void visit(Signal *signal) {}
        virtual void visit(ValueAssignment *valueAssignment) {}
        virtual void visit(BindingAssignment *bindingAssignment) {}

        virtual void endVisit(Object *object) {}
        virtual void endVisit(Property *property) {}
        virtual void endVisit(Method *method) {}
        virtual void endVisit(Signal *signal) {}
        virtual void endVisit(ValueAssignment *valueAssignment) {}
        virtual void endVisit(BindingAssignment *bindingAssignment) {}
    };

    Object();

    QVector<ValueAssignment> &valueAssignments();
    QVector<BindingAssignment> &bindingAssignments();

    ValueAssignment *addValueAssignment();
    BindingAssignment *addBindingAssignment();

    void accept(Visitor *visitor);

protected:
    QVector<ValueAssignment> m_valueAssignments;
    QVector<BindingAssignment> m_bindingAssignments;

    /**
     * Refers to the component that contains this object.
     * If this is a component root itself, it's a pointer to itself.
     */
    Component *m_component;

    friend class TestIR;
};

} // namespace IR
} // namespace QMLJSc

#endif // OBJECT_H

