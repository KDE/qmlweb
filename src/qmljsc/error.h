/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#ifndef ERROR_H
#define ERROR_H

#include <QtCore/QUrl>
#include <QtCore/QString>

namespace QmlJSc {

class Error
{
public:
Error() {};

 QUrl file() { return m_file; }
 void setFile(QUrl file) { m_file = file; }

 QString description() { return m_description; }
 void setDescription(QString description) { m_description = description; }

 int column() { return m_column; }
 void setColumn(int column) { m_column = column; }

 int line() { return m_line; }
 void setLine(int line) { m_line = line; }

private:
    QUrl m_file;
    QString m_description;
    int m_column;
    int m_line;
};

}

Q_DECLARE_METATYPE(QmlJSc::Error);

#endif // ERROR_H
