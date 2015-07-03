/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

#include "visitor.h"

#include "type.h"

namespace QQmlJS {
    namespace AST {
        class ExpressionNode;
    }
}

namespace QmlJSc {
namespace IR {

class Node;

class Object;
class Component;



struct ValueAssignment : public Node {

    ValueAssignment();

    ValueAssignment(Property *property, Object *objectValue, QQmlJS::AST::ExpressionNode *jsValue);

    Property *property;
    Object *objectValue;
    QQmlJS::AST::ExpressionNode *jsValue;

    virtual void accept(Visitor *visitor);
};

struct BindingAssignment : public Node {

    BindingAssignment();

    BindingAssignment(Property *property, QQmlJS::AST::ExpressionNode *binding);

    Property *property;
    QQmlJS::AST::ExpressionNode *binding;

    virtual void accept(Visitor *visitor) {
        visitor->visit(this);
        visitor->endVisit(this);
    }
};

class Object : public Type
{
public:

    Object();

    QVector<ValueAssignment> &valueAssignments();
    QVector<BindingAssignment> &bindingAssignments();

    ValueAssignment *addValueAssignment();
    BindingAssignment *addBindingAssignment();


    virtual void accept(Visitor *visitor);

protected:
    QVector<ValueAssignment> m_valueAssignments;
    QVector<BindingAssignment> m_bindingAssignments;

    virtual void visitChildren(Visitor *visitor);

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

