/*
 * Copyright (C) 2015  Anton Kreuzkamp <akreuzkamp@web.de>
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

#include <QtCore/QObject>
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>

#include <QtCore/QDebug>

#include "../../../src/qmljsc/symboltable.h"

class TestParserStage : public QObject
{
    Q_OBJECT

private slots:
    void findBuiltInTypes();

};

using namespace QmlJSc;

void TestParserStage::findBuiltInTypes()
{
    SymbolTable symbolTable;

    QCOMPARE(symbolTable.findType(ModuleImports(), "QtObject"), QStringLiteral("QtQml20.QtObject"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Component"), QStringLiteral("QtQml20.Component"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Timer"), QStringLiteral("QtQml20.Timer"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Connections"), QStringLiteral("QtQml20.Connections"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Binding"), QStringLiteral("QtQml20.Binding"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Inexistent"), QStringLiteral(""));
}

QTEST_MAIN(TestParserStage)
#include "testsymboltable.moc"

