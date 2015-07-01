/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Jan Marker <jan@jangmarker.de>
 * Copyright (C) 2015  Anton Kreuzkamp <akreuzkamp@web.de>
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

#ifndef QMLJSC_H
#define QMLJSC_H

#include <QtCore/QObject>
#include <QStringList>

namespace QmlJSc {

class SymbolTable;

#define compiler QmlJSc::Compiler::instance()

class Compiler : public QObject
{
    Q_OBJECT

public:
    explicit Compiler(QObject *parent = 0);
    virtual ~Compiler();

    static Compiler* instance() { return s_self; }

    void addIncludePath(QString path);
    const QStringList &includePaths();

private:
    static Compiler* s_self;
    QStringList m_includePaths;
};

}

#endif // QMLJSC_H
