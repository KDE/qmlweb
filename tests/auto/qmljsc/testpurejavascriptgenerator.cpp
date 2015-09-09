/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
 * Copyright (C) 2015 Jan Marker <jan@jangmarker.de>
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

#include <QtTest/QTest>

#include <private/qqmljsast_p.h>

#include "../../../src/qmljsc/error.h"
#include "../../../src/qmljsc/purejavascriptgenerator.h"

#define TEST_SOMEVISIT_GENERATES_FROM(testSituation, expectedResult, visitType, className, ...) \
    void test_ ## visitType ## _ ## className ## _generatesCorrectCode_ ## testSituation() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        m_generator->visitType(&classInstance); \
        QCOMPARE(static_cast<PureJavaScriptGenerator*>(m_generator)->getGeneratedCode(), QStringLiteral(expectedResult)); \
    }

#define TEST_VISIT_GENERATES(testSituation, expectedResult, className, ...) \
            TEST_SOMEVISIT_GENERATES_FROM(testSituation, expectedResult, visit, className, __VA_ARGS__)

#define TEST_ENDVISIT_GENERATES(testSituation, expectedResult, className, ...) \
            TEST_SOMEVISIT_GENERATES_FROM(testSituation, expectedResult, endVisit, className, __VA_ARGS__)

#define TEST_VISIT_RETURNS(testSituation, expectedReturnResult, className, ...) \
    void test_visit ## _ ## className ## _returns_ ## expectedReturnResult ## _ ## testSituation() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        QCOMPARE(m_generator->visit(&classInstance), expectedReturnResult); \
    }

#define TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(className, ...) \
    TEST_VISIT_RETURNS(defaultImplementation, true, className, __VA_ARGS__) \
    TEST_VISIT_GENERATES(defaultImplementation, "", className, __VA_ARGS__)

#define TEST_VISIT_PUTS_ON_STACK(testScenario, expectedTopOfStack, className, ...) \
    void test_visit_## className ## _putsOnStack_ ## testScenario() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        m_generator->visit(&classInstance); \
        int stackElementCount = asPureJSGen(m_generator)->m_outputStack.count(); \
        QCOMPARE(stackElementCount, 1); \
        QString stringOnStack = asPureJSGen(m_generator)->m_outputStack.top(); \
        QCOMPARE(stringOnStack, QStringLiteral(expectedTopOfStack)); \
    }

#define TEST_ENDVISIT_FROM_STACK(testScenario, action, expectedResult, expectedStackSize, stackContent, className, ...) \
    void test_endVisit_ ## className ## _ ## action ## _ ## testScenario() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        asPureJSGen(m_generator)->m_outputStack.append(QVector<QString>stackContent); \
        m_generator->endVisit(&classInstance); \
        int stackElementCount = asPureJSGen(m_generator)->m_outputStack.count(); \
        QCOMPARE(stackElementCount, expectedStackSize); \
        QCOMPARE(asPureJSGen(m_generator)->getGeneratedCode(), QStringLiteral(expectedResult)); \
    }

#define TEST_ENDVISIT_GENERATES_FROM_STACK(testScenario, expectedResult, stackContent, className, ...) \
    TEST_ENDVISIT_FROM_STACK(testScenario, generatesFromStack, expectedResult, 0, stackContent, className, __VA_ARGS__)

#define TEST_ENDVISIT_ONLY_UPDATES_STACK(testScenario, expectedTopOfStack, stackContent, className, ...) \
    void test_endVisit_ ## className ## _updatesStack_ ## testScenario() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        asPureJSGen(m_generator)->m_outputStack.append(QVector<QString>stackContent); \
        m_generator->endVisit(&classInstance); \
        QString stringOnStack = asPureJSGen(m_generator)->m_outputStack.top(); \
        QCOMPARE(stringOnStack, QStringLiteral(expectedTopOfStack)); \
        QCOMPARE(asPureJSGen(m_generator)->getGeneratedCode(), QStringLiteral("")); \
    }

#define TEST_ENDVISIT_DOES_NOTHING(testScenario, className, ...) \
    void test_endVisit_ ## className ## _doesNothing_ ## testScenario() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        m_generator->endVisit(&classInstance); \
        QCOMPARE(asPureJSGen(m_generator)->m_outputStack.size(), 0); \
        QCOMPARE(asPureJSGen(m_generator)->getGeneratedCode(), QStringLiteral("")); \
    }

