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

#ifndef QMLWEB_PUREJAVASCRIPTGENERATOR_H
#define QMLWEB_PUREJAVASCRIPTGENERATOR_H

#include <QStack>

#include <private/qqmljsast_p.h>

class PureJavaScriptGenerator : public QQmlJS::AST::Visitor {

public:
    PureJavaScriptGenerator();

    QString getGeneratedCode();

    virtual bool visit(QQmlJS::AST::BinaryExpression *) override;
    virtual bool visit(QQmlJS::AST::Block *) override;
    virtual bool visit(QQmlJS::AST::BreakStatement *) override;
    virtual bool visit(QQmlJS::AST::FormalParameterList *) override;
    virtual bool visit(QQmlJS::AST::FunctionBody *) override;
    virtual bool visit(QQmlJS::AST::FunctionDeclaration *) override;
    virtual bool visit(QQmlJS::AST::IdentifierExpression *) override;
    virtual bool visit(QQmlJS::AST::NumericLiteral *) override;
    virtual bool visit(QQmlJS::AST::StringLiteral *) override;
    virtual bool visit(QQmlJS::AST::PostDecrementExpression *) override;
    virtual bool visit(QQmlJS::AST::PostIncrementExpression *) override;
    virtual bool visit(QQmlJS::AST::PreDecrementExpression *) override;
    virtual bool visit(QQmlJS::AST::PreIncrementExpression *) override;
    virtual bool visit(QQmlJS::AST::VariableDeclaration *) override;
    virtual bool visit(QQmlJS::AST::VariableDeclarationList *) override;

    virtual void endVisit(QQmlJS::AST::BinaryExpression *) override;
    virtual void endVisit(QQmlJS::AST::Block *) override;
    virtual void endVisit(QQmlJS::AST::EmptyStatement *) override;
    virtual void endVisit(QQmlJS::AST::ExpressionStatement *) override;
    virtual void endVisit(QQmlJS::AST::FunctionBody *) override;
    virtual void endVisit(QQmlJS::AST::FunctionDeclaration *) override;
    virtual void endVisit(QQmlJS::AST::IdentifierExpression *) override;
    virtual void endVisit(QQmlJS::AST::NumericLiteral *) override;
    virtual void endVisit(QQmlJS::AST::PostDecrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::PostIncrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::PreDecrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::PreIncrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::SourceElements *) override;
    virtual void endVisit(QQmlJS::AST::StatementList *) override;
    virtual void endVisit(QQmlJS::AST::StringLiteral *) override;
    virtual void endVisit(QQmlJS::AST::VariableDeclaration *) override;
    virtual void endVisit(QQmlJS::AST::VariableDeclarationList *) override;
    virtual void endVisit(QQmlJS::AST::VariableStatement *) override;

private:
    void updateStackWithPostOperation();
    void updateStackWithPreOperation();
    template<typename ListType> void reduceListStack(ListType* list, const char* separator = "");

    QStack<QString> m_outputStack;

    friend class TestPureJavaScriptGenerator;
};

template<typename ListType> void PureJavaScriptGenerator::reduceListStack(ListType* current, const char* separator) {
    // the list is only iterated to count the elements
    // current is not processed in any way
    QString code;
    while (current) {
        code.prepend(m_outputStack.pop());
        if (current->next) {
            code.prepend(separator);
        }
        current = current->next;
    }
    m_outputStack << code;
}

#endif //QMLWEB_PUREJAVASCRIPTGENERATOR_H
