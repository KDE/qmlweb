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

#include "../../../src/qmljsc/purejavascriptgenerator.h"

#include <QtCore/QMetaType>
#include <QtTest/QTest>
#include <QtCore/QFile>

#include <private/qqmljsast_p.h>
#include <private/qqmljsengine_p.h>
#include <private/qqmljslexer_p.h>
#include <private/qqmljsparser_p.h>

Q_DECLARE_METATYPE(QQmlJS::AST::Node*)

class TestPureJavaScriptGeneratorIntegration
    : public QObject
{
    Q_OBJECT

private:
    QQmlJS::AST::Node* astForFile(QString fileName) {
        QString input = fileContent(fileName);

        QQmlJS::Engine* engine = new QQmlJS::Engine();
        QQmlJS::Lexer* lexer = new QQmlJS::Lexer(engine);

        lexer->setCode(input, 1, true);

        QQmlJS::Parser* parser = new QQmlJS::Parser(engine);

        if (!parser->parseProgram()) {
            qDebug() << parser->errorMessage();
        }

        return parser->rootNode();
    }

    QString fileContent(QString fileName) {
        QFile inputFile(fileName);
        Q_ASSERT_X(inputFile.open(QFile::ReadOnly), __FILE__, "File could not be opened. Is it added as resource in the CMakeLists.txt?");
        return QString(inputFile.readAll());
    }

    void addRowForFile(QString fileName) {
        addRowForFileWithCompiled(fileName, fileName);
    }

    void addRowForFileWithCompiled(QString sourceFileName, QString compiledFileName) {
        const QString folder(":/test/%1.js");
        QTest::newRow(sourceFileName.toLocal8Bit()) << astForFile(folder.arg(sourceFileName)) << fileContent(folder.arg(compiledFileName));

    }

    void addRowForFileWithCompiled(QString fileName) {
        addRowForFileWithCompiled(fileName, QString("%1.compiled").arg(fileName));
    }

private slots:

    void test_compileJavaScriptFile_data() {
        QTest::addColumn<QQmlJS::AST::Node*>("ast");
        QTest::addColumn<QString>("expectedOutput");

        addRowForFileWithCompiled("declarations");
        addRowForFile("expressions");
        addRowForFileWithCompiled("functions");
        addRowForFile("unaryoperators");
        addRowForFileWithCompiled("binaryoperations");
        addRowForFileWithCompiled("otherstatements");
        addRowForFileWithCompiled("ifstatements");
        addRowForFileWithCompiled("switchstatements");
        addRowForFileWithCompiled("literals");
        addRowForFileWithCompiled("arrayliterals");
        addRowForFileWithCompiled("objectliterals");
        addRowForFileWithCompiled("lefthandexpressions");
        addRowForFileWithCompiled("functioncalls");
        addRowForFileWithCompiled("loops");
        addRowForFileWithCompiled("exceptionstatements");
    }

    void test_compileJavaScriptFile() {
        // Prepare
        QFETCH(QQmlJS::AST::Node*, ast);
        QFETCH(QString, expectedOutput);

        PureJavaScriptGenerator generator;

        // Do
        ast->accept(&generator);

        // Verify
        QCOMPARE(generator.getGeneratedCode(), expectedOutput);
    }



};

QTEST_MAIN(TestPureJavaScriptGeneratorIntegration)
#include "testpurejavascriptgenerator_integration.moc"
