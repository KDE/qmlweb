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

#ifndef SHORTSYMBOLNAME_H
#define SHORTSYMBOLNAME_H

#include <QtCore/QString>

namespace QmlJSc {

/**
 * This class represents a minified symbol name like "a", "b", "aa", "A8",...
 *
 * Initialize it with the first character you want to use as symbol name MINUS
 * ONE, as you normally will use the preincrement operator before accessing it.
 * If you won't, feel free to initialize it with the first symbol name directly,
 * of course.
 *
 * To get the next valid symbol name, use the preincrement operator.
 *
 * This is a subclass of QString, so you can just use it as a string.
 */
class ShortSymbolName : public QString
{
public:
    ShortSymbolName(char first);
    ShortSymbolName(QString first);

    ShortSymbolName &operator++();
};

} // namespace QmlJSc

#endif // SHORTSYMBOLNAME_H

