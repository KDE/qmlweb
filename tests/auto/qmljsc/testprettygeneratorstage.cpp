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
#include <QSignalSpy>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <qdir.h>

#include "../../../src/qmljsc/compilerpipeline.h"
#include "../../../src/qmljsc/parserstage.h"
#include "../../../src/qmljsc/prettygeneratorstage.h"
#include "../../../src/qmljsc/compiler.h"

class TestPrettyGeneratorStage : public QObject
{
    Q_OBJECT

private:

private slots:
    void initTestCase();
    void printout_data();
    void printout();

};

void TestPrettyGeneratorStage::initTestCase()
{
    new QmlJSc::Compiler();
}


void TestPrettyGeneratorStage::printout_data()
{
    QTest::addColumn<QString>("qmlFileUrl");
    QTest::addColumn<QString>("jsFileUrl");

    QTest::newRow("minimal") << ":/test/minimal.qml" << ":/test/minimal.qml.js";
}


void TestPrettyGeneratorStage::printout()
{
    QFETCH(QString, qmlFileUrl);
    QFETCH(QString, jsFileUrl);

    QFile jsFile(jsFileUrl);
    Q_ASSERT(jsFile.open(QFile::ReadOnly));
    QTextStream jsFileStream(&jsFile);
    QString jsFileContent = jsFileStream.readAll();

    QmlJSc::CompilerPipeline pipeline;
    pipeline.appendCompilerPass(new QmlJSc::ParserStage());
    pipeline.appendCompilerPass(new QmlJSc::PrettyGeneratorStage());

    QSignalSpy pipelineFinished(&pipeline, SIGNAL(compileFinished(QString)));

    pipeline.compile(qmlFileUrl);

    QCOMPARE(pipelineFinished.count(), 1);

    QString compilerOutput = pipelineFinished.takeFirst().takeFirst().value<QString>();
    QCOMPARE(compilerOutput, jsFileContent);
}

QTEST_MAIN(TestPrettyGeneratorStage)
#include "testprettygeneratorstage.moc"
