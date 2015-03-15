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

#include <QtCore/QObject>
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringRef>

#include <QtCore/QDebug>
#include <qdir.h>

#include "../../../src/qmljsc/parserstage.h"

class TestParserStage : public QObject
{
    Q_OBJECT

private slots:
    void working();

};

void TestParserStage::working()
{
    QmlJSc::ParserStage stage;
    QFile minimalTestFile(":/test/minimal.qml");
    minimalTestFile.open(QFile::ReadOnly);
    QTextStream minimalTestFileStream(&minimalTestFile);
    QString minimalTestQml = minimalTestFileStream.readAll();

    QSignalSpy spy(&stage, SIGNAL(finished(QVariant)));
    stage.process(minimalTestQml);

    QCOMPARE(spy.count(), 1);

    QList<QVariant> stageResult = spy.takeFirst();

    QVERIFY2(stageResult.length() == 1, "Only one result should have been sent");

    QVariant astVariant = stageResult.takeFirst();

    QVERIFY2(astVariant.canConvert<QQmlJS::AST::UiProgram*>(), "Stage produced a wrong result");

    QQmlJS::AST::UiProgram* ast = astVariant.value<QQmlJS::AST::UiProgram*>();
    QVERIFY2(ast->headers == 0, "There should not be headers");

    QVERIFY(ast->members->kind == QQmlJS::AST::Node::Kind_UiObjectMemberList);
    QVERIFY(ast->members->member->kind == QQmlJS::AST::Node::Kind_UiObjectDefinition);

    QQmlJS::AST::UiObjectDefinition* memberObjectDefinition = reinterpret_cast<QQmlJS::AST::UiObjectDefinition*>(ast->members->member);
    QCOMPARE(QString().append(memberObjectDefinition->qualifiedTypeNameId->name), QStringLiteral("QtObject"));

}

QTEST_MAIN(TestParserStage)
#include "testparserstage.moc"
