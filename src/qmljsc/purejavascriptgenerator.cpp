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

#include "purejavascriptgenerator.h"
#include "utils/error.h"

using namespace QQmlJS;

PureJavaScriptGenerator::PureJavaScriptGenerator()
    : m_outputStack()
{
}

QString PureJavaScriptGenerator::getGeneratedCode() {
    if (m_outputStack.size() > 1) {
        const QString errorDescription = QString("stack was not reduced correctly, top element is %1").arg(m_outputStack.top());
        QmlJSc::Error stackError(QmlJSc::Error::InternalError, errorDescription);
        throw stackError;
    } else if (m_outputStack.size() == 0) {
        return "";
    }
    return m_outputStack.pop();
}

bool PureJavaScriptGenerator::visit(AST::BinaryExpression *binaryExpression) {
    switch(binaryExpression->op) {
        case QSOperator::Assign: m_outputStack << "="; break;
        case QSOperator::InplaceAdd: m_outputStack << "+="; break;
        case QSOperator::InplaceSub: m_outputStack << "-="; break;
        case QSOperator::InplaceMul: m_outputStack << "*="; break;
        case QSOperator::InplaceDiv: m_outputStack << "/="; break;
        case QSOperator::InplaceMod: m_outputStack << "%="; break;
        case QSOperator::InplaceLeftShift: m_outputStack << "<<="; break;
        case QSOperator::InplaceRightShift: m_outputStack << ">>="; break;
        case QSOperator::InplaceURightShift: m_outputStack << ">>>="; break;
        case QSOperator::InplaceAnd: m_outputStack << "&="; break;
        case QSOperator::InplaceXor: m_outputStack << "^="; break;
        case QSOperator::InplaceOr: m_outputStack << "|="; break;
        case QSOperator::Add: m_outputStack << "+"; break;
        case QSOperator::Sub: m_outputStack << "-"; break;
        case QSOperator::Mul: m_outputStack << "*"; break;
        case QSOperator::Div: m_outputStack << "/"; break;
        case QSOperator::Mod: m_outputStack << "%"; break;
        case QSOperator::LShift: m_outputStack << "<<"; break;
        case QSOperator::RShift: m_outputStack << ">>"; break;
        case QSOperator::URShift: m_outputStack << ">>>"; break;
        case QSOperator::BitAnd: m_outputStack << "&"; break;
        case QSOperator::BitXor: m_outputStack << "^"; break;
        case QSOperator::BitOr: m_outputStack << "|"; break;
        case QSOperator::Equal: m_outputStack << "=="; break;
        case QSOperator::NotEqual: m_outputStack << "!="; break;
        case QSOperator::StrictEqual: m_outputStack << "==="; break;
        case QSOperator::StrictNotEqual: m_outputStack << "!=="; break;
        case QSOperator::Gt: m_outputStack << ">"; break;
        case QSOperator::Ge: m_outputStack << ">="; break;
        case QSOperator::Lt: m_outputStack << "<"; break;
        case QSOperator::Le: m_outputStack << "<="; break;
        case QSOperator::And: m_outputStack << "&&"; break;
        case QSOperator::Or: m_outputStack << "||"; break;
        case QSOperator::In: m_outputStack << " in "; break;
        case QSOperator::InstanceOf: m_outputStack << " instanceof "; break;
        default: Q_ASSERT_X(binaryExpression->op, __FILE__, "The operator is not handled yet");
    }
    return true;
}

bool PureJavaScriptGenerator::visit(AST::Block *) {
    m_outputStack << "{";
    return true;
}

bool PureJavaScriptGenerator::visit(AST::BreakStatement *breakStatement) {
    QString breakStatementCode("break");
    if (breakStatement->label.length() > 0) {
        breakStatementCode.append(' ');
        breakStatementCode.append(breakStatement->label.toString());
    }
    m_outputStack << breakStatementCode;

    return true;
}

bool PureJavaScriptGenerator::visit(AST::FormalParameterList *currentParameter) {
    QString parameterCode;
    while (currentParameter) {
        parameterCode += currentParameter->name.toString();
        if (currentParameter->next) {
            parameterCode += ',';
        }
        currentParameter = currentParameter->next;
    }
    m_outputStack << parameterCode;
    return true;
}

bool PureJavaScriptGenerator::visit(AST::FunctionBody *) {
    m_outputStack << "{";
    return true;
}

bool PureJavaScriptGenerator::visit(AST::FunctionDeclaration *functionDeclaration) {
    const QString functionName = functionDeclaration->name.toString();
    m_outputStack << QString("function") + ' ' + functionName;
    return true;
}

bool PureJavaScriptGenerator::visit(AST::IdentifierExpression *identifierExpression) {
    m_outputStack << identifierExpression->name.toString();
    return true;
}

bool PureJavaScriptGenerator::visit(AST::NumericLiteral *numericLiteral) {
    m_outputStack.push(QString("%1").arg(numericLiteral->value));
    return true;
}

bool PureJavaScriptGenerator::visit(AST::StringLiteral *stringLiteral) {
    m_outputStack.push(stringLiteral->value.toString());
    return true;
}

