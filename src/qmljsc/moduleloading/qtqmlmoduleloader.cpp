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
#include "qtqmlmoduleloader.h"
#include "ir/typesystem.h"
#include "ir/module.h"
#include "error.h"

using namespace QmlJSc;

QtQmlModuleLoader *QtQmlModuleLoader::create(IR::Module *module)
{
    return new QtQmlModuleLoader(module);
}

QtQmlModuleLoader::QtQmlModuleLoader(IR::Module *module)
    : AbstractModuleLoader(module)
{}

bool QmlJSc::QtQmlModuleLoader::canLoad()
{
    return module()->importDescription().name == "QtQml";
}

void QtQmlModuleLoader::doLoad()
{
    IR::Module *module = AbstractModuleLoader::module();

    IR::Property *p = 0;
    IR::Method *m = 0;
    IR::Signal *s = 0;

    // Basic types
    IR::Type *boolType = new IR::Type;
    boolType->setName("bool");
    boolType->setJavaScriptName("Boolean");
    module->addType(boolType);

    IR::Type *doubleType = new IR::Type;
    doubleType->setName("double");
    doubleType->setJavaScriptName("QWDouble");
    module->addType(doubleType);

    IR::Type *enumType = new IR::Type;
    enumType->setName("enum");
    enumType->setJavaScriptName("QWEnum");
    module->addType(enumType);

    IR::Type *intType = new IR::Type;
    intType->setName("int");
    intType->setJavaScriptName("QWInt");
    module->addType(intType);

    IR::Type *listType = new IR::Type;
    listType->setName("list");
    listType->setJavaScriptName("QWList");
    // TODO: Add list-flag (Requires T504)
    module->addType(listType);

    IR::Type *realType = new IR::Type;
    realType->setName("real");
    realType->setJavaScriptName("QWReal");
    module->addType(realType);

    IR::Type *stringType = new IR::Type;
    stringType->setName("string");
    stringType->setJavaScriptName("String");
    module->addType(stringType);

    IR::Type *urlType = new IR::Type;
    urlType->setName("url");
    urlType->setJavaScriptName("QWUrl");
    module->addType(urlType);

    IR::Type *varType = new IR::Type;
    varType->setName("var");
    varType->setJavaScriptName("QWVar");
    module->addType(varType);

    // === Classes ===

    // Component
    IR::Type *componentClass = new IR::Type( IR::Type::Flags(IR::Type::IsInstantiable | IR::Type::IsComponent) );
    componentClass->setName("Component");
    componentClass->setJavaScriptName("QWComponent");
    p = componentClass->addProperty("progress");
    p->type = realType;
    p = componentClass->addProperty("status");
    p->type = enumType;
    p = componentClass->addProperty("url");
    p->type = urlType;
    m = componentClass->addMethod("createObject");
    m->parameters.append("parent");
    m->parameters.append("properties");
    m = componentClass->addMethod("errorString");
    m = componentClass->addMethod("incubateObject");
    m->parameters.append("parent");
    m->parameters.append("properties");
    m->parameters.append("mode");

    IR::Type *componentAttached = new IR::Type(IR::Type::IsInstantiable);
    componentAttached->addSignal("completed");
    componentAttached->addSignal("destruction");
    componentClass->setAttachedType(componentAttached);
    module->addType(componentClass);

    // QtObject
    IR::Type *qtobjectClass = new IR::Type(IR::Type::IsInstantiable);
    qtobjectClass->setName("QtObject");
    qtobjectClass->setJavaScriptName("QWObject");
    p = qtobjectClass->addProperty("objectName");
    p->type = stringType;
    module->addType(qtobjectClass);

    // Binding
    IR::Type *bindingClass = new IR::Type(IR::Type::IsInstantiable);
    bindingClass->setName("Binding");
    p = bindingClass->addProperty("property");
    p->type = stringType;
    p = bindingClass->addProperty("target");
    p->type = qtobjectClass;
    p = bindingClass->addProperty("value");
    p->type = varType;
    p = bindingClass->addProperty("when");
    p->type = boolType;
    module->addType(bindingClass);

    // Connections
    IR::Type *connectionsClass = new IR::Type(IR::Type::IsInstantiable);
    connectionsClass->setName("Connections");
    p = connectionsClass->addProperty("ignoreUnknownSignals");
    p->type = boolType;
    p = connectionsClass->addProperty("target");
    p->type = qtobjectClass;
    module->addType(connectionsClass);

    // Timer
    IR::Type *timerClass = new IR::Type(IR::Type::IsInstantiable);
    timerClass->setName("Timer");
    p = timerClass->addProperty("interval");
    p->type = intType;
    p = timerClass->addProperty("repeat");
    p->type = boolType;
    p = timerClass->addProperty("running");
    p->type = boolType;
    p = timerClass->addProperty("triggeredOnStart");
    p->type = boolType;
    timerClass->addSignal("triggered");
    timerClass->addMethod("restart");
    timerClass->addMethod("start");
    timerClass->addMethod("stop");
    module->addType(timerClass);

    module->setLoadingState(IR::Module::Successful);
}

