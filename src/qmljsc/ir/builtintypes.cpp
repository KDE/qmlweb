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

// Own
#include "builtintypes.h"
#include "ir/typesystem.h"

using namespace QmlJSc::IR;

Type BuiltinTypes::s_builtinTypes[] =
{
    {"bool", "Boolean"},
    {"double", "QWDouble"},
    {"enum", "QWEnum"},
    {"int", "QWInt"},
    {"list", "QWList", Type::IsList},
    {"real", "QWReal"},
    {"string", "String"},
    {"url", "QWUrl"},
    {"var", "QWVar"}
};

Type *BuiltinTypes::type(const QString &name)
{
    for (int i = 0; i < builtinTypesCount; i++) {
        if (s_builtinTypes[i].name() == name) {
            return &s_builtinTypes[i];
        }
    }
    return 0;
}

Type *BuiltinTypes::typeFromJSName(const QString &jsName)
{
    for (int i = 0; i < builtinTypesCount; i++) {
        if (s_builtinTypes[i].javaScriptName() == jsName) {
            return &s_builtinTypes[i];
        }
    }
    return 0;
}

Type *BuiltinTypes::boolType()
{
    return &s_builtinTypes[0];
}

Type *BuiltinTypes::doubleType()
{
    return &s_builtinTypes[1];
}

Type *BuiltinTypes::enumType()
{
    return &s_builtinTypes[2];
}

Type *BuiltinTypes::intType()
{
    return &s_builtinTypes[3];
}

Type *BuiltinTypes::listType()
{
    return &s_builtinTypes[4];
}

Type *BuiltinTypes::realType()
{
    return &s_builtinTypes[5];
}

Type *BuiltinTypes::stringType()
{
    return &s_builtinTypes[6];
}

Type *BuiltinTypes::urlType()
{
    return &s_builtinTypes[7];
}

Type *BuiltinTypes::varType()
{
    return &s_builtinTypes[8];
}

