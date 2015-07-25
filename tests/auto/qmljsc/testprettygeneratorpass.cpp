/*
 * Copyright (C) 2015  Jan Marker <jan@jangmarker.de>
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

#include "../../../src/qmljsc/ir/ir.h"
#include "../../../src/qmljsc/compiler.h"
#include "../../../src/qmljsc/compilerpasses/prettygeneratorpass.h"

template<class TestedVisitorNodeType>
class TestNodeComponent : public QmlJSc::IR::Component
{
public:
    TestNodeComponent(TestedVisitorNodeType& testedNode)
            : m_testedNode(testedNode)
    {
    }

    virtual void accept(QmlJSc::IR::Visitor* visitor) override {
        m_testedNode.accept(visitor);
    }

private:
    TestedVisitorNodeType& m_testedNode;
};

static const QString MINIMAL_COMPONENT = QStringLiteral("QW_INHERIT(__comp, QtQml20.QtObject);\n"
                                                                "function __comp(parent) {\n"
                                                                "    QtQml20.QtObject.call(this, parent);\n"
                                                                "    var __ = new QWContext(this);\n"
                                                                "}\n"
                                                                "\n"
                                                                "__comp;");

class TestPrettyGeneratorPass : public QObject
{
    Q_OBJECT

private:
    void initBasicTypes();

    QmlJSc::PrettyGeneratorPass* m_prettyGeneratorPass = Q_NULLPTR;
    QmlJSc::SymbolTable* m_symbolTable = Q_NULLPTR;
    QString m_result;
    QmlJSc::IR::Class* m_qtObjectType = Q_NULLPTR;

private slots:
    void setResult(QString);

private slots:
    void initTestCase();
    void init();

    void emitsFinishedSignal();
    void visitComponent();

};

void TestPrettyGeneratorPass::setResult(QString result) {
    if (!m_result.isEmpty()) {
        QFAIL("finished emitted multiple times");
    } else {
        m_result = result;
    }
}

void TestPrettyGeneratorPass::initTestCase()
{
    initBasicTypes();
}

void TestPrettyGeneratorPass::initBasicTypes() {
    QmlJSc::IR::Type* stringType = new QmlJSc::IR::Type();
    stringType->setName("string");

    m_qtObjectType = new QmlJSc::IR::Class();
    m_qtObjectType->setName("QtObject");
    QmlJSc::IR::Property *p = m_qtObjectType->addProperty("objectName");
    p->type = stringType;
}

void TestPrettyGeneratorPass::init()
{
    if (m_prettyGeneratorPass) {
        disconnect(m_prettyGeneratorPass, SIGNAL(finished(QString)), this, SLOT(setResult(QString)));
        delete m_prettyGeneratorPass;
        delete m_symbolTable;
    }

    m_symbolTable = new QmlJSc::SymbolTable();
    m_prettyGeneratorPass = new QmlJSc::PrettyGeneratorPass(m_symbolTable);

    m_result.clear();
    connect(m_prettyGeneratorPass, SIGNAL(finished(QString)), this, SLOT(setResult(QString)));
}

void TestPrettyGeneratorPass::emitsFinishedSignal() {
    // Setup
    QmlJSc::IR::Component component;
    component.setSuper(m_qtObjectType);
    TestNodeComponent<QmlJSc::IR::Component> testNodeComponent(component);

    QSignalSpy finishedSpy(m_prettyGeneratorPass, SIGNAL(finished(QString)));

    // Do
    m_prettyGeneratorPass->process(&testNodeComponent);

    // Verify
    QCOMPARE(finishedSpy.count(), 1);
}

void TestPrettyGeneratorPass::visitComponent()
{
    // Setup
    QmlJSc::IR::Component component;
    component.setSuper(m_qtObjectType);

    TestNodeComponent<QmlJSc::IR::Component> testNodeComponent(component);

    // Do
    m_prettyGeneratorPass->process(&testNodeComponent);

    // Verify
    QCOMPARE(m_result, MINIMAL_COMPONENT);
}

QTEST_MAIN(TestPrettyGeneratorPass)
#include "testprettygeneratorpass.moc"
