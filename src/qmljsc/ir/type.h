/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
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

#ifndef TYPE_H
#define TYPE_H

#include "symbol.h"

namespace QQmlJS {
    namespace AST {
        class Node;
    }
}

namespace QmlJSc {
namespace IR {

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
    enum Kind {
        Kind_BasicType,
        Kind_List,
        Kind_Class,
        Kind_Component,
        Kind_Object,
    };

    Type();

    Kind kind();
    const QString &name();
    Property *property(const QString &name);
    Method *method(const QString &name);
    Signal *signal(const QString &name);
    Symbol *member(const QString &name);

    Property *addProperty(const QString &name);
    Method *addMethod(const QString &name);
    Signal *addSignal(const QString &name);

    void setName(const QString &name);

protected:
    Kind m_kind;

protected:
    QString m_name;
    QHash<QString, Property> m_properties;
    QHash<QString, Method> m_methods;
    QHash<QString, Signal> m_signals;

    /**
     * pointer to the super class or in case of objects the class of the object
     */
    Type *m_super;


    friend class TestIR;
};

} // namespace IR
} // namespace QMLJSc

#endif // TYPE_H