bool PureJavaScriptGenerator::visit(AST::PostDecrementExpression *) {
    m_outputStack.push("--");
    return true;
}

bool PureJavaScriptGenerator::visit(AST::PostIncrementExpression *) {
    m_outputStack.push("++");
    return true;
}

bool PureJavaScriptGenerator::visit(AST::PreDecrementExpression *) {
    m_outputStack.push("--");
    return true;
}

bool PureJavaScriptGenerator::visit(AST::PreIncrementExpression *) {
    m_outputStack.push("++");
    return true;
}

bool PureJavaScriptGenerator::visit(AST::VariableDeclaration *variableDeclaration) {
    const QString identifier = variableDeclaration->name.toString();
    QString variableDeclarationCode = identifier;

    if (variableDeclaration->expression) {
        variableDeclarationCode += '=';
    }

    m_outputStack << variableDeclarationCode;

    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::VariableDeclarationList *variableDeclarationList) {
    if (variableDeclarationList->declaration->readOnly) {
        // read only state is the same for all declarations in one list
        m_outputStack << "const";
    } else {
        m_outputStack << "var";
    }

    return true;
}

void PureJavaScriptGenerator::endVisit(AST::BinaryExpression *) {
    const QString rightOperand = m_outputStack.pop();
    const QString leftOperand = m_outputStack.pop();
    const QString operation = m_outputStack.pop();
    QString expression = QString("%1%2%3").arg(leftOperand).arg(operation).arg(rightOperand);
    m_outputStack.push(expression);
}

void PureJavaScriptGenerator::endVisit(AST::Block *) {
    const QString blockCode = m_outputStack.pop();
    const QString openingParenthesis = m_outputStack.pop();
    m_outputStack << QString("%1%2}").arg(openingParenthesis, blockCode);
}

void PureJavaScriptGenerator::endVisit(AST::EmptyStatement *) {
    m_outputStack << ";";
}

void PureJavaScriptGenerator::endVisit(AST::ExpressionStatement *) {
    m_outputStack << m_outputStack.pop() + ';';
}

void PureJavaScriptGenerator::endVisit(AST::FunctionBody *) {
    const QString body = m_outputStack.pop();
    const QString openingBracket = m_outputStack.pop();
    m_outputStack << openingBracket + body + '}';
}

void PureJavaScriptGenerator::endVisit(AST::FunctionDeclaration *functionDeclaration) {
    const QString body = (functionDeclaration->body)?m_outputStack.pop():"{}";
    const QString parameters = (functionDeclaration->formals)?m_outputStack.pop():"";
    const QString typeAndName = m_outputStack.pop();
    m_outputStack << typeAndName + '(' + parameters + ')' + body;
}

void PureJavaScriptGenerator::endVisit(AST::IdentifierExpression *) {
}

void PureJavaScriptGenerator::endVisit(AST::NumericLiteral *) {
}

void PureJavaScriptGenerator::endVisit(AST::PostDecrementExpression *) {
    updateStackWithPostOperation();
}

void PureJavaScriptGenerator::endVisit(AST::PostIncrementExpression *) {
    updateStackWithPostOperation();
}

void PureJavaScriptGenerator::endVisit(AST::PreDecrementExpression *) {
    updateStackWithPreOperation();
}

void PureJavaScriptGenerator::endVisit(AST::PreIncrementExpression *) {
    updateStackWithPreOperation();
}

void PureJavaScriptGenerator::endVisit(AST::SourceElements *sourceElements) {
    reduceListStack<AST::SourceElements>(sourceElements);
}

void PureJavaScriptGenerator::endVisit(AST::StatementList *statementList) {
    reduceListStack<AST::StatementList>(statementList);
}

void PureJavaScriptGenerator::endVisit(AST::StringLiteral *) {
}

void PureJavaScriptGenerator::endVisit(AST::VariableDeclaration *declaration) {
    const QString expression = (declaration->expression)?m_outputStack.pop():"";
    const QString variableName = m_outputStack.pop();
    m_outputStack << variableName + expression;
}

void PureJavaScriptGenerator::endVisit(AST::VariableDeclarationList *declarationList) {
    reduceListStack<AST::VariableDeclarationList>(declarationList, ",");
    const QString declarationListCode = m_outputStack.pop();
    const QString declarationType = m_outputStack.pop();
    m_outputStack << declarationType + ' ' + declarationListCode;
}

void PureJavaScriptGenerator::endVisit(AST::VariableStatement *) {
    m_outputStack << m_outputStack.pop() + ';';
}

void PureJavaScriptGenerator::updateStackWithPostOperation() {
    const QString expression = m_outputStack.pop();
    const QString operation = m_outputStack.pop();
    m_outputStack.push(QString("%1%2").arg(expression).arg(operation));
}

void PureJavaScriptGenerator::updateStackWithPreOperation() {
    const QString expression = m_outputStack.pop();
    const QString operation = m_outputStack.pop();
    m_outputStack.push(QString("%1%2").arg(operation).arg(expression));
}