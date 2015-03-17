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
#include "QtTest/QSignalSpy"

#include <QtCore/QDebug>
#include <qdir.h>

#include "../../../src/qmljsc/pipeline.h"
#include "../../../src/qmljsc/pipelinestage.h"

class MockStage : public QmlJSc::PipelineStage {
    Q_OBJECT
    public:
        void setDoError(bool doError) { m_doError = doError; }
        bool doError() { return m_doError; }

    public slots:
        void process(QVariant input) {
            if (doError()) {
                QmlJSc::Error error;
                error.setColumn(1);
                error.setLine(1);
                error.setDescription("description");
                error.setFile(QUrl("qrc:/test/minimal.qml"));
                emit errorOccurred(error);
                return;
            }
            emit finished(input);
        }

    private:
        bool m_doError;
};

class TestPipeline : public QObject
{
    Q_OBJECT

public: TestPipeline();

private:
    MockStage *m_mockStage;
    QmlJSc::Pipeline* m_pipeline;

private slots:
    void initTestCase();
    void working();
    void error();
    void cleanupTestCase();

};

TestPipeline::TestPipeline()
    : QObject()
{
}


void TestPipeline::initTestCase()
{
    m_pipeline = new QmlJSc::Pipeline();
    m_mockStage = new MockStage();

    m_pipeline->appendStage(m_mockStage);
}

void TestPipeline::cleanupTestCase()
{
    delete m_pipeline;
    delete m_mockStage;
}

void TestPipeline::working()
{
    m_mockStage->setDoError(false);

    QSignalSpy finishedSpy(m_pipeline, SIGNAL(compileFinished(QVariant)));

    QString filePath(":/test/minimal.qml");
    QFile file(filePath);
    Q_ASSERT(file.open(QFile::ReadOnly));
    QTextStream stream(&file);
    QString fileContent = stream.readAll();

    m_pipeline->compile(filePath);

    QCOMPARE(finishedSpy.count(), 1);
    QCOMPARE(finishedSpy.takeFirst().takeFirst().value<QString>(), fileContent);
}

void TestPipeline::error()
{
    m_mockStage->setDoError(true);

    QSignalSpy finishedSpy(m_pipeline, &QmlJSc::Pipeline::compileFinished);
    QSignalSpy errorSpy(m_pipeline, &QmlJSc::Pipeline::errorOccurred);

    QString filePath(":/test/minimal.qml");
    m_pipeline->compile(filePath);

    QCOMPARE(finishedSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);

    QmlJSc::Error error = errorSpy.takeFirst().takeFirst().value<QmlJSc::Error>();

    QCOMPARE(error.column(), 1);
    QCOMPARE(error.line(), 1);
    QCOMPARE(error.description(), QStringLiteral("description"));
    QCOMPARE(error.file(), QUrl("qrc:/test/minimal.qml"));

}


QTEST_MAIN(TestPipeline)
#include "testpipeline.moc"
