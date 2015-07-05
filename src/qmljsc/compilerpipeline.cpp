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

#include <QtCore/QFile>

#include "compilerpass.h"

#include "compilerpipeline.h"

using namespace QmlJSc;

CompilerPipeline::CompilerPipeline()
{

}

CompilerPipeline::~CompilerPipeline()
{
    foreach (CompilerPass* compilerPass, m_pipeline) {
        delete compilerPass;
    }
}
QUrl CompilerPipeline::file() const
{
    return m_file;
}

void CompilerPipeline::compile(QString& filePath)
{
    m_file = filePath;

    QFile file(m_file);
    bool openSuccessful = file.open(QFile::ReadOnly);
    if (!openSuccessful) {
        Error error(Error::ReadFileError, file.errorString());
        error.setFile(file.fileName());
        throw error;
    }
    QTextStream stream(&file);
    QString qmlCode = stream.readAll();

    Q_ASSERT(m_pipeline.count() > 0);

    m_pipeline.first()->process(qmlCode);
}

void CompilerPipeline::appendCompilerPass(CompilerPass *stage)
{
    Q_ASSERT(stage);

    CompilerPass * lastStage = 0;
    if (m_pipeline.size() > 0) {
        lastStage = m_pipeline.last();
        disconnect(lastStage, SIGNAL(finished(QString)), this, SIGNAL(compileFinished(QString)));
    }

    m_pipeline.append(stage);

    if (lastStage) {
        lastStage->connectToSuccessor(stage);
    }

    connect(stage, SIGNAL(finished(QString)), this, SIGNAL(compileFinished(QString)));
}
