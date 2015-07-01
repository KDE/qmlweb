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

#include "../../../src/qmljsc/compiler.h"
#include "../../../src/qmljsc/symboltable.h"

class TestSymbolTable : public QObject
{
    Q_OBJECT

private slots:
    void loadModule();

};

using namespace QmlJSc;

void TestSymbolTable::loadModule()
{
    Compiler c;

    SymbolTable symbolTable;
    const ModuleImport testModuleImport = {"TestModule", 0, 1};

    QSignalSpy spy(&symbolTable, SIGNAL(importError(QmlJSc::Error)));

    compiler->addIncludePath(":/test/");
    symbolTable.loadModule(testModuleImport);

    if (spy.count()) {
        QList<QVariant> arguments = spy.takeFirst();
        qDebug() << arguments.at(0).value<QmlJSc::Error>().what();
    }
    QCOMPARE(spy.count(), 0);
    QVERIFY(symbolTable.type("Pastry"));
    QCOMPARE(symbolTable.fullyQualifiedName("Pastry"), QStringLiteral("A.Pastry"));
    QVERIFY(symbolTable.type("Cake"));
    QCOMPARE(symbolTable.fullyQualifiedName("Cake"), QStringLiteral("A.Cake"));
    QVERIFY(symbolTable.type("Pizza"));
    QCOMPARE(symbolTable.fullyQualifiedName("Pizza"), QStringLiteral("A.Pizza"));
    QVERIFY(!symbolTable.type("Printer"));
}

QTEST_MAIN(TestSymbolTable)
#include "testsymboltable.moc"

