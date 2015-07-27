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

#include <QtCore/QTextStream>

#include "compiler.h"
#include "compilerpipeline.h"

#include "prettygeneratorpass.h"
#include "symboltable.h"

using namespace QmlJSc;

const QString RUNTIME_INHERIT = QStringLiteral("QW_INHERIT");
const QString RUNTIME_CONTEXT = QStringLiteral("QWContext");

const QString TEMPLATE_COMPONENT_HEAD = QStringLiteral(
    "%1(%2, %3);\n" \
    "function %2(parent) {\n" \
    "    %3.call(this, parent);\n" \
    "    var __ = new %4(this);\n" \
);
const QString TEMPLATE_COMPONENT_FOOT = QStringLiteral(
    "\n\n" \
    "%1;"
);


PrettyGeneratorPass::PrettyGeneratorPass(SymbolTable* symbolTable)
        : m_symbols(symbolTable)
        , m_output()
{
    m_componentRoot = false;
    m_output.setString(new QString());
}

void PrettyGeneratorPass::process(QQmlJS::AST::UiProgram* ast)
{
    Q_ASSERT(ast);

    ast->accept(this);
}

bool PrettyGeneratorPass::visit(QQmlJS::AST::UiProgram* uiProgram)
{
    m_componentRoot = true;
    return true;
}

bool PrettyGeneratorPass::visit(QQmlJS::AST::UiObjectDefinition* objectDefinition)
{
    if (m_componentRoot) {
        const QString objectIdentifier = objectDefinition->qualifiedTypeNameId->name.toString();
        const QString objectFqi = m_symbols->findType(ModuleImports(), objectIdentifier);

        m_output << TEMPLATE_COMPONENT_HEAD.arg(RUNTIME_INHERIT)
                                           .arg("__comp")
                                           .arg(objectFqi)
                                           .arg(RUNTIME_CONTEXT);

        m_componentRoot = false;
    }
    return true;
}


void PrettyGeneratorPass::endVisit(QQmlJS::AST::UiProgram* uiProgram)
{
    m_output << TEMPLATE_COMPONENT_FOOT.arg("__comp");
}

void PrettyGeneratorPass::endVisit(QQmlJS::AST::UiObjectDefinition* objectDefinition)
{
    m_output << '}';
}


void PrettyGeneratorPass::postVisit(QQmlJS::AST::Node* node)
{
    if (node->kind == QQmlJS::AST::Node::Kind_UiProgram) {
        m_output.flush();
        emit finished(*(m_output.string()));
    }
}
