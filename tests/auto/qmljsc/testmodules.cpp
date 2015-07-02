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
#include "../../../src/qmljsc/moduleloader.h"
#include "../../../src/qmljsc/ir/file.h"

class TestSymbolTable : public QObject
{
    Q_OBJECT

private slots:
    void loadModule();
    void testShortSymbolName();

};

using namespace QmlJSc;
using namespace QmlJSc::IR;

void TestSymbolTable::loadModule()
{
    Compiler c;

    IR::File file;
    const ImportDescription testImportDescription = {ImportDescription::Kind_ModuleImport, "TestModule", 0, 1};

    compiler->addIncludePath(":/test/");
    file.addModule(ModuleLoader::loadModule(testImportDescription));

    QVERIFY(file.type("Pastry"));
    QCOMPARE(file.fullyQualifiedName("Pastry"), QStringLiteral("A.Pastry"));
    QVERIFY(file.type("Cake"));
    QCOMPARE(file.fullyQualifiedName("Cake"), QStringLiteral("A.Cake"));
    QVERIFY(file.type("Pizza"));
    QCOMPARE(file.fullyQualifiedName("Pizza"), QStringLiteral("A.Pizza"));
    QVERIFY(!file.type("Printer"));
}

void TestSymbolTable::testShortSymbolName()
{
    ShortSymbolName nA('A');
    ShortSymbolName nB('B');
    ShortSymbolName nZ('Z');
    ShortSymbolName na('a');
    ShortSymbolName nb('b');
    ShortSymbolName nz('z');

    ShortSymbolName nA0("A0");
    ShortSymbolName nA1("A1");
    ShortSymbolName nA9("A9");
    ShortSymbolName nAA("AA");
    ShortSymbolName nAB("AB");
    ShortSymbolName nAZ("AZ");
    ShortSymbolName nAa("Aa");
    ShortSymbolName nAb("Ab");
    ShortSymbolName nAz("Az");

    ShortSymbolName na0("a0");
    ShortSymbolName na1("a1");
    ShortSymbolName na9("a9");
    ShortSymbolName naA("aA");
    ShortSymbolName naB("aB");
    ShortSymbolName naZ("aZ");
    ShortSymbolName naa("aa");
    ShortSymbolName nab("ab");
    ShortSymbolName naz("az");

    ShortSymbolName nZ9("Z9");
    ShortSymbolName nZZ("ZZ");
    ShortSymbolName nZz("Zz");
    ShortSymbolName nz9("z9");
    ShortSymbolName nzZ("zZ");
    ShortSymbolName nzz("zz");


    ShortSymbolName nA5dh("A5dh");
    ShortSymbolName nA5dz("A5dz");
    ShortSymbolName nA5zz("A5zz");
    ShortSymbolName nAzzz("Azzz");
    ShortSymbolName nZzzz("Zzzz");
    ShortSymbolName nzzzz("zzzz");

    QCOMPARE(static_cast<QString>(++nA), QStringLiteral("B"));
    QCOMPARE(static_cast<QString>(++nB), QStringLiteral("C"));
    QCOMPARE(static_cast<QString>(++nZ), QStringLiteral("A0"));
    QCOMPARE(static_cast<QString>(++na), QStringLiteral("b"));
    QCOMPARE(static_cast<QString>(++nb), QStringLiteral("c"));
    QCOMPARE(static_cast<QString>(++nz), QStringLiteral("a0"));

    QCOMPARE(static_cast<QString>(++nA0), QStringLiteral("A1"));
    QCOMPARE(static_cast<QString>(++nA1), QStringLiteral("A2"));
    QCOMPARE(static_cast<QString>(++nA9), QStringLiteral("AA"));
    QCOMPARE(static_cast<QString>(++nAA), QStringLiteral("AB"));
    QCOMPARE(static_cast<QString>(++nAB), QStringLiteral("AC"));
    QCOMPARE(static_cast<QString>(++nAZ), QStringLiteral("Aa"));
    QCOMPARE(static_cast<QString>(++nAa), QStringLiteral("Ab"));
    QCOMPARE(static_cast<QString>(++nAb), QStringLiteral("Ac"));
    QCOMPARE(static_cast<QString>(++nAz), QStringLiteral("B0"));

    QCOMPARE(static_cast<QString>(++na0), QStringLiteral("a1"));
    QCOMPARE(static_cast<QString>(++na1), QStringLiteral("a2"));
    QCOMPARE(static_cast<QString>(++na9), QStringLiteral("aA"));
    QCOMPARE(static_cast<QString>(++naA), QStringLiteral("aB"));
    QCOMPARE(static_cast<QString>(++naB), QStringLiteral("aC"));
    QCOMPARE(static_cast<QString>(++naZ), QStringLiteral("aa"));
    QCOMPARE(static_cast<QString>(++naa), QStringLiteral("ab"));
    QCOMPARE(static_cast<QString>(++nab), QStringLiteral("ac"));
    QCOMPARE(static_cast<QString>(++naz), QStringLiteral("b0"));

    QCOMPARE(static_cast<QString>(++nZ9), QStringLiteral("ZA"));
    QCOMPARE(static_cast<QString>(++nZZ), QStringLiteral("Za"));
    QCOMPARE(static_cast<QString>(++nZz), QStringLiteral("A00"));
    QCOMPARE(static_cast<QString>(++nz9), QStringLiteral("zA"));
    QCOMPARE(static_cast<QString>(++nzZ), QStringLiteral("za"));
    QCOMPARE(static_cast<QString>(++nzz), QStringLiteral("a00"));

    QCOMPARE(static_cast<QString>(++nA5dh), QStringLiteral("A5di"));
    QCOMPARE(static_cast<QString>(++nA5dz), QStringLiteral("A5e0"));
    QCOMPARE(static_cast<QString>(++nA5zz), QStringLiteral("A600"));
    QCOMPARE(static_cast<QString>(++nAzzz), QStringLiteral("B000"));
    QCOMPARE(static_cast<QString>(++nZzzz), QStringLiteral("A0000"));
    QCOMPARE(static_cast<QString>(++nzzzz), QStringLiteral("a0000"));
}

QTEST_MAIN(TestSymbolTable)
#include "testmodules.moc"

