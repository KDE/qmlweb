/*
 * Qml.js Runtime - Bringing QML's power to the web.
 *
 * Copyright (C) 2015  Anton Kreuzkamp <kde-development@akreuzkamp.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER “AS IS” AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

QW_INHERIT(QWQmlComponent, QWObject);
function QWQmlComponent()
{
    QWObject.call(this, arguments[0]); // Uses either parent or engine as parent.

    // Parse Arguments - this is the javascript way of doing function overloads
    var engine, parent, fileName = '', parent = null;
    if (arguments[0] instanceof QWObject) {
        parent = arguments[0];
    } else {
        engine = arguments[0];
        if (typeof arguments[1] == "string") {
            fileName = arguments[1];
            if (arguments[2] instanceof Object)
                parent = arguments[2];
        } else if (arguments[1] instanceof Object) {
            parent = arguments[1];
        }
    }

    this.__componentCtor = new QWProperty({});

    // Fetch file from url, evaluate and set as __componentCtor.
    if (fileName != '') {
        engine.baseUrl = fileName.split("/");
        engine.baseUrl[engine.baseUrl.length - 1] = "";
        engine.baseUrl = engine.baseUrl.join("/");
        this.__componentCtor.set(qw_evalJS(qw_fetchData([fileName + ".js"]), engine));
    }

    this.createObject = function(parent)
    {
        return this.create(QWQmlEngine.contextForObject(this), parent);
    }

    this.create = function(context, parent)
    {
        var ctor = this.__componentCtor.get();
        var object = new ctor(parent);
        object.__ctx.parent = context;
        object.__ctx.evaluatePendingBindings();
        object.__ctx.finalizeInit();
        return object;
    }
}

function QWQmlComponentAttached(object) {
    this.completed = QWSignal();
    QWQmlEngine.contextForObject(object).componentAttached.push(this);
}
QWQmlComponent.qmlAttachedProperties = function(object) // static
{
    return new QWQmlComponentAttached(object);
}
