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

#include <QtCore/QObject>
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>

#include <QtCore/QDebug>

#include "../../../src/qmljsc/compiler.h"
#include "../../../src/qmljsc/ir/objecttree.h"
#include "../../../src/qmljsc/ir/visitor.h"
#include "../../../src/qmljsc/ir/typesystem.h"

// Qt private
#include <private/qqmljsast_p.h>

namespace QmlJSc {
namespace IR {

class TestIR : public QObject
{
    Q_OBJECT

public:
    TestIR();

private slots:
    void initTestCase();
    void testBasics();
    void testAdd();
    void testVisitorAPI();

private:
    Type city;
    Object christiania;
    Object copenhagen;
    Type state;
    Type democracy;
    Object ottomanEmpire;
    Object denmark;

    QString christianiaName;
    QString copenhagenName;
    QString ottomanEmpireName;
    QString ottomanEmpireLanguage;
    QString denmarkName;
    QString denmarkLanguage;
    QString denmarkRParty;

    QStringRef christianiaNameRef;
    QStringRef copenhagenNameRef;
    QStringRef ottomanEmpireNameRef;
    QStringRef ottomanEmpireLangRef;
    QStringRef denmarkNameRef;
    QStringRef denmarkLangRef;
    QStringRef denmarkRPartyRef;

    QQmlJS::AST::StringLiteral christianiaNameNode;
    QQmlJS::AST::StringLiteral copenhagenNameNode;
    QQmlJS::AST::StringLiteral ottomanEmpireNameNode;
    QQmlJS::AST::StringLiteral ottomanEmpireLangNode;
    QQmlJS::AST::StringLiteral denmarkNameNode;
    QQmlJS::AST::StringLiteral denmarkLangNode;
    QQmlJS::AST::StringLiteral denmarkRPartyNode;
    QQmlJS::AST::NumericLiteral christianiaPostCodeNode;

};

TestIR::TestIR()
    : QObject()
    , christianiaName(QStringLiteral("Fristad Christiania"))
    , copenhagenName(QStringLiteral("København"))
    , ottomanEmpireName(QStringLiteral("Osmanlı İmparatorluğu"))
    , ottomanEmpireLanguage(QStringLiteral("Ottoman Turkish"))
    , denmarkName(QStringLiteral("Danmark"))
    , denmarkLanguage(QStringLiteral("Danish"))
    , denmarkRParty(QStringLiteral("S-RV"))

    , christianiaNameRef(&christianiaName)
    , copenhagenNameRef(&copenhagenName)
    , ottomanEmpireNameRef(&ottomanEmpireName)
    , ottomanEmpireLangRef(&ottomanEmpireLanguage)
    , denmarkNameRef(&denmarkName)
    , denmarkLangRef(&denmarkLanguage)
    , denmarkRPartyRef(&denmarkRParty)

