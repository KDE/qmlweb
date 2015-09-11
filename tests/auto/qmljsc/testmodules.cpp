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

// Qt
#include <QtCore/QObject>
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QDebug>

// Qt private
#include <QtQml/private/qqmljsast_p.h>

// own
#include "../../../src/qmljsc/compiler.h"
#include "../../../src/qmljsc/moduleloading/moduleloading.h"
#include "../../../src/qmljsc/moduleloading/javascriptmoduleloader.h"
#include "../../../src/qmljsc/moduleloading/qtqmlmoduleloader.h"
#include "../../../src/qmljsc/ir/module.h"
#include "../../../src/qmljsc/ir/builtintypes.h"

class TestModules : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void loadMinimalModule();
    void loadQtQml();
    void loadModule();
    void testShortSymbolName();

};

using namespace QmlJSc;
using namespace QmlJSc::IR;
using namespace QQmlJS;

void TestModules::initTestCase()
{
    new Compiler;
    compiler->addIncludePath(":/test/");

    ModuleLoading::registerModuleLoader(&JavaScriptModuleLoader::create);
    ModuleLoading::registerModuleLoader(&QtQmlModuleLoader::create);
}

void TestModules::loadMinimalModule()
{
    const ImportDescription testImportDescription = {ImportDescription::Kind_ModuleImport, "MinimalModule", 0, 1};

    IR::Module *module = ModuleLoading::loadModule(testImportDescription);
    module->waitForLoaded();

    Type *a = module->type("A");
    Type *b = module->type("B");
    Type *c = module->type("C");
    Type *d = module->type("D");
    Type *e = module->type("E");

    QCOMPARE((int)module->loadingState(), (int)Module::Successful);
    QVERIFY(a);
    QVERIFY(b);
    QVERIFY(c);
    QVERIFY(d);
    QVERIFY(!e);

    QCOMPARE(a->name(), QStringLiteral("A"));
    QCOMPARE(b->name(), QStringLiteral("B"));
    QCOMPARE(c->name(), QStringLiteral("C"));
    QCOMPARE(d->name(), QStringLiteral("D"));

    QVERIFY(b->property("minimalProp"));
    QVERIFY(c->property("otherPropWithType"));
    QVERIFY(c->property("yaPropWithType"));
    QVERIFY(d->property("readonlyProp"));
    QVERIFY(d->property("otherPropWithType"));
    QVERIFY(!d->property("yaPropWithType"));

    IR::Property *readonlyProp = d->property("readonlyProp");
    QVERIFY(readonlyProp);
    QCOMPARE(readonlyProp->type, d);
    QCOMPARE(readonlyProp->readOnly, true);

    IR::Method *prototypeMethod = b->method("prototypeMethod");
    QVERIFY(prototypeMethod);
    QCOMPARE(prototypeMethod->name, QStringLiteral("prototypeMethod"));
    IR::Method *bPrototypeMethod = b->method("prototypeMethod");
    QVERIFY(bPrototypeMethod);
    QCOMPARE(bPrototypeMethod->parameters.size(), 0);
    IR::Method *bPrototypeMethod2 = b->method("prototypeMethod2");
    QVERIFY(bPrototypeMethod2);
    QCOMPARE(bPrototypeMethod2->parameters.size(), 1);
    QCOMPARE(bPrototypeMethod2->parameters[0], QStringLiteral("prop"));
    IR::Method *bConstructorMethod = b->method("constructorMethod");
    QVERIFY(bConstructorMethod);
    QCOMPARE(bConstructorMethod->parameters.size(), 2);
    QCOMPARE(bConstructorMethod->parameters[0], QStringLiteral("prop1"));
    QCOMPARE(bConstructorMethod->parameters[1], QStringLiteral("prop2"));

    QVERIFY(c->method("prototypeMethod"));
    QVERIFY(c->method("constructorMethod"));
    QVERIFY(d->method("prototypeMethod"));
    QVERIFY(d->method("constructorMethod"));

    IR::Signal *someSignal = d->signal("someSignal");
    QVERIFY(someSignal);
    QCOMPARE(someSignal->parameters.size(), 0);

    IR::Signal *anotherSignal = d->signal("anotherSignal");
    QVERIFY(anotherSignal);
    QCOMPARE(anotherSignal->parameters.size(), 2);
    QCOMPARE(anotherSignal->parameters[0].name, QStringLiteral("arg1"));
    QCOMPARE(anotherSignal->parameters[0].type, a);
    QCOMPARE(anotherSignal->parameters[1].name, QStringLiteral("arg2"));
    QCOMPARE(anotherSignal->parameters[1].type, a);
}

