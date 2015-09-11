/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
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

#include <QtCore/QObject>
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QFile>

#include "../../../src/qmljsc/ir/objecttree.h"
#include "../../../src/qmljsc/compiler.h"
#include "../../../src/qmljsc/compilerpasses/prettygeneratorpass.h"

template<class TestedVisitorNodeType>
class TestNodeFile : public QmlJSc::IR::File
{
public:
    TestNodeFile(TestedVisitorNodeType& testedNode)
            : m_testedNode(testedNode)
    {
    }

    virtual void accept(QmlJSc::IR::Visitor* visitor) override {
        m_testedNode.accept(visitor);
    }

private:
    TestedVisitorNodeType& m_testedNode;
};

class TestPrettyGeneratorPass : public QObject
{
    Q_OBJECT

private:
    void initBasicTypes();
    QString readTestFileContent(const char* fileName);

    QmlJSc::PrettyGeneratorPass* m_prettyGeneratorPass = Q_NULLPTR;
    QString m_result;
    QmlJSc::IR::Type* m_qtObjectType = Q_NULLPTR;

private slots:
    void setResult(QString);

private slots:
    void initTestCase();
    void init();

    void emitsFinishedSignal();
    void visitFile();

};

void TestPrettyGeneratorPass::setResult(QString result) {
    if (!m_result.isEmpty()) {
        QFAIL("finished emitted multiple times");
    } else {
        m_result = result;
    }
}

void TestPrettyGeneratorPass::initTestCase()
{
    initBasicTypes();
}

void TestPrettyGeneratorPass::initBasicTypes() {
    QmlJSc::IR::Type* stringType = new QmlJSc::IR::Type();
    stringType->setName("string");

    m_qtObjectType = new QmlJSc::IR::Type();
    m_qtObjectType->setName("QtObject");
    QmlJSc::IR::Property *p = m_qtObjectType->addProperty("objectName");
    p->type = stringType;
}

QString TestPrettyGeneratorPass::readTestFileContent(const char *fileName) {
    QFile testFile( QString(":/test/%1").arg(fileName) );
    Q_ASSERT(testFile.open(QFile::ReadOnly));
    QTextStream input(&testFile);
    return input.readAll();
}


void TestPrettyGeneratorPass::init()
{
    if (m_prettyGeneratorPass) {
        disconnect(m_prettyGeneratorPass, SIGNAL(finished(QString)), this, SLOT(setResult(QString)));
        delete m_prettyGeneratorPass;
    }

    m_prettyGeneratorPass = new QmlJSc::PrettyGeneratorPass();

    m_result.clear();
    connect(m_prettyGeneratorPass, SIGNAL(finished(QString)), this, SLOT(setResult(QString)));
}

void TestPrettyGeneratorPass::emitsFinishedSignal() {
    // Setup
    QmlJSc::IR::File file;
    file.setRootObject(new QmlJSc::IR::Object);
    file.rootObject()->setSuper(m_qtObjectType);
    TestNodeFile<QmlJSc::IR::File> testNodeFile(file);

    QSignalSpy finishedSpy(m_prettyGeneratorPass, SIGNAL(finished(QString)));

    // Do
    m_prettyGeneratorPass->process(&testNodeFile);

    // Verify
    QCOMPARE(finishedSpy.count(), 1);
}

void TestPrettyGeneratorPass::visitFile()
{
    // Setup
    QString minimalFileJs = readTestFileContent("minimal.qml.js");

    QmlJSc::IR::File file;
    file.setRootObject(new QmlJSc::IR::Object);
    file.rootObject()->setSuper(m_qtObjectType);

    TestNodeFile<QmlJSc::IR::File> testNodeFile(file);

    // Do
    m_prettyGeneratorPass->process(&testNodeFile);

    // Verify
    QCOMPARE(m_result, minimalFileJs);
}

QTEST_MAIN(TestPrettyGeneratorPass)
#include "testprettygeneratorpass.moc"