/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

// Own
#include "abstractmoduleloader.h"
#include "ir/module.h"
#include "error.h"

using namespace QmlJSc;

void AbstractModuleLoader::run()
{
    try {
        doLoad();
    } catch (Error *e) {
        qWarning() << e->what();
        m_module->setStatus(IR::Module::ErrorState);
    }
}

IR::Module *AbstractModuleLoader::module()
{
    return m_module;
}

void AbstractModuleLoader::setModule(IR::Module* module)
{
    m_module = module;
}

