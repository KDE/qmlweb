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

#include "parserstage.h"

using namespace QmlJSc;

ParserStage::ParserStage()
{
}

ParserStage::~ParserStage()
{

}

void ParserStage::process(QVariant input)
{
    Q_ASSERT(input.canConvert<QString>());

    QString inputString = input.value<QString>();

    QQmlJS::Engine* engine = new QQmlJS::Engine();
    QQmlJS::Lexer* lexer = new QQmlJS::Lexer(engine);

    lexer->setCode(inputString, 1, true);

    QQmlJS::Parser* parser = new QQmlJS::Parser(engine);

    bool successfullyParsed = parser->parse();
    if (!successfullyParsed) {
        Error error(Error::ParseError, parser->errorMessage());
        error.setColumn(parser->errorColumnNumber());
        error.setLine(parser->errorLineNumber());
        emit errorOccurred(error);
        return;
    }

    QVariant ast = QVariant::fromValue(parser->ast());

    emit finished(ast);
}
