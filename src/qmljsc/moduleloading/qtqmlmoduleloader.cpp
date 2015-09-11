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
#include "ir/builtintypes.h"
#include "utils/error.h"

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

    // === Classes ===

    // Component
    IR::Type *componentClass = new IR::Type("Component", "QWComponent", IR::Type::Flags(IR::Type::IsInstantiable | IR::Type::IsComponent) );
    p = componentClass->addProperty("progress");
    p->type = IR::BuiltinTypes:: realType();
    p = componentClass->addProperty("status");
    p->type = IR::BuiltinTypes:: enumType();
    p = componentClass->addProperty("url");
    p->type = IR::BuiltinTypes:: urlType();
    m = componentClass->addMethod("createObject");
    m->parameters.append("parent");
    m->parameters.append("properties");
    m = componentClass->addMethod("errorString");
    m = componentClass->addMethod("incubateObject");
    m->parameters.append("parent");
    m->parameters.append("properties");
    m->parameters.append("mode");

    IR::Type *componentAttached = new IR::Type;
    componentAttached->addSignal("completed");
    componentAttached->addSignal("destruction");
    componentClass->setAttachedType(componentAttached);
    module->addType(componentClass);

    // QtObject
    IR::Type *qtobjectClass = new IR::Type("QtObject", "QWObject", IR::Type::IsInstantiable);
    p = qtobjectClass->addProperty("objectName");
    p->type = IR::BuiltinTypes:: stringType();
    module->addType(qtobjectClass);

    // Binding
    IR::Type *bindingClass = new IR::Type("Binding", "", IR::Type::IsInstantiable);
    p = bindingClass->addProperty("property");
    p->type = IR::BuiltinTypes:: stringType();
    p = bindingClass->addProperty("target");
    p->type = qtobjectClass;
    p = bindingClass->addProperty("value");
    p->type = IR::BuiltinTypes:: varType();
    p = bindingClass->addProperty("when");
    p->type = IR::BuiltinTypes:: boolType();
    module->addType(bindingClass);

    // Connections
    IR::Type *connectionsClass = new IR::Type("Connections", "", IR::Type::IsInstantiable);
    p = connectionsClass->addProperty("ignoreUnknownSignals");
    p->type = IR::BuiltinTypes:: boolType();
    p = connectionsClass->addProperty("target");
    p->type = qtobjectClass;
    module->addType(connectionsClass);

    // Timer
    IR::Type *timerClass = new IR::Type("Timer", "", IR::Type::IsInstantiable);
    p = timerClass->addProperty("interval");
    p->type = IR::BuiltinTypes:: intType();
    p = timerClass->addProperty("repeat");
    p->type = IR::BuiltinTypes:: boolType();
    p = timerClass->addProperty("running");
    p->type = IR::BuiltinTypes:: boolType();
    p = timerClass->addProperty("triggeredOnStart");
    p->type = IR::BuiltinTypes:: boolType();
    timerClass->addSignal("triggered");
    timerClass->addMethod("restart");
    timerClass->addMethod("start");
    timerClass->addMethod("stop");
    module->addType(timerClass);

    module->setLoadingState(IR::Module::Successful);
}

