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

#include "shortsymbolname.h"

using namespace QmlJSc;

ShortSymbolName::ShortSymbolName(char first)
    : QString(first)
{}

ShortSymbolName::ShortSymbolName(QString first)
    : QString(first)
{}

ShortSymbolName &ShortSymbolName::operator++()
{
    Q_ASSERT(!isEmpty());

    iterator i = end();
    i--;

    char borrow = 1;

    while (i != begin()) {
        if ((*i) == '9') {
            *i = 'A';
            borrow = 0;
            break;
        } else if ((*i) == 'Z') {
            *i = 'a';
            borrow = 0;
            break;
        } else if ((*i) == 'z') {
            // We need to add a borrow of 1 to the previous digit
            *i = '0';
            i--;
            borrow = 1;
            continue;
        } else {
            *i = i->toLatin1() + 1;
            borrow = 0;
            break;
        }
    }

    if (borrow == 1) {
        if (*i <= 'Z') { // the first letter is a capital one, so it should remain so.
            if (*i == 'Z') { // We need to prepend a new digit
                *i = '0';
                prepend('A');
            } else {
                *i = i->toLatin1() + 1;
            }
        } else { // the first letter is a small one, so it should remain so.
            if (*i == 'z') { // We need to prepend a new digit
                *i = '0';
                prepend('a');
            } else {
                *i = i->toLatin1() + 1;
            }
        }
    }

    return *this;
}
