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

#include "../../../src/qmljsc/compilerpipeline.h"
#include "../../../src/qmljsc/compilerpasses/parserpass.h"

class TestParserPass : public QObject
{
    Q_OBJECT

private slots:
    void working();
    void not_working();

};

void TestParserPass::working()
{
    QmlJSc::CompilerPipeline pipeline;

    QmlJSc::ParserPass *parserPass = new QmlJSc::ParserPass();
    pipeline.appendCompilerPass(parserPass);

    QFile minimalTestFile(":/test/minimal.qml");
    minimalTestFile.open(QFile::ReadOnly);
    QTextStream minimalTestFileStream(&minimalTestFile);
    QString minimalTestQml = minimalTestFileStream.readAll();

    QSignalSpy spy(parserPass, SIGNAL(finished(QQmlJS::AST::UiProgram*)));

    try {
        parserPass->process(minimalTestQml);
    } catch (QmlJSc::Error& error) {
        QFAIL("no error should occur");
    }

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

void TestParserPass::not_working()
{
    QmlJSc::CompilerPipeline pipeline;

    QmlJSc::ParserPass *parserPass = new QmlJSc::ParserPass();
    pipeline.appendCompilerPass(parserPass);
    QString invalidQml = "not valid QML";

    QSignalSpy spy(parserPass, SIGNAL(finished(QQmlJS::AST::UiProgram*)));

    try {
        parserPass->process(invalidQml);
        QFAIL("an parse error should have been thrown");
    } catch (QmlJSc::Error& error) {
        QCOMPARE(error.type(), QmlJSc::Error::ParseError);
    }

    QCOMPARE(spy.count(), 0);
}

QTEST_MAIN(TestParserPass)
#include "testparserpass.moc"
