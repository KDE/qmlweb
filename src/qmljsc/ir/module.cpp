/*
 * Qml.js Compiler -  a QML to JS compiler bringing QML's power to the web.
 *
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
#include "module.h"
#include "../compiler.h"
#include "../utils/error.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QThread>
#include <QtCore/QThreadPool>

// Qt private
#include <private/qqmljsengine_p.h>
#include <private/qqmljslexer_p.h>
#include <private/qqmljsparser_p.h>

using namespace QmlJSc;
using namespace QmlJSc::IR;
using namespace QQmlJS::AST;

Module::Module(ImportDescription import, QObject *parent)
    : m_importDescription(import)
    , m_loadingState(Loading)
{
}

Module::~Module()
{
    foreach (Type *type, m_types) {
        delete type;
    }
}

Module::LoadingState Module::loadingState()
{
    return m_loadingState;
}

void Module::setLoadingState(Module::LoadingState status)
{
    Q_ASSERT_X(m_loadingState == Loading, __FILE__,
               "It's not allowed to change status after loading finished.");

    m_loadingState = status;
    if (m_loadingState == Successful || m_loadingState == ErrorState) {
        m_loadFinishedCondition.wakeAll();
    }
}


const QString &Module::name()
{
    return m_importDescription.name;
}

Type *Module::type(QString name)
{
    waitForLoaded();
    return m_types.value(name);
}

Type *Module::typeFromJSName(QString name)
{
    return m_jsNameToTypeHash.value(name);
}

void Module::addType(Type *type)
{
    Q_ASSERT_X(m_loadingState == Loading, __FILE__,
               "It's not allowed to add types after loading finished.");

    m_types.insert(type->name(), type);
    m_jsNameToTypeHash.insert(type->javaScriptName(), type);
}

void Module::waitForLoaded()
{
    if (m_loadingState != Loading)
        return;

    m_loadMutex.lock();
    m_loadFinishedCondition.wait(&m_loadMutex);
    m_loadMutex.unlock();
}

ImportDescription Module::importDescription()
{
    return m_importDescription;
}
