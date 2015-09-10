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

#include "compilerpipeline.h"

#include "compilerpass.h"

using namespace QmlJSc;

CompilerPass::CompilerPass(QObject *parent): QObject(parent)
{

}

void CompilerPass::connectToSuccessor(CompilerPass *successor) {
    connect(this, SIGNAL(finished(QString)), successor, SLOT(process(QString)));
    connect(this, SIGNAL(finished(QQmlJS::AST::UiProgram*)), successor, SLOT(process(QQmlJS::AST::UiProgram*)));
}


void CompilerPass::process(QString) {
    failBecauseOfWrongType();
}

void CompilerPass::process(QQmlJS::AST::UiProgram*) {
    failBecauseOfWrongType();
}

void CompilerPass::process(IR::File*) {
    failBecauseOfWrongType();
}

void CompilerPass::failBecauseOfWrongType() {
    Q_STATIC_ASSERT_X(1, "The type is not supported by this compiler pass, is the compiler pass order correct?");
}

