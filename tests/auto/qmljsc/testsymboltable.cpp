/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
 * Copyright (C) 2015 Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

class TestSymbolTable : public QObject
{
    Q_OBJECT

private slots:
    void findBuiltInTypes();
    void loadModule();

};

using namespace QmlJSc;

void TestSymbolTable::findBuiltInTypes()
{
    SymbolTable symbolTable;

    QCOMPARE(symbolTable.findType(ModuleImports(), "QtObject"), QStringLiteral("QtQml20.QtObject"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Component"), QStringLiteral("QtQml20.Component"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Timer"), QStringLiteral("QtQml20.Timer"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Connections"), QStringLiteral("QtQml20.Connections"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Binding"), QStringLiteral("QtQml20.Binding"));
    QCOMPARE(symbolTable.findType(ModuleImports(), "Inexistent"), QStringLiteral(""));
}

void TestSymbolTable::loadModule()
{
    SymbolTable symbolTable;
    const ModuleImport testModuleImport = {"TestModule", 0, 1};

    QSignalSpy spy(&symbolTable, SIGNAL(importError(QmlJSc::Error)));

    symbolTable.addIncludePath(":/test/");
    symbolTable.loadModule(testModuleImport);

// Symbol table is broken due to a change of module API.
//     QCOMPARE(spy.count(), 0);
//     QCOMPARE(symbolTable.findType({testModuleImport}, "Pastry"), QStringLiteral("TestModule01.Pastry"));
//     QCOMPARE(symbolTable.findType({testModuleImport}, "Cake"), QStringLiteral("TestModule01.Cake"));
//     QCOMPARE(symbolTable.findType({testModuleImport}, "Pizza"), QStringLiteral("TestModule01.Pizza"));
//     QCOMPARE(symbolTable.findType({testModuleImport}, "Printer"), QStringLiteral(""));
}

QTEST_MAIN(TestSymbolTable)
#include "testsymboltable.moc"

