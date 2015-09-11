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

#ifndef PIPELINE_H
#define PIPELINE_H

#include <QtCore/QObject>
#include <QtCore/QLinkedList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

#include "utils/error.h"

namespace QmlJSc {

class CompilerPass;

class CompilerPipeline : public QObject
{
 Q_OBJECT

public:
 CompilerPipeline(QObject* parent = 0);
 ~CompilerPipeline();

 /**
  * Appends @param compilerPass to the pipeline and takes the
  * ownership of @param compilerPass
  */
 void appendCompilerPass(CompilerPass *compilerPass);

 void compile(QString &file);

 QUrl file() const;

signals:
 void compileFinished(QString output);

private:
 QLinkedList<CompilerPass *> m_pipeline;
 QString m_file;
};

}

#endif // PIPELINE_H
