/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015 Anton Kreuzkamp <akreuzkamp@web.de>
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

#include "builtintypes.h"
#include "type.h"
#include "class.h"

#include <QHash>
#include <QMutex>

using namespace QmlJSc::IR;

static QHash<QString, Type *> s_builtinTypes;
static bool builtinsInitialized = false;
static QMutex loadMutex;

static void initializeBuiltins()
{
    if (builtinsInitialized) { // the mutex requires us to check twice
        return;
    }

    Type *t = new Type();
    t->setName("bool");
    t->addMethod("toString");
    t->addMethod("valueOf");
    s_builtinTypes.insert("bool", t);
    s_builtinTypes.insert("Boolean", t);

    t = new Type();
    t->setName("double");
    t->addMethod("toString");
    t->addMethod("toLocaleString");
    t->addMethod("valueOf");
    t->addMethod("toFixed");
    t->addMethod("toExponential");
    t->addMethod("toPrecision");
    s_builtinTypes.insert("double", t);
    s_builtinTypes.insert("Number", t);

    t = new Type();
    t->setName("enumeration");
    s_builtinTypes.insert("enumeration", t);

    Type *integer = new Type();
    integer->setName("int");
    integer->addMethod("toString");
    integer->addMethod("toLocaleString");
    integer->addMethod("valueOf");
    integer->addMethod("toFixed");
    integer->addMethod("toExponential");
    integer->addMethod("toPrecision");
    s_builtinTypes.insert("int", integer);
    s_builtinTypes.insert("QWInt", integer);

    t = new Type();
    t->setName("list");
    s_builtinTypes.insert("list", t);
    s_builtinTypes.insert("QWList", t);

    t = new Type();
    t->setName("real");
    t->addMethod("toString");
    t->addMethod("toLocaleString");
    t->addMethod("valueOf");
    t->addMethod("toFixed");
    t->addMethod("toExponential");
    t->addMethod("toPrecision");
    s_builtinTypes.insert("real", t);

    Type *string = new Type();
    string->setName("string");
    string->addMethod("toString");
    string->addMethod("valueOf");
    string->addMethod("charAt");
    string->addMethod("charCodeAt");
    string->addMethod("concat");
    string->addMethod("indexOf");
    string->addMethod("lastIndexOf");
    string->addMethod("localeCompare");
    string->addMethod("match");
    string->addMethod("replace");
    string->addMethod("search");
    string->addMethod("slice");
    string->addMethod("split");
    string->addMethod("sustr");
    string->addMethod("substring");
    string->addMethod("toLowerCase");
    string->addMethod("toLocaleLowerCase");
    string->addMethod("toUpperCase");
    string->addMethod("toLocaleUpperCase");
    string->addMethod("trim");
    Property *length = string->addProperty("length");
    length->simple = true;
    length->type = integer;
    s_builtinTypes.insert("string", string);
    s_builtinTypes.insert("String", string);

    t = new Type();
    t->setName("url");
    s_builtinTypes.insert("url", t);
    s_builtinTypes.insert("QWUrl", t);

    t = new Type();
    t->setName("var");
    s_builtinTypes.insert("var", t);
    s_builtinTypes.insert("QWVar", t);



    t = new Class();
    t->setName("QtObject");
    Property *p = t->addProperty("objectName");
    p->type = string;
    s_builtinTypes.insert("QtObject", t);
    s_builtinTypes.insert("QWObject", t);

    builtinsInitialized = true;
}

Type *QmlJSc::IR::getBuiltinType(const QString &name)
{
    if (!builtinsInitialized) {
        loadMutex.lock();
        initializeBuiltins();
        loadMutex.unlock();
    }
    return s_builtinTypes.value(name);
}