class TestPureJavaScriptGenerator
    : public QObject
{
    Q_OBJECT

private:
    PureJavaScriptGenerator* asPureJSGen(QQmlJS::AST::Visitor* generator) {
        return static_cast<PureJavaScriptGenerator*>(generator);
    }
    QQmlJS::AST::Visitor *m_generator;

    const QString m_someLabel = QString("ALabel");
    const QStringRef m_someLabelStringRef = QStringRef(&m_someLabel);
    const QString m_someString = QString("some string");
    const QStringRef m_someStringStringRef = QStringRef(&m_someString);
    const QString m_someIdentifier = QString("i");
    const QStringRef m_someIdentifierStringRef = QStringRef(&m_someIdentifier);
    QQmlJS::AST::TrueLiteral m_trueExpression;
    QQmlJS::AST::FalseLiteral m_falseExpression;

private slots:
    void init() {
        m_generator = new PureJavaScriptGenerator();
    };

    void cleanup() {
        delete m_generator;
    }

    void testGeneratedCodeIsInitiallyEmpty() {
        QCOMPARE(asPureJSGen(m_generator)->getGeneratedCode(), QStringLiteral(""));
    }

    TEST_VISIT_RETURNS(Generally, true, BinaryExpression, &m_trueExpression, QSOperator::Assign , &m_falseExpression);
    TEST_VISIT_PUTS_ON_STACK(EqualOperator, "=", BinaryExpression, &m_trueExpression, QSOperator::Assign, &m_falseExpression);
    TEST_VISIT_RETURNS(WithoutStatements, true, Block, nullptr)
    TEST_VISIT_GENERATES(WithoutStatements, "{", Block, nullptr)
    TEST_VISIT_RETURNS(WithLabel, true, BreakStatement, m_someLabelStringRef)
    TEST_VISIT_GENERATES(WithLabel, "break ALabel", BreakStatement, m_someLabelStringRef)
    TEST_VISIT_RETURNS(WithoutLabel, true, BreakStatement, nullptr)
    TEST_VISIT_GENERATES(WithoutLabel, "break", BreakStatement, nullptr)
    TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(ExpressionStatement, nullptr)
    TEST_VISIT_RETURNS(DefautScenario, true, FunctionBody, nullptr)
    TEST_VISIT_GENERATES(NoSourceElements, "{", FunctionBody, nullptr)
    TEST_VISIT_RETURNS(DefaultScenario, true, FunctionDeclaration, m_someIdentifierStringRef, nullptr, nullptr)
    TEST_VISIT_GENERATES(DefaultScenario, "function i", FunctionDeclaration, m_someIdentifierStringRef, nullptr, nullptr)
    TEST_VISIT_RETURNS(DefaultScenario, true, IdentifierExpression, m_someIdentifierStringRef)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "i", IdentifierExpression, m_someIdentifierStringRef)
    TEST_VISIT_RETURNS(DefaultScenario, true, NumericLiteral, 3.14)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "3.14", NumericLiteral, 3.14)
    TEST_VISIT_RETURNS(DefaultScenario, true, PostDecrementExpression, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "--", PostDecrementExpression, nullptr)
    TEST_VISIT_RETURNS(DefaultScenario, true, PostIncrementExpression, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "++", PostIncrementExpression, nullptr)
    TEST_VISIT_RETURNS(DefaultScenario, true, PreDecrementExpression, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "--", PreDecrementExpression, nullptr)
    TEST_VISIT_RETURNS(DefaultScenario, true, PreIncrementExpression, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "++", PreIncrementExpression, nullptr)
    TEST_VISIT_RETURNS(SomeString, true, StringLiteral, m_someStringStringRef)
    TEST_VISIT_PUTS_ON_STACK(SomeString, "some string", StringLiteral, m_someStringStringRef)
    TEST_VISIT_RETURNS(AssignmentScenario, true, VariableDeclaration, m_someIdentifierStringRef, nullptr)
    TEST_VISIT_GENERATES(AssignmentScenario, "var i=", VariableDeclaration, m_someIdentifierStringRef, &m_trueExpression)
    TEST_VISIT_GENERATES(NoAssignmentScenario, "var i", VariableDeclaration, m_someIdentifierStringRef, nullptr)
    void test_visit_VariableDeclaration_generatesCorrectCode_ConstAssignment() {
        // Prepare
        QQmlJS::AST::VariableDeclaration variableDeclaration(m_someIdentifierStringRef, &m_trueExpression);
        variableDeclaration.readOnly = true;

        // Do
        m_generator->visit(&variableDeclaration);

        // Verify
        QCOMPARE(asPureJSGen(m_generator)->getGeneratedCode(), QStringLiteral("const i="));
    }
    TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(VariableStatement, nullptr)

    TEST_ENDVISIT_ONLY_UPDATES_STACK(MoreThanTwoOperands, "2==4", ({"++", "==", "2", "4"}), BinaryExpression, nullptr, -1, nullptr)
    TEST_ENDVISIT_GENERATES_FROM_STACK(TwoOperands, "2==4", ({"==", "2", "4"}), BinaryExpression, nullptr, -1, nullptr)
    TEST_ENDVISIT_GENERATES(WithoutStatements, "}", Block, nullptr)
    TEST_ENDVISIT_GENERATES(NoExpression, ";", ExpressionStatement, nullptr)
    TEST_ENDVISIT_GENERATES(FunctionBodyClosesCorrectly, "}", FunctionBody, nullptr);
    TEST_ENDVISIT_DOES_NOTHING(DefaultScenario, FunctionDeclaration, m_someIdentifierStringRef, nullptr, nullptr);
    TEST_ENDVISIT_GENERATES_FROM_STACK(DefaultScenario, "abc", ({"abc"}), IdentifierExpression, nullptr)
    TEST_ENDVISIT_ONLY_UPDATES_STACK(DefaultScenario, "abc", ({"def", "abc"}), IdentifierExpression, nullptr)
    TEST_ENDVISIT_GENERATES_FROM_STACK(OneElement, "2.7", ({"2.7"}), NumericLiteral, 3.14)
    TEST_ENDVISIT_ONLY_UPDATES_STACK(TwoElements, "2.2", ({"2.1", "2.2"}), NumericLiteral, 3.14)
    TEST_ENDVISIT_GENERATES_FROM_STACK(OneLiteral, "2.7--", ({"--", "2.7"}), PostDecrementExpression, nullptr)
    TEST_ENDVISIT_ONLY_UPDATES_STACK(TwoLiterals, "2.7--", ({"0.0", "--", "2.7"}), PostDecrementExpression, nullptr)
    TEST_ENDVISIT_GENERATES_FROM_STACK(OneLiteral, "2.7++", ({"++", "2.7"}), PostIncrementExpression, nullptr)
    TEST_ENDVISIT_ONLY_UPDATES_STACK(TwoLiterals, "2.7++", ({"0.0", "++", "2.7"}), PostIncrementExpression, nullptr)
    TEST_ENDVISIT_GENERATES_FROM_STACK(OneLiteral, "--2.7", ({"--", "2.7"}), PreDecrementExpression, nullptr)
    TEST_ENDVISIT_ONLY_UPDATES_STACK(TwoLiterals, "--2.7", ({"0.0", "--", "2.7"}), PreDecrementExpression, nullptr)
    TEST_ENDVISIT_GENERATES_FROM_STACK(OneLiteral, "++2.7", ({"++", "2.7"}), PreIncrementExpression, nullptr)
    TEST_ENDVISIT_ONLY_UPDATES_STACK(TwoLiterals, "++2.7", ({"0.0", "++", "2.7"}), PreIncrementExpression, nullptr)
    TEST_ENDVISIT_GENERATES_FROM_STACK(OneLiteral, "another string", ({"another string"}), StringLiteral, nullptr)
    TEST_ENDVISIT_ONLY_UPDATES_STACK(TwoLiterals, "string", ({"another", "string"}), StringLiteral, nullptr)
    TEST_ENDVISIT_DOES_NOTHING(DefaultScenario, VariableDeclaration, m_someIdentifierStringRef, nullptr)
    TEST_ENDVISIT_GENERATES(NoDeclarationList, ";", VariableStatement, nullptr)

};

QTEST_MAIN(TestPureJavaScriptGenerator)
#include "testpurejavascriptgenerator.moc"