void TestModules::loadQtQml()
{
    const ImportDescription qtqmlImportDescription = {ImportDescription::Kind_ModuleImport, "QtQml", 1, 0};
    IR::Module *qtqmlModule = ModuleLoading::loadModule(qtqmlImportDescription);
    qtqmlModule->waitForLoaded();

    IR::Property *p = 0;
    IR::Method *m = 0;
    IR::Signal *s = 0;

    Type *componentClass = qtqmlModule->type("Component");
    QVERIFY(componentClass);
    QCOMPARE(qtqmlModule->typeFromJSName("QWComponent"), componentClass);
    QVERIFY(componentClass->flags() & IR::Type::IsComponent);
    QVERIFY(p = componentClass->property("progress"));
    QCOMPARE(p->type, BuiltinTypes:: realType());
    QVERIFY(p = componentClass->property("status"));
    QCOMPARE(p->type, BuiltinTypes:: enumType());
    QVERIFY(p = componentClass->property("url"));
    QCOMPARE(p->type, BuiltinTypes:: urlType());
    QVERIFY(!componentClass->property("heyo"));
    QVERIFY(!componentClass->method("heyo"));
    QVERIFY(m = componentClass->method("createObject"));
    QCOMPARE(m->parameters.count(), 2);
    QCOMPARE(m->parameters.at(0), QStringLiteral("parent"));
    QCOMPARE(m->parameters.at(1), QStringLiteral("properties"));
    QVERIFY(m = componentClass->method("errorString"));
    QCOMPARE(m->parameters.count(), 0);
    QVERIFY(m = componentClass->method("incubateObject"));
    QCOMPARE(m->parameters.count(), 3);
    QCOMPARE(m->parameters.at(0), QStringLiteral("parent"));
    QCOMPARE(m->parameters.at(1), QStringLiteral("properties"));
    QCOMPARE(m->parameters.at(2), QStringLiteral("mode"));
    Type *componentAttached = componentClass->attachedType();
    QVERIFY(componentAttached);
    QVERIFY(s = componentAttached->signal("completed"));
    QCOMPARE(s->parameters.count(), 0);
    QVERIFY(!componentAttached->property("completed"));
    QVERIFY(!componentAttached->method("completed"));
    QVERIFY(s = componentAttached->signal("destruction"));
    QCOMPARE(s->parameters.count(), 0);

    Type *qtobjectClass = qtqmlModule->type("QtObject");
    QVERIFY(qtobjectClass);
    QCOMPARE(qtqmlModule->typeFromJSName("QWObject"), qtobjectClass);
    QVERIFY(p = qtobjectClass->property("objectName"));
    QCOMPARE(p->type, BuiltinTypes:: stringType());

    Type *bindingClass = qtqmlModule->type("Binding");
    QVERIFY(bindingClass);
    QVERIFY(p = bindingClass->property("property"));
    QCOMPARE(p->type, BuiltinTypes:: stringType());
    QVERIFY(p = bindingClass->property("target"));
    QCOMPARE(p->type, qtobjectClass);
    QVERIFY(p = bindingClass->property("value"));
    QCOMPARE(p->type, BuiltinTypes:: varType());
    QVERIFY(p = bindingClass->property("when"));
    QCOMPARE(p->type, BuiltinTypes:: boolType());


    Type *connectionsClass = qtqmlModule->type("Connections");
    QVERIFY(connectionsClass);
    QVERIFY(p = connectionsClass->property("ignoreUnknownSignals"));
    QCOMPARE(p->type, BuiltinTypes:: boolType());
    QVERIFY(p = connectionsClass->property("target"));
    QCOMPARE(p->type, qtobjectClass);


    Type *timerClass = qtqmlModule->type("Timer");
    QVERIFY(timerClass);
    QVERIFY(p = timerClass->property("interval"));
    QCOMPARE(p->type, BuiltinTypes:: intType());
    QVERIFY(p = timerClass->property("repeat"));
    QCOMPARE(p->type, BuiltinTypes:: boolType());
    QVERIFY(p = timerClass->property("running"));
    QCOMPARE(p->type, BuiltinTypes:: boolType());
    QVERIFY(p = timerClass->property("triggeredOnStart"));
    QCOMPARE(p->type, BuiltinTypes:: boolType());
    QVERIFY(timerClass->method("restart"));
    QVERIFY(timerClass->method("start"));
    QVERIFY(timerClass->method("stop"));
    QVERIFY(s = timerClass->signal("triggered"));
    QCOMPARE(s->parameters.count(), 0);
}

