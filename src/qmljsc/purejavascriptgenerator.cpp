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
        default: Q_ASSERT(binaryExpression->op);
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

bool PureJavaScriptGenerator::visit(AST::FormalParameterList *parameter) {
    m_outputStack << parameter->name.toString();
    return true;
}

bool PureJavaScriptGenerator::visit(AST::FunctionBody *) {
    m_outputStack << "{";
    return true;
}

bool PureJavaScriptGenerator::visit(AST::FunctionDeclaration *functionDeclaration) {
    const QString functionName = functionDeclaration->name.toString();
    m_outputStack << QString("function") + ' ' + functionName;
    if (!functionDeclaration->formals) {
        // if no parameters are set, formals will be zero and therefore not be visited
        // but a parameter string is needed on the stack
        m_outputStack << "";
    }
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
    QString variableDeclarationCode;
    const QString identifier = variableDeclaration->name.toString();
    if (variableDeclaration->readOnly) {
        variableDeclarationCode += "const";
    } else {
        variableDeclarationCode += "var";
    }
    variableDeclarationCode += ' ';
    variableDeclarationCode += identifier;
    if (variableDeclaration->expression) {
        variableDeclarationCode += '=';
    }

    m_outputStack << variableDeclarationCode;

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

void PureJavaScriptGenerator::endVisit(AST::FormalParameterList *parameterList) {
    reduceListStack<AST::FormalParameterList>(parameterList, ",");
}

void PureJavaScriptGenerator::endVisit(AST::FunctionBody *) {
    const QString body = m_outputStack.pop();
    const QString openingBracket = m_outputStack.pop();
    m_outputStack << openingBracket + body + '}';
}

void PureJavaScriptGenerator::endVisit(AST::FunctionDeclaration *) {
    const QString body = m_outputStack.pop();
    const QString parameters = m_outputStack.pop();
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

void PureJavaScriptGenerator::endVisit(AST::VariableDeclaration *) {
    const QString expression = m_outputStack.pop();
    const QString variableName = m_outputStack.pop();
    m_outputStack << variableName + expression;
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