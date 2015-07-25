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

void PrettyGeneratorPass::process(IR::Component* immediateRepresentation)
{
    Q_ASSERT(immediateRepresentation);

    immediateRepresentation->accept(this);

    emit finished( *(m_output.string()) );
}

void PrettyGeneratorPass::visit(IR::Component* component) {
    const QString objectIdentifier = component->super()->name();
    const QString objectFqi = m_symbols->findType(ModuleImports(), objectIdentifier);

    m_output << TEMPLATE_COMPONENT_HEAD
                        .arg(RUNTIME_INHERIT)
                        .arg("__comp")
                        .arg(objectFqi)
                        .arg("QWContext");
}

void PrettyGeneratorPass::endVisit(IR::Component *) {
    m_output << '}'; // close __comp definition
    m_output << TEMPLATE_COMPONENT_FOOT
                        .arg("__comp");
}