/*
 * Copyright (c) 2015 Anton Kreuzkamp <akreuzkamp@web.de>
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *      * Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER “AS IS” AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */

QWQmlEngine.prototype.baseUrl = "";

function QWQmlEngine()
{

}

/**
 * Import QML module.
 */
QWQmlEngine.prototype.importModule = function(name, versionMajor, versionMinor)
{
    var modulePrefix = name + versionMajor + versionMinor;
    if (modulePrefix in QWQmlEngine.__loadedModules)
        return QWQmlEngine.__loadedModules[modulePrefix];

    var module = qw_evalJS(qw_fetchData([this.baseUrl + name + "." + versionMajor + "." + versionMinor + ".js"]), this);
    QWQmlEngine.__loadedModules[modulePrefix] = module;
    return module;
}

/**
 * Loads the component files given and adds them to local namespace.
 */
QWQmlEngine.prototype.loadLocal = function(urls)
{
    var namespace = {};
    for (var i in urls) {
        var url = urls[i];
        var componentName = url.slice(0, -4);
        if (componentName in QWQmlEngine.__loadedFiles) {
            namespace[componentName] = QWQmlEngine.__loadedFiles[componentName];
            continue;
        }

        var component = qw_evalJS(qw_fetchData([this.baseUrl + url + ".js"]), this);
        QWQmlEngine.__loadedFiles[componentName] = component;
        namespace[componentName] = component;
    }
    return namespace;
}

QWQmlEngine.__loadedModules = {}; // static
QWQmlEngine.__loadedFiles = {}; // static

QWQmlEngine.contextForObject = function(object) { // static
    if (object.__ctx) // If the object is the root of a component, we want to
        return object.__ctx.__parentContext || object.__ctx; // return the outer context.

    while (!object.__ctx)
        object = object.__parent;

    return object.__ctx;
}
