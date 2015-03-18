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

#include "symboltable.h"

#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtQml/QJSEngine>
#include <QtQml/QJSValue>
#include <QtQml/QJSValueIterator>

// Qt private
#include <QtQml/private/qv8engine_p.h>
#include <QtQml/private/qv4engine_p.h>
#include <QtQml/private/qv4context_p.h>
#include <QtQml/private/qjsvalue_p.h>

using namespace QmlJSc;

static QV4::ReturnedValue NOOP(QV4::CallContext *)
{
}

SymbolTable::SymbolTable(QObject* parent)
    : QObject(parent)
    , m_jsEngine(new QJSEngine(this))
{
    ModuleData *builtInModule = new ModuleData;
    builtInModule->typeToFQIHash.insert("Component", "QtQml20.Component");
    builtInModule->typeToFQIHash.insert("QtObject", "QtQml20.QtObject");
    builtInModule->typeToFQIHash.insert("Binding", "QtQml20.Binding");
    builtInModule->typeToFQIHash.insert("Connections", "QtQml20.Connections");
    builtInModule->typeToFQIHash.insert("Timer", "QtQml20.Timer");
    builtInModule->loaded = true;
    m_modules.insert({"QtQml", 2, 0}, builtInModule);


    // Inject methods into the js execution environment that we use to find out about the module API
    QV4::ExecutionEngine *v4 = QV8Engine::getV4(m_jsEngine);
    QV4::Scope scope(v4);
    QV4::ScopedObject obj(scope, v4->globalObject);

    // QmlJS Runtime macros
    obj->defineDefaultProperty("QW_INHERIT", NOOP);

    // QML built in types
    obj->defineDefaultProperty("QWObject", NOOP);
}

SymbolTable::~SymbolTable()
{
    foreach (ModuleData *module, m_modules) {
        delete module;
    }
}

void SymbolTable::addIncludePath(QString path)
{
    m_includePaths << path;
}

void SymbolTable::loadModule(ModuleImport import)
{
    if (m_modules.contains(import))
        return;

    m_modules.insert(import, new ModuleData);

    QMetaObject::invokeMethod(this, "doLoadModule", Q_ARG(ModuleImport, import));
}

QString SymbolTable::findType(ModuleImport module, QString typeName)
{
    if (!m_modules.contains(module))
        return QString();

    ModuleData *data = m_modules.value(module);
    if (!data->loaded) { // Wait for data to be available.
        data->loadMutex.lock();
        data->waitForLoaded.wait(&data->loadMutex);
        data->loadMutex.unlock();

        if (!m_modules.contains(module)) // If an error occurred, the module won't exist anymore
            return QString();
    }

    return data->typeToFQIHash.value(typeName);
}

QString SymbolTable::findType(ModuleImports modules, QString typeName)
{
    if (!modules.contains({"QtQml", 2, 0})) {
        modules.append({"QtQml", 2, 0});
    }
    foreach (ModuleImport module, modules) {
        QString result = findType(module, typeName);
        if (!result.isEmpty()) {
            return result;
        }
    }
    return QString();
}

void SymbolTable::doLoadModule(ModuleImport import)
{
    QFile moduleFile;
    const QString moduleFileName = QStringLiteral("%1.%2.%3.js").arg(import.name)
                                                                .arg(import.versionMajor)
                                                                .arg(import.versionMinor);

    // For now we only support local files.
    foreach (QString includePath, m_includePaths) {
        QDir includeDir(includePath);
        if (includeDir.exists(moduleFileName)) {
            moduleFile.setFileName(includeDir.absoluteFilePath(moduleFileName));
            break;
        }
    }

    if (!moduleFile.exists()) {
        error(import, QStringLiteral("Could not find file %1 in path.").arg(moduleFileName));
        return;
    }

    ModuleData *moduleData = m_modules[import];

    // Read file
    moduleFile.open(QFile::ReadOnly);
    QTextStream modueFileStream(&moduleFile);
    QString moduleSource = modueFileStream.readAll();

    // Process the module file using a QJSEngine and filter out all required information
    QJSValue result = m_jsEngine->evaluate(moduleSource, moduleFileName);
    QJSValueIterator it(result);

    if (result.isError()) {
        Error *err = new Error(Error::JSEngineError, result.property("message").toString());
        err->setFile(result.property("fileName").toString());
        err->setLine(result.property("lineNumber").toInt());
        error(import, QStringLiteral("Error while processing module %1 %2.%3")
                        .arg(import.name).arg(import.versionMajor).arg(import.versionMinor), err);
        return;
    }

    // Iterate over all types and add their Fully Qualified Identifier to the module.
    while(it.hasNext()) {
        it.next();
        QString fqi = QStringLiteral("%1%2%3.%4").arg(import.name)
                                                 .arg(import.versionMajor)
                                                 .arg(import.versionMinor)
                                                 .arg(it.name());
        moduleData->typeToFQIHash.insert(it.name(), fqi);
    }

    moduleData->loaded = true;
    moduleData->waitForLoaded.wakeAll(); // Wake up threads, that wait to access this module.
}

void SymbolTable::error(ModuleImport import, QString message, Error *reason)
{
    m_modules[import]->waitForLoaded.wakeAll();
    m_modules.remove(import);
    emit importError({ Error::ModuleImportError, message, reason });
}
