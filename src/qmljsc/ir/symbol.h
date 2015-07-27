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

#ifndef IR_SYMBOL_H
#define IR_SYMBOL_H

#include <QHash>
#include <QVector>
#include <QString>

namespace QQmlJS {
    namespace AST {
        class ExpressionNode;
    }
}

namespace QmlJSc {
namespace IR {

class Type;
class Object;

struct Symbol {
    enum Kind {
        Kind_Invalid,
        Kind_Property,
        Kind_Method,
        Kind_Signal
    };

    template<typename T> T *as()
    {
        if (T::kind == kind) {
            return reinterpret_cast<T>(this);
        }
        return 0;
    }
    Kind kind;
};

struct Property : public Symbol {
    Property();
    Property(const QString &name);
    Property(Type *type, QString name);

    Type *type;
    QString name;

    Object *objectValue;
    QQmlJS::AST::ExpressionNode *jsValue;
    bool readOnly :1;
    bool constant :1;
    bool dummy :6;
};

struct Parameter {
    Type *type;
    QString name;
};

struct Method : public Symbol {
    Method();
    Method(const QString &name);
    Method(Type *returnType, QString name);

    Type *returnType;
    QString name;
};

struct Signal : public Symbol {
    Signal();
    Signal(QString name);
    QString name;
    QVector<Parameter> parameters;
};

} // namespace IR
} // namespace QMLJSc

#endif // IR_SYMBOL_H