    , christianiaNameNode(christianiaNameRef)
    , copenhagenNameNode(copenhagenNameRef)
    , ottomanEmpireNameNode(ottomanEmpireNameRef)
    , ottomanEmpireLangNode(ottomanEmpireLangRef)
    , denmarkNameNode(denmarkNameRef)
    , denmarkLangNode(denmarkLangRef)
    , denmarkRPartyNode(denmarkRPartyRef)
    , christianiaPostCodeNode(1050)
{
}


void TestIR::initTestCase()
{
    city.m_name = "City";
    city.m_properties = {
        {"name", {0,"name"}},
        {"postCode", {0,"postCode"}}
    };
    city.m_methods = {
        {"visit", {0, "visit"}},
    };

    state.m_name = "State";
    state.m_properties = {
        {"name", {0,"name"}},
        {"language", {0,"language"}},
        {"capital", {&city, "capital"}}
    };
    state.m_methods = {
        {"visit", {0, "visit"}},
    };
    Signal isOffensive("warStarted");
    isOffensive.parameters.append({0, "isOffensive"});
    state.m_signals = {
        {"warStarted", isOffensive},
    };

    democracy.m_name = "Democracy";
    democracy.m_super = &state;
    democracy.m_properties = {
        {"reigningParty", {0,"reigningParty"}},
    };
    democracy.m_methods = {
        {"elect", {0, "elect"}},
    };
    democracy.m_signals = {
        {"lawAdopted", {"lawAdopted"}},
    };

    copenhagen.m_super = &city;
    christiania.m_super = &city;

    ottomanEmpire.m_name = "OttomanEmpire";
    ottomanEmpire.m_super = &state;
    ottomanEmpire.m_methods = {
        {"capital", {0, "capital"}}, // Capital for year, overrides property capital.
    };
    ottomanEmpire.m_valueAssignments = {
        {&state.m_properties["name"], 0, &ottomanEmpireNameNode},
        {&state.m_properties["language"], 0, &ottomanEmpireLangNode}
    };

    denmark.m_name = "Denmark";
    denmark.m_super = &democracy;
}

void TestIR::testBasics()
{
    QCOMPARE(city.name(), QStringLiteral("City"));
    QVERIFY(city.property("name"));
    QCOMPARE(city.property("name")->name, QStringLiteral("name"));
    QVERIFY(city.property("postCode"));
    QVERIFY(city.property("postCode") == city.property("postCode")); // Check that no copying happens
    QVERIFY(!city.property("capital"));
    QVERIFY(!city.property("visit"));
    QVERIFY(city.method("visit"));
    QCOMPARE(city.method("visit")->name, QStringLiteral("visit"));
    QVERIFY(city.method("visit") == city.method("visit")); // Check that no copying happens

    QCOMPARE(state.name(), QStringLiteral("State"));
    QVERIFY(state.property("name"));
    QCOMPARE(state.property("name")->name, QStringLiteral("name"));
    QVERIFY(state.property("language"));
    QVERIFY(state.property("capital"));
    QCOMPARE(state.property("capital")->type, &city);
    QCOMPARE(state.property("capital")->type->name(), QStringLiteral("City"));
    QVERIFY(!state.property("reigningParty"));
    QVERIFY(!state.property("postCode"));
    QVERIFY(state.method("visit"));
    QVERIFY(state.method("visit") != city.method("visit"));
    QVERIFY(state.signal("warStarted"));
    QVERIFY(state.signal("warStarted") == state.signal("warStarted")); // Check that no copying happens
    QCOMPARE(state.signal("warStarted")->name, QStringLiteral("warStarted"));
    QCOMPARE(state.signal("warStarted")->parameters[0].name, QStringLiteral("isOffensive"));

    QCOMPARE(democracy.name(), QStringLiteral("Democracy"));
    QVERIFY(democracy.property("name"));
    QCOMPARE(democracy.property("name")->name, QStringLiteral("name"));
    QVERIFY(democracy.property("language"));
    QVERIFY(democracy.property("capital"));
    QCOMPARE(democracy.property("capital")->type, &city);
    QCOMPARE(democracy.property("capital")->type->name(), QStringLiteral("City"));
    QVERIFY(democracy.property("reigningParty"));
    QVERIFY(!democracy.property("postCode"));

    QCOMPARE(ottomanEmpire.name(), QStringLiteral("OttomanEmpire"));
    QVERIFY(ottomanEmpire.property("name"));
    QVERIFY(!ottomanEmpire.property("reigningParty"));
    QVERIFY(!ottomanEmpire.property("postCode"));
    QCOMPARE(ottomanEmpire.valueAssignments().count(), 2);
    QCOMPARE(ottomanEmpire.valueAssignments()[0].property, &state.m_properties["name"]);
    QCOMPARE(reinterpret_cast<QQmlJS::AST::StringLiteral*>(
        ottomanEmpire.valueAssignments()[0].jsValue)->value.toString(),
        QStringLiteral("Osmanlı İmparatorluğu"));

}

void TestIR::testAdd()
{
    ValueAssignment *denmarkNameAssignment = denmark.addValueAssignment();
    denmarkNameAssignment->property = &state.m_properties["name"];
    denmarkNameAssignment->jsValue = &denmarkNameNode;
    ValueAssignment *denmarkLanguageAssignment = denmark.addValueAssignment();
    denmarkLanguageAssignment->property = &state.m_properties["language"];
    denmarkLanguageAssignment->jsValue = &denmarkLangNode;
    ValueAssignment *denmarkCapitalAssignment = denmark.addValueAssignment();
    denmarkCapitalAssignment->property = &state.m_properties["capital"];
    denmarkCapitalAssignment->objectValue = &copenhagen;
    ValueAssignment *denmarkReigningPartyAssignment = denmark.addValueAssignment();
    denmarkReigningPartyAssignment->property = &democracy.m_properties["reigningParty"];
    denmarkReigningPartyAssignment->jsValue = &denmarkRPartyNode;

    Property *christianiaProperty = copenhagen.addProperty("christiania");
    christianiaProperty->type = &city;
    ValueAssignment *christianiaAssignment = copenhagen.addValueAssignment();
    christianiaAssignment->objectValue = &christiania;
    ValueAssignment *copenhagenNameAssignment = copenhagen.addValueAssignment();
    copenhagenNameAssignment->property = &city.m_properties["name"];
    copenhagenNameAssignment->jsValue = &copenhagenNameNode;

    Method *buyWeed = christiania.addMethod("buyWeed");
    Signal *policeRaid = christiania.addSignal("policeRaid");
    ValueAssignment *christianiaNameAssignment = christiania.addValueAssignment();
    christianiaNameAssignment->property = &city.m_properties["name"];
    christianiaNameAssignment->jsValue = &christianiaNameNode;
    ValueAssignment *christianiaPostCodeAssignment = christiania.addValueAssignment();
    christianiaPostCodeAssignment->property = &city.m_properties["postCode"];
    christianiaPostCodeAssignment->jsValue = &christianiaPostCodeNode;


    QVERIFY(denmark.property("capital"));
    QVERIFY(denmark.property("reigningParty"));
    QVERIFY(!denmark.property("postCode"));
    QCOMPARE(denmark.valueAssignments().count(), 4);
    QVERIFY(denmark.valueAssignments()[2].property);
    QVERIFY(denmark.valueAssignments()[2].property == denmark.valueAssignments()[2].property);
    QCOMPARE(denmark.valueAssignments()[3].property, &democracy.m_properties["reigningParty"]);
    QCOMPARE(reinterpret_cast<QQmlJS::AST::StringLiteral*>(
        denmark.valueAssignments()[3].jsValue)->value.toString(),
        QStringLiteral("S-RV"));

    QVERIFY(copenhagen.property("name"));
    QVERIFY(copenhagen.property("christiania"));
    QCOMPARE(copenhagen.valueAssignments().count(), 2);

    QVERIFY(christiania.method("visit"));
    QVERIFY(christiania.method("buyWeed"));
    QCOMPARE(christiania.method("buyWeed")->name, QStringLiteral("buyWeed"));
    QVERIFY(christiania.method("buyWeed") == buyWeed); // Check that no copying happens
    QVERIFY(christiania.signal("policeRaid"));
    QCOMPARE(christiania.signal("policeRaid")->name, QStringLiteral("policeRaid"));
    QVERIFY(christiania.signal("policeRaid") == policeRaid); // Check that no copying happens
}

class TestVisitor : public Visitor
{
public:
    TestVisitor()
        : Visitor()
        , objectsVisited(0)
        , propertiesVisited(0)
        , methodsVisited(0)
        , signalsVisited(0)
        , currentDepth(0)
        , valueAssignmentsVisited(0)
        , bindingAssignmentsVisited(0)
        , lastValueAssigned(0)
    {}

