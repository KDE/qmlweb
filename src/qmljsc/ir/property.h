/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015 Anton Kreuzkamp <akreuzkamp@web.de>
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

#ifndef QMLWEB_PROPERTY_H
#define QMLWEB_PROPERTY_H

#include <QString>

#include "node.h"

namespace QQmlJS {
    namespace AST {
        class ExpressionNode;
    }
}

namespace QmlJSc {
namespace IR {

class Object;
class Type;

struct Property : public QmlJSc::IR::Node {
    Property();
    Property(const QString &name);
    Property(QmlJSc::IR::Type *type, QString name);

    QmlJSc::IR::Type *type;
    QString name;

    QmlJSc::IR::Object *objectValue;
    QQmlJS::AST::ExpressionNode *jsValue;
    bool readOnly :1;
    bool constant :1;
    bool dummy :6;

    virtual void accept(QmlJSc::IR::Visitor *visitor) override;
};

}
}

#endif //QMLWEB_PROPERTY_H
