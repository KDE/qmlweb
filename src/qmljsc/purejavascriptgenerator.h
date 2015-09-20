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
#include <private/qqmljslexer_p.h>

class PureJavaScriptGenerator : public QQmlJS::AST::Visitor {

public:
    PureJavaScriptGenerator();

    QString getGeneratedCode();

    virtual bool visit(QQmlJS::AST::BinaryExpression *) override;
    virtual bool visit(QQmlJS::AST::BreakStatement *) override;
    virtual bool visit(QQmlJS::AST::CaseClause *) override;
    virtual bool visit(QQmlJS::AST::ContinueStatement *) override;
    virtual bool visit(QQmlJS::AST::DefaultClause *) override;
    virtual bool visit(QQmlJS::AST::Elision *) override;
    virtual bool visit(QQmlJS::AST::FalseLiteral *) override;
    virtual bool visit(QQmlJS::AST::FormalParameterList *) override;
    virtual bool visit(QQmlJS::AST::IdentifierExpression *) override;
    virtual bool visit(QQmlJS::AST::IdentifierPropertyName *) override;
    virtual bool visit(QQmlJS::AST::NullExpression *) override;
    virtual bool visit(QQmlJS::AST::NumericLiteral *) override;
    virtual bool visit(QQmlJS::AST::NumericLiteralPropertyName *) override;
    virtual bool visit(QQmlJS::AST::RegExpLiteral *) override;
    virtual bool visit(QQmlJS::AST::StringLiteral *) override;
    virtual bool visit(QQmlJS::AST::ThisExpression *) override;
    virtual bool visit(QQmlJS::AST::TrueLiteral *) override;

    virtual void endVisit(QQmlJS::AST::ArgumentList *) override;
    virtual void endVisit(QQmlJS::AST::ArrayLiteral *) override;
    virtual void endVisit(QQmlJS::AST::ArrayMemberExpression *) override;
    virtual void endVisit(QQmlJS::AST::BinaryExpression *) override;
    virtual void endVisit(QQmlJS::AST::Block *) override;
    virtual void endVisit(QQmlJS::AST::CallExpression *) override;
    virtual void endVisit(QQmlJS::AST::CaseBlock *) override;
    virtual void endVisit(QQmlJS::AST::CaseClause *) override;
    virtual void endVisit(QQmlJS::AST::CaseClauses *) override;
    virtual void endVisit(QQmlJS::AST::ConditionalExpression *) override;
    virtual void endVisit(QQmlJS::AST::DefaultClause *) override;
    virtual void endVisit(QQmlJS::AST::DeleteExpression *) override;
    virtual void endVisit(QQmlJS::AST::DoWhileStatement *) override;
    virtual void endVisit(QQmlJS::AST::ElementList *) override;
    virtual void endVisit(QQmlJS::AST::EmptyStatement *) override;
    virtual void endVisit(QQmlJS::AST::Expression *) override;
    virtual void endVisit(QQmlJS::AST::ExpressionStatement *) override;
    virtual void endVisit(QQmlJS::AST::FieldMemberExpression *) override;
    virtual void endVisit(QQmlJS::AST::ForEachStatement *) override;
    virtual void endVisit(QQmlJS::AST::ForStatement *) override;
    virtual void endVisit(QQmlJS::AST::FunctionBody *) override;
    virtual void endVisit(QQmlJS::AST::FunctionDeclaration *) override;
    virtual void endVisit(QQmlJS::AST::FunctionExpression *) override;
    virtual void endVisit(QQmlJS::AST::IdentifierExpression *) override;
    virtual void endVisit(QQmlJS::AST::IfStatement *) override;
    virtual void endVisit(QQmlJS::AST::LabelledStatement *) override;
    virtual void endVisit(QQmlJS::AST::LocalForEachStatement *) override;
    virtual void endVisit(QQmlJS::AST::LocalForStatement *) override;
    virtual void endVisit(QQmlJS::AST::NewExpression *) override;
    virtual void endVisit(QQmlJS::AST::NewMemberExpression *) override;
    virtual void endVisit(QQmlJS::AST::NestedExpression *) override;
    virtual void endVisit(QQmlJS::AST::NotExpression *) override;
    virtual void endVisit(QQmlJS::AST::NumericLiteral *) override;
    virtual void endVisit(QQmlJS::AST::ObjectLiteral *) override;
    virtual void endVisit(QQmlJS::AST::PostDecrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::PostIncrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::PreDecrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::PreIncrementExpression *) override;
    virtual void endVisit(QQmlJS::AST::PropertyAssignmentList *) override;
    virtual void endVisit(QQmlJS::AST::PropertyGetterSetter *) override;
    virtual void endVisit(QQmlJS::AST::PropertyNameAndValue *) override;
    virtual void endVisit(QQmlJS::AST::ReturnStatement *) override;
    virtual void endVisit(QQmlJS::AST::SourceElements *) override;
    virtual void endVisit(QQmlJS::AST::StringLiteral *) override;
    virtual void endVisit(QQmlJS::AST::StatementList *) override;
    virtual void endVisit(QQmlJS::AST::SwitchStatement *) override;
    virtual void endVisit(QQmlJS::AST::TildeExpression *) override;
    virtual void endVisit(QQmlJS::AST::TypeOfExpression *) override;
    virtual void endVisit(QQmlJS::AST::UnaryMinusExpression *) override;
    virtual void endVisit(QQmlJS::AST::UnaryPlusExpression *) override;
    virtual void endVisit(QQmlJS::AST::VariableDeclaration *) override;
    virtual void endVisit(QQmlJS::AST::VariableDeclarationList *) override;
    virtual void endVisit(QQmlJS::AST::VariableStatement *) override;
    virtual void endVisit(QQmlJS::AST::VoidExpression *) override;
    virtual void endVisit(QQmlJS::AST::WhileStatement *) override;
    virtual void endVisit(QQmlJS::AST::WithStatement *) override;

private:
    template<typename ListType> void reduceListStack(ListType* list, const char* separator = "");
    void reduceJumpStatement(const char *keyword, QStringRef label);

    QStack<QString> m_outputStack;

    friend class TestPureJavaScriptGenerator;

    const QString regExpFlagsString(const QQmlJS::Lexer::RegExpFlag &flagsAsEnum) const;
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
