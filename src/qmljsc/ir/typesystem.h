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

#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H

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
class Type
{
public:
    enum Flag {
        None = 0,
        IsInstantiable = 1,
        IsComponent = 2
    };
    Q_DECLARE_FLAGS(Flags, Flag);

    Type();
    Type(Flags flags);

    const QString &name();
    const QString &javaScriptName();
    Property *property(const QString &name);
    Method *method(const QString &name);
    Signal *signal(const QString &name);

    Property *addProperty(const QString &name);
    Method *addMethod(const QString &name);
    Signal *addSignal(const QString &name);

    void setName(const QString &name);
    void setJavaScriptName(const QString &jsName);

    Type *super();
    void setSuper(Type *superType);

    void setFlags(Flags flags);
    Flags flags();

    Type *attachedType();
    void setAttachedType(Type *);

protected:
    QString m_name;
    QString m_javaScriptName;
    Flags m_flags;
    QHash<QString, Property> m_properties;
    QHash<QString, Method> m_methods;
    QHash<QString, Signal> m_signals;
    Type *m_attachedType;

    /**
     * pointer to the super class or in case of objects the class of the object
     */
    Type *m_super;


    friend class TestIR;
};

struct Parameter {
    Type *type;
    QString name;
};

class Method {
public:
    Method();
    Method(const QString &name);
    Method(Type *returnType, QString name);

    Type *returnType;
    QString name;
    QVector<QString> parameters;
};

class Signal {
public:
    Signal();
    Signal(QString name);

    QString name;
    QVector<Parameter> parameters;
};

class Property {
public:
    Property();
    Property(const QString &name);
    Property(Type *type, QString name);

    Type *type;
    QString name;

    bool readOnly :1;
    bool constant :1;
    bool dummy :6;
};

} // namespace IR
} // namespace QmlJSc

#endif // TYPESYSTEM_H

