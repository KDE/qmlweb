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

#ifndef PIPELINESTAGE_H
#define PIPELINESTAGE_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "utils/error.h"

#include "ir/file.h"
#include <private/qqmljsast_p.h>

Q_DECLARE_METATYPE(QQmlJS::AST::UiProgram*)

namespace QmlJSc {

class CompilerPipeline;

class CompilerPass : public QObject
{
  Q_OBJECT

public:
  CompilerPass(QObject *parent = 0);

  void connectToSuccessor(CompilerPass *successor);

public slots:
  virtual void process(QString);
  virtual void process(QQmlJS::AST::UiProgram*);
  virtual void process(IR::File*);

signals:
  void finished(QString);
  void finished(QQmlJS::AST::UiProgram*);
  void finished(IR::File*);

private:
  void failBecauseOfWrongType();
};

}

#endif // PIPELINESTAGE_H
