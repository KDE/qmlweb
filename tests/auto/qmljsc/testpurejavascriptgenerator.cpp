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

#include "../../../src/qmljsc/utils/error.h"
#include "../../../src/qmljsc/purejavascriptgenerator.h"

#define TEST_VISIT_PUTS_ON_STACK_OBJ(testSituation, expectedStackContent, className, instancePtr) \
    void test_ ## visit ## _ ## className ## _putsOnStack_ ## testSituation() { \
        QStack<QString> expectedStack;\
        expectedStack.append(QVector<QString>expectedStackContent);\
        m_generator->visit(instancePtr); \
        int stackElementCount = asPureJSGen(m_generator)->m_outputStack.size(); \
        QCOMPARE(stackElementCount, expectedStack.size()); \
        QCOMPARE(asPureJSGen(m_generator)->m_outputStack, expectedStack); \
    }

#define TEST_SOMEVISIT_PUTS_MULTIPLE_ON_STACK(testSituation, expectedStackContent, visitType, className, ...) \
    void test_ ## visitType ## _ ## className ## _putsOnStack_ ## testSituation() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        QStack<QString> expectedStack;\
        expectedStack.append(QVector<QString>expectedStackContent);\
        m_generator->visit(&classInstance); \
        int stackElementCount = asPureJSGen(m_generator)->m_outputStack.size(); \
        QCOMPARE(stackElementCount, expectedStack.size()); \
        QCOMPARE(asPureJSGen(m_generator)->m_outputStack, expectedStack); \
    }

#define TEST_SOMEVISIT_PUTS_ON_STACK(testSituation, expectedStackContent, visitType, className, ...) \
        TEST_SOMEVISIT_PUTS_MULTIPLE_ON_STACK(testSituation, ({expectedStackContent}), visitType, className, __VA_ARGS__)

#define TEST_VISIT_PUTS_ON_STACK_WITHOUT_RETURN(testSituation, expectedResult, className, ...) \
            TEST_SOMEVISIT_PUTS_ON_STACK(testSituation, expectedResult, visit, className, __VA_ARGS__)

#define TEST_ENDVISIT_PUTS_ON_STACK(testSituation, expectedResult, className, ...) \
            TEST_SOMEVISIT_PUTS_ON_STACK(testSituation, expectedResult, endVisit, className, __VA_ARGS__)

#define TEST_VISIT_RETURNS(testSituation, expectedReturnResult, className, ...) \
    void test_visit ## _ ## className ## _returns_ ## expectedReturnResult ## _ ## testSituation() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        QCOMPARE(m_generator->visit(&classInstance), expectedReturnResult); \
    }

#define TEST_VISIT_PUTS_ON_STACK(testSituation, expectedResult, className, ...) \
            TEST_VISIT_PUTS_ON_STACK_WITHOUT_RETURN(testSituation, expectedResult, className, __VA_ARGS__) \
            TEST_VISIT_RETURNS(testSituation, true, className, __VA_ARGS__)

#define TEST_VISIT_PUTS_NOTHING_ON_STACK(testSituation, className, ...) \
    void test_ ## visitType ## _ ## className ## _putsNothingOnStack_ ## testSituation() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        m_generator->visit(&classInstance); \
        int stackElementCount = asPureJSGen(m_generator)->m_outputStack.count(); \
        QCOMPARE(stackElementCount, 0); \
    }

#define TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(className, ...) \
    TEST_VISIT_RETURNS(defaultImplementation, true, className, __VA_ARGS__) \
    TEST_VISIT_PUTS_NOTHING_ON_STACK(defaultImplementation, className, __VA_ARGS__)

#define TEST_ENDVISIT_REDUCES_STACK(testScenario, expectedTopOfStack, stackContent, className, ...) \
    void test_endVisit_ ## className ## _reducesStack_ ## testScenario() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        asPureJSGen(m_generator)->m_outputStack.append(QVector<QString>stackContent); \
        m_generator->endVisit(&classInstance); \
        int stackElementCount = asPureJSGen(m_generator)->m_outputStack.count(); \
        QCOMPARE(stackElementCount, 1); \
        QString stringOnStack = asPureJSGen(m_generator)->m_outputStack.top(); \
        QCOMPARE(stringOnStack, QStringLiteral(expectedTopOfStack)); \
    }

