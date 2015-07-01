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

#ifndef ERROR_H
#define ERROR_H

#include <QtCore/QMetaType>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>
#include <QtCore/QDebug>

#include <exception>

namespace QmlJSc {

class Error : public std::exception
{
public:
    enum Type {
        UnknownError,
        ReadFileError,
        ParseError,
        ModuleImportError,
        SymbolLookupError
    };

    Error()
        : m_type(UnknownError)
        , m_column(-1)
        , m_line(-1)
        , m_reason(0)
    {}
    Error(Type type, QString description, Error *reason = 0)
        : m_type(type)
        , m_description(description)
        , m_column(-1)
        , m_line(-1)
        , m_reason(reason)
    {}
    virtual ~Error()
    {}

    Type type() const { return m_type; }
    void setType(Type type) { m_type = type; }

    QUrl file() const { return m_file; }
    void setFile(QUrl file) { m_file = file; }

    QString description() const { return m_description; }
    void setDescription(QString description) { m_description = description; }

    int column() const { return m_column; }
    void setColumn(int column) { m_column = column; }

    int line() const { return m_line; }
    void setLine(int line) { m_line = line; }

    Error *reason() const { return m_reason.data(); }
    void setReason(Error *reason) { m_reason.reset(reason); }

    virtual const char* what() const noexcept;

private:
    Type m_type;
    QUrl m_file;
    QString m_description;
    int m_column;
    int m_line;
    QSharedPointer<Error> m_reason;
};

}

QDebug operator<<(QDebug dbg, const QmlJSc::Error &error);

Q_DECLARE_METATYPE(QmlJSc::Error);

#endif // ERROR_H
