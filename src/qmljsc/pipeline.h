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

#ifndef PIPELINE_H
#define PIPELINE_H

#include <QtCore/QObject>
#include <QtCore/QLinkedList>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

#include "error.h"

namespace QmlJSc {

class PipelineStage;

class Pipeline : public QObject
{
 Q_OBJECT

public:
 Pipeline();
 ~Pipeline();

 void appendStage(PipelineStage *stage);

 void compile(QString &file);

 QUrl file() const;

signals:
 void compileFinished(QVariant output);
 void errorOccurred(QmlJSc::Error error);

private:
 QLinkedList<PipelineStage*> m_pipeline;
 QString m_file;
};

}

#endif // PIPELINE_H
