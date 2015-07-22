/*
 * Copyright (C) 2015 Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

#ifndef QMLWEB_IR_H
#define QMLWEB_IR_H

#include <QString>
#include <QHash>
#include <QVector>

namespace QQmlJS {
    namespace AST {
        class ExpressionNode;
    }
}

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

class Property;
class Method;
class Signal;

/**
 * This class provides API representing Qml.js objects and types and allows to
 * learn about the type's properties, functions, etc.
 *
 * Type hereby refers to (built in) basic types, types provided by modules and
 * components as well as any objects defined in QML.
 */
class Type : public Node
{
public:
    Type();

    Kind kind();
    const QString &name();
    Property *property(const QString &name);
    Method *method(const QString &name);
    Signal *signal(const QString &name);
    Node *member(const QString &name);

    Property *addProperty(const QString &name);
    Method *addMethod(const QString &name);
    Signal *addSignal(const QString &name);

    void setName(const QString &name);

    virtual void accept(Visitor *visitor) override;

protected:
    Kind m_kind;

protected:
    QString m_name;
    QHash<QString, Property> m_properties;
    QHash<QString, Method> m_methods;
    QHash<QString, Signal> m_signals;

    virtual void visitChildren(Visitor *visitor);

    /**
     * pointer to the super class or in case of objects the class of the object
     */
    Type *m_super;


    friend class TestIR;
};

class Class : public Type
{
public:
    Class();

    virtual void accept(Visitor *visitor) override;

private:
    Type *m_attached;

    friend class TestIR;
};

class Object;

struct ValueAssignment : public Node {

    ValueAssignment();

    ValueAssignment(Property *property, Object *objectValue, QQmlJS::AST::ExpressionNode *jsValue);

    Property *property;
    Object *objectValue;
    QQmlJS::AST::ExpressionNode *jsValue;

    virtual void accept(Visitor *visitor) override;
};

struct BindingAssignment : public Node {

    BindingAssignment();

    BindingAssignment(Property *property, QQmlJS::AST::ExpressionNode *binding);

    Property *property;
    QQmlJS::AST::ExpressionNode *binding;

    virtual void accept(Visitor *visitor) override;
};

class Component;

class Object : public Type
{
public:

    Object();

    QVector<ValueAssignment> &valueAssignments();
    QVector<BindingAssignment> &bindingAssignments();

    ValueAssignment *addValueAssignment();
    BindingAssignment *addBindingAssignment();

    virtual void accept(Visitor *visitor) override;

protected:
    QVector<ValueAssignment> m_valueAssignments;
    QVector<BindingAssignment> m_bindingAssignments;

    virtual void visitChildren(Visitor *visitor) override;

    /**
     * Refers to the component that contains this object.
     * If this is a component root itself, it's a pointer to itself.
     */
    Component *m_component;

    friend class TestIR;
};

class Component : public Object
{
public:
    Component();

    Object *object(const QString &id);

    virtual void accept(Visitor *visitor) override;
    virtual void visitChildren(Visitor *visitor) override;

private:
    QHash<QString, Object*> m_ids;

    Component *m_parentComponent;

    friend class TestIR;
};

struct Parameter {
    QmlJSc::IR::Type *type;
    QString name;
};

struct Method : public QmlJSc::IR::Node {
    Method();
    Method(const QString &name);
    Method(QmlJSc::IR::Type *returnType, QString name);

    QmlJSc::IR::Type *returnType;
    QString name;

    virtual void accept(QmlJSc::IR::Visitor *visitor) override;
};

struct Signal : public QmlJSc::IR::Node {
    Signal();
    Signal(QString name);
    QString name;
    QVector<Parameter> parameters;

    virtual void accept(QmlJSc::IR::Visitor *visitor) override;
};

struct Property : public QmlJSc::IR::Node {
    Property();
    Property(const QString &name);
    Property(QmlJSc::IR::Type *type, QString name);

    QmlJSc::IR::Type *type;
    QString name;

    QmlJSc::IR::Object *objectValue;
    QQmlJS::AST::ExpressionNode *jsValue;
    bool readOnly :1;
    bool constant :1;
    bool dummy :6;

    virtual void accept(QmlJSc::IR::Visitor *visitor) override;
};

} // namespace IR
} // namespace QmlJSc

#endif //QMLWEB_IR_H