void TestModules::loadModule()
{
    const ImportDescription qtqmlImportDescription = {ImportDescription::Kind_ModuleImport, "QtQml", 1, 0};
    const ImportDescription testImportDescription = {ImportDescription::Kind_ModuleImport, "TestModule", 0, 1};

    IR::Module *module = ModuleLoading::loadModule(testImportDescription);
    IR::Module *qtqmlModule = ModuleLoading::loadModule(qtqmlImportDescription);
    module->waitForLoaded();
    qtqmlModule->waitForLoaded();

    Type *pastry = module->type("Pastry");
    Type *cake = module->type("Cake");
    Type *pizza = module->type("Pizza");
    Type *printer = module->type("Printer");

    QVERIFY(module->loadingState() == Module::Successful);
    QVERIFY(pastry);
    QVERIFY(cake);
    QVERIFY(pizza);
    QVERIFY(!printer);

    QCOMPARE(pastry->name(), QStringLiteral("Pastry"));
    QCOMPARE(cake->name(), QStringLiteral("Cake"));
    QCOMPARE(pizza->name(), QStringLiteral("Pizza"));

    IR::Property *p = 0;
    QVERIFY(p = pastry->property("bakingTime"));
    QCOMPARE(p->type, BuiltinTypes::type("int"));
    QVERIFY(p = cake->property("containsRawEgg"));
    QCOMPARE(p->type, BuiltinTypes::type("bool"));
    QVERIFY(p = cake->property("bakingTime"));
    QCOMPARE(p->type, BuiltinTypes::type("int"));
    QVERIFY(p = pizza->property("isCalzone"));
    QCOMPARE(p->type, BuiltinTypes::type("bool"));
    QVERIFY(p = pizza->property("topping"));
    QCOMPARE(p->type, BuiltinTypes::type("var"));
    QVERIFY(!pizza->property("containsRawEgg"));

    IR::Property *baked = pizza->property("baked");
    QVERIFY(baked);
    QCOMPARE(baked->readOnly, true);

    QVERIFY(pastry->method("eat"));
    QVERIFY(pastry->method("bake"));
    QVERIFY(pizza->method("eat"));
    QVERIFY(pizza->method("bake"));

    QVERIFY(pizza->signal("bakingFinished"));
}

