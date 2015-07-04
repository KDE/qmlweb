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

#ifndef QMLWEB_SIGNAL_H
#define QMLWEB_SIGNAL_H

#include <QVector>
#include <QString>

#include "node.h"

namespace QmlJSc {
namespace IR {

class Type;

struct Parameter {
    QmlJSc::IR::Type *type;
    QString name;
};

struct Signal : public QmlJSc::IR::Node {
    Signal();
    Signal(QString name);
    QString name;
    QVector<Parameter> parameters;

    virtual void accept(QmlJSc::IR::Visitor *visitor);
};

}
}

#endif //QMLWEB_SIGNAL_H
