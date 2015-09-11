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

#define TEST_VISIT_PUTS_ON_STACK(className, testSituation, expectedStackContent, instance) \
            void test_visit_ ## className ## _ ## testSituation ## _returnsTrue() { \
                QCOMPARE(m_generator->visit(&instance), true); \
            } \
            void test_ ## visit ## _ ## className ## _ ## testSituation ## _putsOnStack() { \
                m_generator->visit(&instance); \
                QCOMPARE(asPureJSGen(m_generator)->m_outputStack.top(), QStringLiteral(expectedStackContent)); \
                QCOMPARE(asPureJSGen(m_generator)->m_outputStack.size(), 1); \
            }

#define TEST_VISIT_BINARYOP_PUTS_ON_STACK(operatorEnumName, character) \
            void test_visit_BinaryExpression_ ## operatorEnumName ## _returnsTrue() { \
                QQmlJS::AST::BinaryExpression classInstance(nullptr, QSOperator::operatorEnumName, nullptr); \
                QCOMPARE(m_generator->visit(&classInstance), true); \
            } \
            void test_visit_BinaryExpression_ ## operatorEnumName ## _putsOnStack() { \
                QQmlJS::AST::BinaryExpression instance(nullptr, QSOperator::operatorEnumName, nullptr); \
                QStack<QString> expectedStack;\
                expectedStack.append(character);\
                m_generator->visit(&instance); \
                QCOMPARE(asPureJSGen(m_generator)->m_outputStack, expectedStack); \
            }

#define TEST_VISIT_DEFAULT_IMPL_(className, instance) \
            void test_visit ## _ ## className ## _defaultImplementation_returns_true() { \
                QCOMPARE(m_generator->visit(&instance), true); \
            } \
            void test_ ## visit ## _ ## className ## _ ## testSituation ## _putsNothingOnStack() { \
                m_generator->visit(&instance); \
                QCOMPARE(asPureJSGen(m_generator)->m_outputStack.count(), 0); \
            }

#define TEST_ENDVISIT_REDUCES(className, scenarioName, expectedTopOfStack, stackContent, instance) \
            void test_endVisit_ ## className ## scenarioName ## _reducesStack() { \
                asPureJSGen(m_generator)->m_outputStack.append(QVector<QString>stackContent); \
                m_generator->endVisit(&instance); \
                QCOMPARE(asPureJSGen(m_generator)->m_outputStack.top(), QStringLiteral(expectedTopOfStack)); \
                QCOMPARE(asPureJSGen(m_generator)->m_outputStack.count(), 1); \
            }