#define TEST_ENDVISIT_REDUCES_STACK_OBJ(testScenario, expectedTopOfStack, stackContent, className, classInstancePtr) \
    void test_endVisit_ ## className ## _reducesStack_ ## testScenario() { \
        asPureJSGen(m_generator)->m_outputStack.append(QVector<QString>stackContent); \
        m_generator->endVisit(classInstancePtr); \
        int stackElementCount = asPureJSGen(m_generator)->m_outputStack.count(); \
        QCOMPARE(stackElementCount, 1); \
        QString stringOnStack = asPureJSGen(m_generator)->m_outputStack.top(); \
        QCOMPARE(stringOnStack, QStringLiteral(expectedTopOfStack)); \
    }

#define TEST_ENDVISIT_DOES_NOTHING(testScenario, className, ...) \
    void test_endVisit_ ## className ## _doesNothing_ ## testScenario() { \
        QQmlJS::AST::className classInstance(__VA_ARGS__); \
        m_generator->endVisit(&classInstance); \
        QCOMPARE(asPureJSGen(m_generator)->m_outputStack.size(), 0); \
        QCOMPARE(asPureJSGen(m_generator)->getGeneratedCode(), QStringLiteral("")); \
    }

#define TEST_VISIT_BINARYOP_PUTS_ON_STACK(operatorEnumName, character) \
        TEST_VISIT_PUTS_ON_STACK(operatorEnumName ## Operation, character, BinaryExpression, &m_trueExpression, QSOperator::operatorEnumName, &m_falseExpression)

class TestPureJavaScriptGenerator
    : public QObject
{
    Q_OBJECT

public:
    TestPureJavaScriptGenerator()
        : m_someLabel("ALabel")
        , m_someLabelStringRef(&m_someLabel)
        , m_someString("some string")
        , m_someStringStringRef(&m_someString)
        , m_someIdentifier("i")
        , m_someIdentifierStringRef(&m_someIdentifier)
        , m_anotherIdentifier("e")
        , m_anotherIdentifierStringRef(&m_anotherIdentifier)
        , m_trueExpression()
        , m_falseExpression()
        , m_statement1()
        , m_statement2()
        , m_statement3()
        , m_threeStatementsList(&m_statement1)
        , m_statementListPart2(&m_threeStatementsList, &m_statement2)
        , m_statementListPart3(&m_statementListPart2, &m_statement3)
        , m_sourceElement1(nullptr)
        , m_sourceElement2(nullptr)
        , m_sourceElement3(nullptr)
        , m_threeSourceElementsList(&m_sourceElement1)
        , m_sourceElementsListPart2(&m_threeSourceElementsList, &m_sourceElement2)
        , m_sourceElementsListPart3(&m_sourceElementsListPart2, &m_sourceElement3)
        , m_twoParameters(m_someIdentifierStringRef)
        , m_parameterListPart2(&m_twoParameters, m_anotherIdentifierStringRef)
        , m_functionBody(nullptr)
        , m_constDeclaration1(m_someIdentifierStringRef, nullptr)
        , m_constDeclaration2(m_anotherIdentifierStringRef, nullptr)
        , m_varDeclaration1(m_someIdentifierStringRef, nullptr)
        , m_varDeclaration2(m_anotherIdentifierStringRef, nullptr)
        , m_twoConstDeclarations(&m_constDeclaration1)
        , m_twoConstDeclarationsPart2(&m_twoConstDeclarations, &m_constDeclaration2)
        , m_twoVarDeclarations(&m_varDeclaration1)
        , m_twoVarDeclarationsPart2(&m_twoVarDeclarations, &m_varDeclaration2)
    {
        m_statementListPart3.finish();
        m_sourceElementsListPart3.finish();
        m_parameterListPart2.finish();
        m_twoConstDeclarationsPart2.finish(true);
        m_twoVarDeclarationsPart2.finish(false);
    }

private:
    PureJavaScriptGenerator* asPureJSGen(QQmlJS::AST::Visitor* generator) {
        return static_cast<PureJavaScriptGenerator*>(generator);
    }
    QQmlJS::AST::Visitor *m_generator;

    const QString m_someLabel;
    const QStringRef m_someLabelStringRef;
    const QString m_someString;
    const QStringRef m_someStringStringRef;
    const QString m_someIdentifier;
    const QStringRef m_someIdentifierStringRef;
    const QString m_anotherIdentifier;
    const QStringRef m_anotherIdentifierStringRef;
    QQmlJS::AST::TrueLiteral m_trueExpression;
    QQmlJS::AST::FalseLiteral m_falseExpression;
    QQmlJS::AST::EmptyStatement m_statement1;
    QQmlJS::AST::EmptyStatement m_statement2;
    QQmlJS::AST::EmptyStatement m_statement3;
    QQmlJS::AST::StatementList m_threeStatementsList;
    QQmlJS::AST::StatementList m_statementListPart2;
    QQmlJS::AST::StatementList m_statementListPart3;
    QQmlJS::AST::StatementSourceElement m_sourceElement1;
    QQmlJS::AST::StatementSourceElement m_sourceElement2;
    QQmlJS::AST::StatementSourceElement m_sourceElement3;
    QQmlJS::AST::SourceElements m_threeSourceElementsList;
    QQmlJS::AST::SourceElements m_sourceElementsListPart2;
    QQmlJS::AST::SourceElements m_sourceElementsListPart3;
    QQmlJS::AST::FormalParameterList m_twoParameters;
    QQmlJS::AST::FormalParameterList m_parameterListPart2;
    QQmlJS::AST::FunctionBody m_functionBody;
    QQmlJS::AST::VariableDeclaration m_constDeclaration1;
    QQmlJS::AST::VariableDeclaration m_constDeclaration2;
    QQmlJS::AST::VariableDeclaration m_varDeclaration1;
    QQmlJS::AST::VariableDeclaration m_varDeclaration2;
    QQmlJS::AST::VariableDeclarationList m_twoConstDeclarations;
    QQmlJS::AST::VariableDeclarationList m_twoConstDeclarationsPart2;
    QQmlJS::AST::VariableDeclarationList m_twoVarDeclarations;
    QQmlJS::AST::VariableDeclarationList m_twoVarDeclarationsPart2;

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

    void test_getGeneratedCode_getsTopOfStack() {
        // Prepare
        asPureJSGen(m_generator)->m_outputStack << "1";

        // Verify
        QCOMPARE(asPureJSGen(m_generator)->getGeneratedCode(), QStringLiteral("1"));
    }

    void test_getGeneratedCode_throwsError_OnStackSizeGreaterThanOne() {
        // Prepare
        asPureJSGen(m_generator)->m_outputStack << "1" << "2";

        // Verify
        QVERIFY_EXCEPTION_THROWN(asPureJSGen(m_generator)->getGeneratedCode(), QmlJSc::Error);
    }

    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Assign, "=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceAdd, "+=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceSub, "-=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceMul, "*=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceDiv, "/=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceMod, "%=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceLeftShift, "<<=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceRightShift, ">>=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceURightShift, ">>>=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceAnd, "&=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceXor, "^=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InplaceOr, "|=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Add, "+")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Sub, "-")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Mul, "*")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Div, "/")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Mod, "%")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(LShift, "<<")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(RShift, ">>")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(URShift, ">>>")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(BitAnd, "&")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(BitXor, "^")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(BitOr, "|")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Equal, "==")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(NotEqual, "!=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(StrictEqual, "===")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(StrictNotEqual, "!==")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Gt, ">")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Ge, ">=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Lt, "<")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Le, "<=")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(And, "&&")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(Or, "||")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(In, " in ")
    TEST_VISIT_BINARYOP_PUTS_ON_STACK(InstanceOf, " instanceof ")
    TEST_VISIT_PUTS_ON_STACK(WithoutStatements, "{", Block, nullptr)
    TEST_VISIT_PUTS_ON_STACK(WithLabel, "break ALabel", BreakStatement, m_someLabelStringRef)
    TEST_VISIT_PUTS_ON_STACK(WithoutLabel, "break", BreakStatement, nullptr)
    //TODO TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(EmptyStatement)
    TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(ExpressionStatement, nullptr)
    TEST_VISIT_PUTS_ON_STACK_OBJ(OneParameter, ({"e"}), FormalParameterList, &m_parameterListPart2)
    TEST_VISIT_PUTS_ON_STACK_OBJ(TwoParameters, ({"i,e"}), FormalParameterList, &m_twoParameters)
    TEST_VISIT_PUTS_ON_STACK(NoSourceElements, "{", FunctionBody, nullptr)
    TEST_VISIT_PUTS_ON_STACK(WithParameters, "function i", FunctionDeclaration, m_someIdentifierStringRef, &m_twoParameters, nullptr)
    TEST_VISIT_PUTS_ON_STACK(WithoutParameters, "function i", FunctionDeclaration, m_someIdentifierStringRef, nullptr, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "i", IdentifierExpression, m_someIdentifierStringRef)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "3.14", NumericLiteral, 3.14)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "--", PostDecrementExpression, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "++", PostIncrementExpression, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "--", PreDecrementExpression, nullptr)
    TEST_VISIT_PUTS_ON_STACK(DefaultScenario, "++", PreIncrementExpression, nullptr)
    TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(SourceElements, nullptr)
    TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(StatementList, nullptr)
    TEST_VISIT_PUTS_ON_STACK(SomeString, "some string", StringLiteral, m_someStringStringRef)
    TEST_VISIT_PUTS_ON_STACK(AssignmentScenario, "i=", VariableDeclaration, m_someIdentifierStringRef, &m_trueExpression)
    TEST_VISIT_PUTS_ON_STACK(NoAssignmentScenario, "i", VariableDeclaration, m_someIdentifierStringRef, nullptr)
    TEST_VISIT_PUTS_ON_STACK(VarDeclarationList, "var", VariableDeclarationList, &m_varDeclaration1)
    TEST_VISIT_PUTS_ON_STACK(ConstDelcarationList, "const", VariableDeclarationList, &m_constDeclaration1)
    TEST_VISIT_IS_DEFAULT_IMPLEMENTATION(VariableStatement, nullptr)

    TEST_ENDVISIT_REDUCES_STACK(TwoOperands, "2==4", ({"==", "2", "4"}), BinaryExpression, nullptr, -1, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(WithoutStatements, "{content}", ({"{", "content"}), Block, nullptr)
    TEST_ENDVISIT_REDUCES_STACK_OBJ(DefaultScenario, ";", ({}), EmptyStatement, &m_statement1)
    TEST_ENDVISIT_REDUCES_STACK(NoExpression, "expression;", ({"expression"}), ExpressionStatement, nullptr)
    TEST_ENDVISIT_REDUCES_STACK_OBJ(AnyNumberOfParameters, "i", ({"i"}), FormalParameterList, &m_twoParameters) // does nothing
    TEST_ENDVISIT_REDUCES_STACK(FunctionBodyClosesCorrectly, "{func}", ({"{", "func"}), FunctionBody, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(WithoutParametersWithBody, "name(){body}", ({"name", "{body}"}),
                                                        FunctionDeclaration, m_someIdentifierStringRef, nullptr, &m_functionBody)
    TEST_ENDVISIT_REDUCES_STACK(WithParametersWithBody, "name(parameters){body}", ({"name", "parameters", "{body}"}),
                                                        FunctionDeclaration, m_someIdentifierStringRef, &m_twoParameters, &m_functionBody)
    TEST_ENDVISIT_REDUCES_STACK(WithoutBody, "name(parameters){}", ({"name", "parameters"}),
                                                        FunctionDeclaration, m_someIdentifierStringRef, &m_twoParameters, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(DefaultScenario, "abc", ({"abc"}), IdentifierExpression, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(OneElement, "2.7", ({"2.7"}), NumericLiteral, 3.14)
    TEST_ENDVISIT_REDUCES_STACK(OneLiteral, "2.7--", ({"--", "2.7"}), PostDecrementExpression, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(OneLiteral, "2.7++", ({"++", "2.7"}), PostIncrementExpression, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(OneLiteral, "--2.7", ({"--", "2.7"}), PreDecrementExpression, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(OneLiteral, "++2.7", ({"++", "2.7"}), PreIncrementExpression, nullptr)
    TEST_ENDVISIT_REDUCES_STACK_OBJ(ThreeSourceElements, "sEl1sEl2sEl3", ({"sEl1", "sEl2", "sEl3"}), SourceElements, &m_threeSourceElementsList)
    TEST_ENDVISIT_REDUCES_STACK_OBJ(ThreeStatements, "st1st2st3", ({"st1", "st2", "st3"}), StatementList, &m_threeStatementsList)
    TEST_ENDVISIT_REDUCES_STACK(OneLiteral, "another string", ({"another string"}), StringLiteral, nullptr)
    TEST_ENDVISIT_REDUCES_STACK(Assignment, "x=5", ({"x=", "5"}), VariableDeclaration, m_someIdentifierStringRef, &m_trueExpression)
    TEST_ENDVISIT_REDUCES_STACK(NoAssignment, "x", ({"x"}), VariableDeclaration, m_someIdentifierStringRef, nullptr)
    TEST_ENDVISIT_REDUCES_STACK_OBJ(TwoVarDeclarations, "var i,e=5", ({"var", "i", "e=5"}), VariableDeclarationList, &m_twoVarDeclarations)
    TEST_ENDVISIT_REDUCES_STACK_OBJ(OneVarDeclaration, "var e=5", ({"var", "e=5"}), VariableDeclarationList, &m_twoVarDeclarationsPart2)
    TEST_ENDVISIT_REDUCES_STACK(DefaultScenario, "x;", ({"x"}), VariableStatement, nullptr)

};

QTEST_MAIN(TestPureJavaScriptGenerator)
#include "testpurejavascriptgenerator.moc"
