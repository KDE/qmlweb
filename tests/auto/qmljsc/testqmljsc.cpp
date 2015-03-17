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

#include "../../../src/qmljsc/qmljsc.h"

class TestQmlJSc
    : public QObject
{
    Q_OBJECT

private slots:
    void singleton() {
        QmlJSc::QmlJSc* qmljsc = new QmlJSc::QmlJSc();
        QCOMPARE(QmlJSc::qmlJSc, qmljsc);
        delete qmljsc;
    }

    void nullAfterDelete() {
        QmlJSc::QmlJSc* qmljsc = new QmlJSc::QmlJSc();
        delete qmljsc;
        QCOMPARE(QmlJSc::qmlJSc, static_cast<QmlJSc::QmlJSc*>(0));
    }
};

QTEST_MAIN(TestQmlJSc)
#include "testqmljsc.moc"