using namespace QQmlJS;

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
        /* Expressions */
        , m_identifierExpression(m_someIdentifierStringRef)
        , m_numericalExpressionPi(3.14)
        , m_stringLiteral(m_someStringStringRef)
        , m_trueExpression()
        , m_falseExpression()
        , m_equalsBinaryExpression(nullptr, QSOperator::Equal, nullptr)
        , m_postDecrementExpression(&m_numericalExpressionPi)
        , m_postIncrementExpression(&m_numericalExpressionPi)
        , m_preDecrementExpression(&m_numericalExpressionPi)
        , m_preIncrementExpression(&m_numericalExpressionPi)
        , m_block(nullptr)
        /* Variable Declarations */
        , m_constDeclaration1(m_someIdentifierStringRef, nullptr)
        , m_constDeclaration2(m_anotherIdentifierStringRef, nullptr)
        , m_variableDeclarationWithAssignment(m_someIdentifierStringRef, &m_trueExpression)
        , m_variableDeclarationWithoutAssignment(m_someIdentifierStringRef, nullptr)
        , m_twoConstDeclarations(&m_constDeclaration1)
        , m_twoConstDeclarationsPart2(&m_twoConstDeclarations, &m_constDeclaration2)
        , m_twoVarDeclarations(&m_variableDeclarationWithAssignment)
        , m_twoVarDeclarationsPart2(&m_twoVarDeclarations, &m_variableDeclarationWithoutAssignment)
        /* Statements */
        , m_breakStatementWithLabel(m_someLabelStringRef)
        , m_breakStatementWithoutLabel(nullptr)
        , m_continueStatementWithLabel(m_someLabelStringRef)
        , m_continueStatementWithoutLabel(nullptr)
        , m_emptyStatement()
        , m_statement1()
        , m_statement2()
        , m_statement3()
        , m_expressionStatement(nullptr)
        , m_ifStatementWithoutElse(&m_trueExpression, &m_statement1)
        , m_ifStatementWithElse(&m_trueExpression, &m_statement1, &m_statement2)
        , m_returnStatementWithoutValue(nullptr)
        , m_returnStatementWithValue(&m_trueExpression)
        , m_variableStatement(&m_twoVarDeclarations)
        , m_threeStatementsList(&m_statement1)
        , m_statementListPart2(&m_threeStatementsList, &m_statement2)
        , m_statementListPart3(&m_statementListPart2, &m_statement3)
        , m_sourceElement1(nullptr)
        , m_sourceElement2(nullptr)
        , m_sourceElement3(nullptr)
        , m_threeSourceElementsList(&m_sourceElement1)
        , m_sourceElementsListPart2(&m_threeSourceElementsList, &m_sourceElement2)
        , m_sourceElementsListPart3(&m_sourceElementsListPart2, &m_sourceElement3)
        /* Function declaration */
        , m_twoParameters(m_someIdentifierStringRef)
        , m_parameterListPart2(&m_twoParameters, m_anotherIdentifierStringRef)
        , m_functionBody(nullptr)
        , m_functionDeclarationWithoutParameters(m_someIdentifierStringRef, nullptr, &m_functionBody)
        , m_functionDeclarationWithParameters(m_someIdentifierStringRef, &m_twoParameters , &m_functionBody)
        , m_functionDeclarationWithoutBody(m_someIdentifierStringRef, &m_twoParameters, nullptr)
        /* Switch */
        , m_caseClause1(&m_trueExpression, &m_threeStatementsList)
        , m_caseClause2(&m_trueExpression, &m_threeStatementsList)
        , m_twoCaseClauses(&m_caseClause1)
        , m_twoCaseClausesPart2(&m_twoCaseClauses, &m_caseClause2)
        , m_defaultClause(&m_threeStatementsList)
        , m_caseBlock(&m_twoCaseClauses, &m_defaultClause, &m_twoCaseClauses)
        , m_caseBlockOnlyCases(&m_twoCaseClauses)
        , m_caseBlockCasesAndDefault(&m_twoCaseClauses, &m_defaultClause)
        , m_caseBlockCasesDefaultCases(&m_twoCaseClauses, &m_defaultClause, &m_twoCaseClauses)
        , m_caseClause(&m_trueExpression, &m_threeStatementsList)
        , m_switchStatement(&m_trueExpression, &m_caseBlock)
    {
        m_statementListPart3.finish();
        m_sourceElementsListPart3.finish();
        m_parameterListPart2.finish();
        m_twoConstDeclarationsPart2.finish(true);
        m_twoVarDeclarationsPart2.finish(false);
        m_twoCaseClausesPart2.finish();
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

    /* Expressions */
    AST::IdentifierExpression m_identifierExpression;
    AST::NumericLiteral m_numericalExpressionPi;
    AST::StringLiteral m_stringLiteral;
    AST::TrueLiteral m_trueExpression;
    AST::FalseLiteral m_falseExpression;

    AST::BinaryExpression m_equalsBinaryExpression;

    AST::PostDecrementExpression m_postDecrementExpression;
    AST::PostIncrementExpression m_postIncrementExpression;
    AST::PreDecrementExpression m_preDecrementExpression;
    AST::PreIncrementExpression m_preIncrementExpression;

    /* Variable Declarations */
    AST::VariableDeclaration m_constDeclaration1;
    AST::VariableDeclaration m_constDeclaration2;
    AST::VariableDeclaration m_variableDeclarationWithAssignment;
    AST::VariableDeclaration m_variableDeclarationWithoutAssignment;
    AST::VariableDeclarationList m_twoConstDeclarations;
    AST::VariableDeclarationList m_twoConstDeclarationsPart2;
    AST::VariableDeclarationList m_twoVarDeclarations;
    AST::VariableDeclarationList m_twoVarDeclarationsPart2;

    /* Statements */
    AST::BreakStatement m_breakStatementWithLabel;
    AST::BreakStatement m_breakStatementWithoutLabel;
    AST::ContinueStatement m_continueStatementWithLabel;
    AST::ContinueStatement m_continueStatementWithoutLabel;
    AST::EmptyStatement m_emptyStatement;
    AST::EmptyStatement m_statement1;
    AST::EmptyStatement m_statement2;
    AST::EmptyStatement m_statement3;
    AST::ExpressionStatement m_expressionStatement;
    AST::IfStatement m_ifStatementWithoutElse;
    AST::IfStatement m_ifStatementWithElse;
    AST::ReturnStatement m_returnStatementWithoutValue;
    AST::ReturnStatement m_returnStatementWithValue;
    AST::VariableStatement m_variableStatement;
    AST::Block m_block;
    AST::StatementList m_threeStatementsList;
    AST::StatementList m_statementListPart2;
    AST::StatementList m_statementListPart3;
    AST::StatementSourceElement m_sourceElement1;
    AST::StatementSourceElement m_sourceElement2;
    AST::StatementSourceElement m_sourceElement3;
    AST::SourceElements m_threeSourceElementsList;
    AST::SourceElements m_sourceElementsListPart2;
    AST::SourceElements m_sourceElementsListPart3;

    /* Function declarations */
    AST::FormalParameterList m_twoParameters;
    AST::FormalParameterList m_parameterListPart2;
    AST::FunctionBody m_functionBody;
    AST::FunctionDeclaration m_functionDeclarationWithoutParameters;
    AST::FunctionDeclaration m_functionDeclarationWithParameters;
    AST::FunctionDeclaration m_functionDeclarationWithoutBody;
    
    /* Switch */
    AST::CaseClause m_caseClause1;
    AST::CaseClause m_caseClause2;
    AST::CaseClauses m_twoCaseClauses;
    AST::CaseClauses m_twoCaseClausesPart2;
    AST::DefaultClause m_defaultClause;
    AST::CaseBlock m_caseBlock;
    AST::CaseBlock m_caseBlockOnlyCases;
    AST::CaseBlock m_caseBlockCasesAndDefault;
    AST::CaseBlock m_caseBlockCasesDefaultCases;
    AST::CaseClause m_caseClause;
    AST::SwitchStatement m_switchStatement;

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

    TEST_VISIT_PUTS_ON_STACK(Block                  , AnyCase           , "{"               , m_block)
    TEST_VISIT_PUTS_ON_STACK(BreakStatement         , WithLabel         , "break ALabel;"   , m_breakStatementWithLabel)
    TEST_VISIT_PUTS_ON_STACK(BreakStatement         , WithoutLabel      , "break;"          , m_breakStatementWithoutLabel)
    TEST_VISIT_PUTS_ON_STACK(CaseBlock              , AnyCase           , "{"               , m_caseBlock)
    TEST_VISIT_PUTS_ON_STACK(CaseClause             , AnyCase           , "case"            , m_caseClause1)
    TEST_VISIT_DEFAULT_IMPL_(CaseClauses                                                    , m_twoCaseClauses)
    TEST_VISIT_PUTS_ON_STACK(DefaultClause          , Default           , "default"         , m_defaultClause)
    TEST_VISIT_PUTS_ON_STACK(ContinueStatement      , WithLabel         , "continue ALabel;", m_continueStatementWithLabel)
    TEST_VISIT_PUTS_ON_STACK(ContinueStatement      , WithoutLabel      , "continue;"       , m_continueStatementWithoutLabel)
    TEST_VISIT_DEFAULT_IMPL_(EmptyStatement                                                 , m_emptyStatement)
    TEST_VISIT_DEFAULT_IMPL_(ExpressionStatement                                            , m_expressionStatement)
    TEST_VISIT_PUTS_ON_STACK(FormalParameterList    , OneParameter      , "e"               , m_parameterListPart2)
    TEST_VISIT_PUTS_ON_STACK(FormalParameterList    , TwoParameters     , "i,e"             , m_twoParameters)
    TEST_VISIT_PUTS_ON_STACK(FunctionBody           , AnyCase           , "{"               , m_functionBody)
    TEST_VISIT_PUTS_ON_STACK(FunctionDeclaration    , WithParameters    , "function i"      , m_functionDeclarationWithParameters)
    TEST_VISIT_PUTS_ON_STACK(FunctionDeclaration    , WithoutParameters , "function i"      , m_functionDeclarationWithoutParameters)
    TEST_VISIT_PUTS_ON_STACK(IdentifierExpression   , AnyCase           , "i"               , m_identifierExpression)
    TEST_VISIT_PUTS_ON_STACK(IfStatement            , AnyCase           , "if"              , m_ifStatementWithoutElse)
    TEST_VISIT_PUTS_ON_STACK(NumericLiteral         , Pi                , "3.14"            , m_numericalExpressionPi)
    TEST_VISIT_PUTS_ON_STACK(PostDecrementExpression, AnyCase           , "--"              , m_postDecrementExpression)
    TEST_VISIT_PUTS_ON_STACK(PostIncrementExpression, AnyCase           , "++"              , m_postIncrementExpression)
    TEST_VISIT_PUTS_ON_STACK(PreDecrementExpression , AnyCase           , "--"              , m_preDecrementExpression)
    TEST_VISIT_PUTS_ON_STACK(PreIncrementExpression , AnyCase           , "++"              , m_preIncrementExpression)
    TEST_VISIT_PUTS_ON_STACK(ReturnStatement        , WithoutReturnValue, "return"          , m_returnStatementWithoutValue)
    TEST_VISIT_PUTS_ON_STACK(ReturnStatement        , WithReturnValue   , "return"          , m_returnStatementWithValue)
    TEST_VISIT_DEFAULT_IMPL_(SourceElements                                                 , m_threeSourceElementsList)
    TEST_VISIT_DEFAULT_IMPL_(StatementList                                                  , m_threeStatementsList)
    TEST_VISIT_PUTS_ON_STACK(StringLiteral          , AnyCase           , "some string"     , m_stringLiteral) //TODO correct? probably " is missing in generation
    TEST_VISIT_PUTS_ON_STACK(SwitchStatement        , AnyCase           , "switch"          , m_switchStatement)
    TEST_VISIT_PUTS_ON_STACK(VariableDeclaration    , Assignment        , "i="              , m_variableDeclarationWithAssignment)
    TEST_VISIT_PUTS_ON_STACK(VariableDeclaration    , NoAssignment      , "i"               , m_variableDeclarationWithoutAssignment)
    TEST_VISIT_PUTS_ON_STACK(VariableDeclarationList, VarDeclaratio     , "var"             , m_twoVarDeclarations)
    TEST_VISIT_PUTS_ON_STACK(VariableDeclarationList, ConstDeclaration  , "const"           , m_twoConstDeclarations)
    TEST_VISIT_DEFAULT_IMPL_(VariableStatement                                              , m_variableStatement)

    TEST_ENDVISIT_REDUCES(BinaryExpression        , TwoOperands       , "2==4"            , ({"==", "2", "4"})                , m_equalsBinaryExpression)
    TEST_ENDVISIT_REDUCES(Block                   , AnyCase           , "{content}"       , ({"{", "content"})                , m_block)
    TEST_ENDVISIT_REDUCES(CaseBlock               , OnlyCases         , "{cases}"         , ({"{", "cases"})                  , m_caseBlockOnlyCases)
    TEST_ENDVISIT_REDUCES(CaseBlock               , CasesAndDefault   , "{casesdefault}"  , ({"{", "cases", "default"})       , m_caseBlockCasesAndDefault)
    TEST_ENDVISIT_REDUCES(CaseBlock               , CasesDefaultCases , "{casdefaultcas}" , ({"{", "cas", "default", "cas"})  , m_caseBlockCasesDefaultCases)
    TEST_ENDVISIT_REDUCES(CaseClause              , CaseWithStatement , "case exp:stm;"   , ({"case", "exp", "stm;"})         , m_caseClause)
    TEST_ENDVISIT_REDUCES(CaseClauses             , TwoClauses        , "case e:s;case e2:s2;", ({"case e:s;", "case e2:s2;"}), m_twoCaseClauses)
    TEST_ENDVISIT_REDUCES(DefaultClause           , AnyCase           , "default:stm"     , ({"default", "stm"})              , m_defaultClause)
    TEST_ENDVISIT_REDUCES(EmptyStatement          , DefaultScenario   , ";"               , ({})                              , m_emptyStatement)
    TEST_ENDVISIT_REDUCES(ExpressionStatement     , AnyCase           , "expression;"     , ({"expression"})                  , m_expressionStatement)
    TEST_ENDVISIT_REDUCES(FormalParameterList     , AnyCase           , "i"               , ({"i"})                           , m_twoParameters) // does nothing
    TEST_ENDVISIT_REDUCES(FunctionBody            , ClosesCorrectly   , "{func}"          , ({"{", "func"})                   , m_functionBody)
    TEST_ENDVISIT_REDUCES(FunctionDeclaration     , BodyNoParameters  , "name(){body}"    , ({"name", "{body}"})              , m_functionDeclarationWithoutParameters)
    TEST_ENDVISIT_REDUCES(FunctionDeclaration     , BodyParameters    , "name(para){body}", ({"name", "para", "{body}"})      , m_functionDeclarationWithParameters)
    TEST_ENDVISIT_REDUCES(FunctionDeclaration     , WithoutBody       , "name(para){}"    , ({"name", "para"})                , m_functionDeclarationWithoutBody)
    TEST_ENDVISIT_REDUCES(IdentifierExpression    , AnyCase           , "abc"             , ({"abc"})                         , m_identifierExpression)
    TEST_ENDVISIT_REDUCES(IfStatement             , OnlyIf            , "if(exp)stm;"     , ({"if", "exp", "stm;"})           , m_ifStatementWithoutElse)
    TEST_ENDVISIT_REDUCES(IfStatement             , IfElse            , "if(exp)s;else s;", ({"if", "exp", "s;", "s;"})       , m_ifStatementWithElse)
    TEST_ENDVISIT_REDUCES(NumericLiteral          , AnyCase           , "2.7"             , ({"2.7"})                         , m_numericalExpressionPi)
    TEST_ENDVISIT_REDUCES(PostDecrementExpression , AnyCase           , "2.7--"           , ({"--", "2.7"})                   , m_postDecrementExpression)
    TEST_ENDVISIT_REDUCES(PostIncrementExpression , AnyCase           , "2.7++"           , ({"++", "2.7"})                   , m_postIncrementExpression)
    TEST_ENDVISIT_REDUCES(PreDecrementExpression  , AnyCase           , "--2.7"           , ({"--", "2.7"})                   , m_preDecrementExpression)
    TEST_ENDVISIT_REDUCES(PreIncrementExpression  , AnyCase           , "++2.7"           , ({"++", "2.7"})                   , m_preIncrementExpression)
    TEST_ENDVISIT_REDUCES(ReturnStatement         , WithoutReturnValue, "return;"         , ({"return"})                      , m_returnStatementWithoutValue)
    TEST_ENDVISIT_REDUCES(ReturnStatement         , WithReturnValue   , "return true;"    , ({"return", "true"})              , m_returnStatementWithValue)
    TEST_ENDVISIT_REDUCES(SourceElements          , ThreeSrcElements  , "sEl1sEl2sEl3"    , ({"sEl1", "sEl2", "sEl3"})        , m_threeSourceElementsList)
    TEST_ENDVISIT_REDUCES(SourceElements          , ThreeStatements   , "st1st2st3"       , ({"st1", "st2", "st3"})           , m_threeStatementsList)
    TEST_ENDVISIT_REDUCES(StringLiteral           , AnyCase           , "another string"  , ({"another string"})              , m_stringLiteral)
    TEST_ENDVISIT_REDUCES(SwitchStatement         , AnyCase           , "switch(e){blk}"  , ({"switch", "e", "{blk}"})        , m_switchStatement)
    TEST_ENDVISIT_REDUCES(VariableDeclaration     , WithAssignment    , "x=5"             , ({"x=", "5"})                     , m_variableDeclarationWithAssignment)
    TEST_ENDVISIT_REDUCES(VariableDeclaration     , WithoutAssignment , "x"               , ({"x"})                           , m_variableDeclarationWithoutAssignment)
    TEST_ENDVISIT_REDUCES(VariableDeclarationList , TwoDeclarations   , "var i,e=5"       , ({"var", "i", "e=5"})             , m_twoVarDeclarations)
    TEST_ENDVISIT_REDUCES(VariableDeclarationList , OneDeclaration    , "var e=5"         , ({"var", "e=5"})                  , m_twoVarDeclarationsPart2)
    TEST_ENDVISIT_REDUCES(VariableStatement       , AnyCase           , "x;"              , ({"x"})                           , m_variableStatement)

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
};

QTEST_MAIN(TestPureJavaScriptGenerator)
#include "testpurejavascriptgenerator.moc"
