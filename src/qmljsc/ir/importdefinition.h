/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
 * Copyright (C) 2015 Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

#ifndef IMPORTDESCRIPTION_H
#define IMPORTDESCRIPTION_H

#include <QtCore/QMetaType>
#include <QtCore/QString>

namespace QmlJSc {
namespace IR {

struct ImportDescription {
    enum Kind {
        Kind_ModuleImport,
        Kind_FileImport,
        Kind_DirectoryImport
    };

    Kind kind;
    QString name;
    int versionMajor;
    int versionMinor;

    inline bool operator==(const ImportDescription& other) const
    {
        return name == other.name && versionMajor == other.versionMajor && versionMinor == other.versionMinor;
    }
};

inline uint qHash(const ImportDescription &key, uint seed)
{
    return qHash(key.name, seed) ^ key.versionMajor ^ key.versionMinor;
}

} // namespace IR
} // namespace QmlJSc

Q_DECLARE_METATYPE(QmlJSc::IR::ImportDescription);

#endif // IMPORTDESCRIPTION_H

