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

#ifndef BUILTINTYPES_H
#define BUILTINTYPES_H

#include <QtCore/QString>

namespace QmlJSc {

namespace IR {

class Type;

class BuiltinTypes
{
public:
    static Type *type(const QString &name);
    static Type *typeFromJSName(const QString &name);
    static Type *boolType();
    static Type *doubleType();
    static Type *enumType();
    static Type *intType();
    static Type *listType();
    static Type *realType();
    static Type *stringType();
    static Type *urlType();
    static Type *varType();

private:
    static bool init();

    static const int builtinTypesCount = 9;

    static Type s_builtinTypes[builtinTypesCount];
};

} // namespace IR
} // namespace QMLJSc

#endif // BUILTINTYPES_H
