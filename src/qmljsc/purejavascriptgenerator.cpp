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

PureJavaScriptGenerator::PureJavaScriptGenerator()
    : m_outputStack()
    , m_generatedCode(new QString())
{
}

QString PureJavaScriptGenerator::getGeneratedCode() {
    return *m_generatedCode.string();
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::BinaryExpression *binaryExpression) {
    switch(binaryExpression->op) {
        case QSOperator::Assign: m_outputStack << "="; break;
        default: Q_ASSERT(binaryExpression->op);
    }
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::Block *) {
    m_generatedCode << '{';
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::BreakStatement *breakStatement) {
    m_generatedCode << "break";
    if (breakStatement->label.length() > 0) {
        m_generatedCode << ' ';
        m_generatedCode << breakStatement->label.toString();
    }

    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::FunctionBody *) {
    m_generatedCode << "{";
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::FunctionDeclaration *functionDeclaration) {
    const QString functionName = functionDeclaration->name.toString();
    m_generatedCode << "function" << ' ' << functionName;
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::IdentifierExpression *identifierExpression) {
    m_outputStack << identifierExpression->name.toString();
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::NumericLiteral *numericLiteral) {
    m_outputStack.push(QString("%1").arg(numericLiteral->value));
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::StringLiteral *stringLiteral) {
    m_outputStack.push(stringLiteral->value.toString());
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::PostDecrementExpression *) {
    m_outputStack.push("--");
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::PostIncrementExpression *) {
    m_outputStack.push("++");
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::PreDecrementExpression *) {
    m_outputStack.push("--");
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::PreIncrementExpression *) {
    m_outputStack.push("++");
    return true;
}

bool PureJavaScriptGenerator::visit(QQmlJS::AST::VariableDeclaration *variableDeclaration) {
    const QString identifier = variableDeclaration->name.toString();
    if (variableDeclaration->readOnly) {
        m_generatedCode << "const";
    } else {
        m_generatedCode << "var";
    }
    m_generatedCode << ' ' <<  identifier;
    if (variableDeclaration->expression) {
        m_generatedCode << '=';
    }
    return true;
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::BinaryExpression *) {
    const QString rightOperand = m_outputStack.pop();
    const QString leftOperand = m_outputStack.pop();
    const QString operation = m_outputStack.pop();
    QString expression = QString("%1%2%3").arg(leftOperand).arg(operation).arg(rightOperand);
    m_outputStack.push(expression);
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::Block *) {
    m_generatedCode << '}';
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::ExpressionStatement *) {
    m_generatedCode << ';';
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::FunctionBody *) {
    m_generatedCode << '}';
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::IdentifierExpression *) {
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::NumericLiteral *) {
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::PostDecrementExpression *) {
    updateStackWithPostOperation();
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::PostIncrementExpression *) {
    updateStackWithPostOperation();
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::PreDecrementExpression *) {
    updateStackWithPreOperation();
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::PreIncrementExpression *) {
    updateStackWithPreOperation();
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::StringLiteral *) {
    generateIfLastElementOnStack();
}

void PureJavaScriptGenerator::generateIfLastElementOnStack() {
    if (m_outputStack.size() == 1) {
        m_generatedCode << m_outputStack.pop();
    }
}

void PureJavaScriptGenerator::endVisit(QQmlJS::AST::VariableStatement *) {
    m_generatedCode << ';';
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