    virtual void visit(Object *object)
    {
        currentDepth++;
        objectsVisited++;
    }
    virtual void visit(ValueAssignment *valueAssignment)
    {
        currentDepth++;
        valueAssignmentsVisited++;
        lastValueAssigned = valueAssignment->jsValue;
    }
    virtual void visit(BindingAssignment *bindingAssignment)
    {
        currentDepth++;
        bindingAssignmentsVisited++;
    }

    virtual void endVisit(Object *object)
    {
        currentDepth--;
    }
    virtual void endVisit(ValueAssignment *valueAssignment)
    {
        currentDepth--;
    }
    virtual void endVisit(BindingAssignment *bindingAssignment)
    {
        currentDepth--;
    }

    int objectsVisited;
    int propertiesVisited;
    int methodsVisited;
    int signalsVisited;
    int valueAssignmentsVisited;
    int bindingAssignmentsVisited;
    int currentDepth;
    QString lastPropertyVisited;
    QQmlJS::AST::ExpressionNode *lastValueAssigned;
};

void TestIR::testVisitorAPI()
{
    QSKIP("Needs propertydef to be added to succeed.");
    TestVisitor visitor;
    copenhagen.accept(&visitor);

    QCOMPARE(visitor.currentDepth, 0);
    QCOMPARE(visitor.objectsVisited, 2);
    QCOMPARE(visitor.propertiesVisited, 1);
    QCOMPARE(visitor.methodsVisited, 1);
    QCOMPARE(visitor.signalsVisited, 1);
    QCOMPARE(visitor.valueAssignmentsVisited, 3);
    QCOMPARE(visitor.bindingAssignmentsVisited, 0);
    QCOMPARE(visitor.lastPropertyVisited, QStringLiteral("christiania"));
    QCOMPARE(visitor.lastValueAssigned->kind, (int)QQmlJS::AST::Node::Kind_StringLiteral);
    QQmlJS::AST::StringLiteral* lastValueAssigned = QQmlJS::AST::cast<QQmlJS::AST::StringLiteral*>(visitor.lastValueAssigned);
    if (lastValueAssigned)
        QCOMPARE(lastValueAssigned->value.toString(), QStringLiteral("København"));
}

} // namespace IR
} // namespace QMLJSc

QTEST_MAIN(QmlJSc::IR::TestIR)
#include "testir.moc"


