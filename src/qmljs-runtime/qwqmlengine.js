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
    this.__pendingModule = null;
}

/**
 * Import QML module.
 */
QWQmlEngine.prototype.importModule = function(name, versionMajor, versionMinor)
{
    var modulePrefix = name + versionMajor + versionMinor;
    if (modulePrefix in QWQmlEngine.__loadedModules)
        return QWQmlEngine.__loadedModules[modulePrefix];

    qw_evalJS(qw_fetchData([this.baseUrl + name + "." + versionMajor + "." + versionMinor + ".js"]), this);
    if (!this.__pendingModule) {
        throw "Can't load module: No valid module registered.";
    }
    QWQmlEngine.__loadedModules[modulePrefix] = this.__pendingModule;
    this.__pendingModule = null;
    return QWQmlEngine.__loadedModules[modulePrefix];
}

QWQmlEngine.prototype.registerModule = function(module)
{
    if (this.__pendingModule) {
        throw "Can't load module: A module may call registerModule only once.";
    }
    this.__pendingModule = module;
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

/**
 * Fetches JavaScript resource files and returns the code.
 * This function does not evaluate the code, as it needs to be evaluated in the
 * same context as the importing qml file itself.
 *
 * This function is for code-behind implementation resources.
 */
QWQmlEngine.prototype.fetchJSResource = function(url)
{
    var resourceName = url.split('/')[0]; // FIXME: use the last, instead of the first part.
    if (resourceName in QWQmlEngine.__loadedJSResources) {
        return QWQmlEngine.__loadedJSResources[resourceName];
    }

    var resource = qw_fetchData([this.baseUrl + url]);
    QWQmlEngine.__loadedFiles[resourceName] = resource;
    return resource;
}

/**
 * Fetches a JavaScript resource file and evaluates it.
 *
 * This function is for shared JavaScript resources.
 */
QWQmlEngine.prototype.importSharedJS = function(url)
{
    var resourceName = url.split('/')[0]; // FIXME: use the last, instead of the first part.
    if (resourceName in QWQmlEngine.__loadedJSSharedResources)
        return QWQmlEngine.__loadedJSSharedResources[resourceName];

    var module = qw_evalJS(qw_fetchData([this.baseUrl + url]), this);
    QWQmlEngine.__loadedJSSharedResources[resourceName] = module;
    return module;
}

QWQmlEngine.__loadedModules = {}; // static
QWQmlEngine.__loadedFiles = {}; // static
QWQmlEngine.__loadedJSResources = {}; // static
QWQmlEngine.__loadedJSSharedResources = {}; // static

QWQmlEngine.contextForObject = function(object) { // static
    if (object.__ctx) // If the object is the root of a component, we want to
        return object.__ctx.__parentContext || object.__ctx; // return the outer context.

    while (object && !object.__ctx)
        object = object.__parent;

    return object ? object.__ctx : null;
}
