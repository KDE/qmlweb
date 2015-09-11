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

#ifndef PRETTYGENERATORSTAGE_H
#define PRETTYGENERATORSTAGE_H


#include "../ir/visitor.h"
#include "../compilerpass.h"


namespace QmlJSc {

class PrettyGeneratorPass : public CompilerPass, public IR::Visitor
{
 Q_OBJECT

public:
 PrettyGeneratorPass();

 virtual void visit(IR::File* file) override;
 virtual void endVisit(IR::File* file) override;

public slots:
 void process(IR::File *file) override;

private:
 int m_levelSpaceCount = 4;
 QTextStream m_output;
 bool m_componentRoot;

};

}

#endif // PRETTYGENERATORSTAGE_H
