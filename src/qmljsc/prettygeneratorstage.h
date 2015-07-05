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

#ifndef PRETTYGENERATORSTAGE_H
#define PRETTYGENERATORSTAGE_H

#include <private/qqmljsastvisitor_p.h>
#include <private/qqmljsast_p.h>

#include "compilerpass.h"


namespace QmlJSc {

class PrettyGeneratorStage : public CompilerPass, public QQmlJS::AST::Visitor
{
 Q_OBJECT

public:
 PrettyGeneratorStage();

 bool visit(QQmlJS::AST::UiProgram*) override;
 bool visit(QQmlJS::AST::UiObjectDefinition*) override;

 void endVisit(QQmlJS::AST::UiProgram*) override;
 void endVisit(QQmlJS::AST::UiObjectDefinition*) override;

 void postVisit(QQmlJS::AST::Node*) override;

public slots:
 void process(QQmlJS::AST::UiProgram*) override;

private:
 int m_levelSpaceCount = 4;
 QTextStream m_output;
 bool m_componentRoot;

};

}

#endif // PRETTYGENERATORSTAGE_H