void TestModules::testShortSymbolName()
{
    ShortSymbolName nA('A');
    ShortSymbolName nB('B');
    ShortSymbolName nZ('Z');
    ShortSymbolName na('a');
    ShortSymbolName nb('b');
    ShortSymbolName nz('z');

    ShortSymbolName nA0("A0");
    ShortSymbolName nA1("A1");
    ShortSymbolName nA9("A9");
    ShortSymbolName nAA("AA");
    ShortSymbolName nAB("AB");
    ShortSymbolName nAZ("AZ");
    ShortSymbolName nAa("Aa");
    ShortSymbolName nAb("Ab");
    ShortSymbolName nAz("Az");

    ShortSymbolName na0("a0");
    ShortSymbolName na1("a1");
    ShortSymbolName na9("a9");
    ShortSymbolName naA("aA");
    ShortSymbolName naB("aB");
    ShortSymbolName naZ("aZ");
    ShortSymbolName naa("aa");
    ShortSymbolName nab("ab");
    ShortSymbolName naz("az");

    ShortSymbolName nZ9("Z9");
    ShortSymbolName nZZ("ZZ");
    ShortSymbolName nZz("Zz");
    ShortSymbolName nz9("z9");
    ShortSymbolName nzZ("zZ");
    ShortSymbolName nzz("zz");


    ShortSymbolName nA5dh("A5dh");
    ShortSymbolName nA5dz("A5dz");
    ShortSymbolName nA5zz("A5zz");
    ShortSymbolName nAzzz("Azzz");
    ShortSymbolName nZzzz("Zzzz");
    ShortSymbolName nzzzz("zzzz");

    QCOMPARE(static_cast<QString>(++nA), QStringLiteral("B"));
    QCOMPARE(static_cast<QString>(++nB), QStringLiteral("C"));
    QCOMPARE(static_cast<QString>(++nZ), QStringLiteral("A0"));
    QCOMPARE(static_cast<QString>(++na), QStringLiteral("b"));
    QCOMPARE(static_cast<QString>(++nb), QStringLiteral("c"));
    QCOMPARE(static_cast<QString>(++nz), QStringLiteral("a0"));

    QCOMPARE(static_cast<QString>(++nA0), QStringLiteral("A1"));
    QCOMPARE(static_cast<QString>(++nA1), QStringLiteral("A2"));
    QCOMPARE(static_cast<QString>(++nA9), QStringLiteral("AA"));
    QCOMPARE(static_cast<QString>(++nAA), QStringLiteral("AB"));
    QCOMPARE(static_cast<QString>(++nAB), QStringLiteral("AC"));
    QCOMPARE(static_cast<QString>(++nAZ), QStringLiteral("Aa"));
    QCOMPARE(static_cast<QString>(++nAa), QStringLiteral("Ab"));
    QCOMPARE(static_cast<QString>(++nAb), QStringLiteral("Ac"));
    QCOMPARE(static_cast<QString>(++nAz), QStringLiteral("B0"));

    QCOMPARE(static_cast<QString>(++na0), QStringLiteral("a1"));
    QCOMPARE(static_cast<QString>(++na1), QStringLiteral("a2"));
    QCOMPARE(static_cast<QString>(++na9), QStringLiteral("aA"));
    QCOMPARE(static_cast<QString>(++naA), QStringLiteral("aB"));
    QCOMPARE(static_cast<QString>(++naB), QStringLiteral("aC"));
    QCOMPARE(static_cast<QString>(++naZ), QStringLiteral("aa"));
    QCOMPARE(static_cast<QString>(++naa), QStringLiteral("ab"));
    QCOMPARE(static_cast<QString>(++nab), QStringLiteral("ac"));
    QCOMPARE(static_cast<QString>(++naz), QStringLiteral("b0"));

    QCOMPARE(static_cast<QString>(++nZ9), QStringLiteral("ZA"));
    QCOMPARE(static_cast<QString>(++nZZ), QStringLiteral("Za"));
    QCOMPARE(static_cast<QString>(++nZz), QStringLiteral("A00"));
    QCOMPARE(static_cast<QString>(++nz9), QStringLiteral("zA"));
    QCOMPARE(static_cast<QString>(++nzZ), QStringLiteral("za"));
    QCOMPARE(static_cast<QString>(++nzz), QStringLiteral("a00"));

    QCOMPARE(static_cast<QString>(++nA5dh), QStringLiteral("A5di"));
    QCOMPARE(static_cast<QString>(++nA5dz), QStringLiteral("A5e0"));
    QCOMPARE(static_cast<QString>(++nA5zz), QStringLiteral("A600"));
    QCOMPARE(static_cast<QString>(++nAzzz), QStringLiteral("B000"));
    QCOMPARE(static_cast<QString>(++nZzzz), QStringLiteral("A0000"));
    QCOMPARE(static_cast<QString>(++nzzzz), QStringLiteral("a0000"));
}

QTEST_MAIN(TestModules)
#include "testmodules.moc"

