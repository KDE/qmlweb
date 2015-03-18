/*
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

#include <QtTest/QTest>

#include "../../../src/qmljsc/compiler.h"

class TestCompiler
    : public QObject
{
    Q_OBJECT

private slots:
    void singleton() {
        QmlJSc::Compiler* qmljsc = new QmlJSc::Compiler();
        QCOMPARE(compiler, qmljsc);
        delete qmljsc;
    }

    void nullAfterDelete() {
        QmlJSc::Compiler* qmljsc = new QmlJSc::Compiler();
        delete qmljsc;
        QCOMPARE(compiler, static_cast<QmlJSc::Compiler*>(0));
    }

    void symbolTable() {
        QmlJSc::Compiler* qmljsc = new QmlJSc::Compiler();
        QVERIFY(qmljsc->symbols());
    }
};

QTEST_MAIN(TestCompiler)
#include "testcompiler.moc"
