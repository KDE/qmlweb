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

function QWContext()
{
    this.__pendingBindingEvaluations = [];
    this.nameForObject = function(obj) {
        for (var name in this) {
            if (this[name] == obj)
                return name;
        }
    }
}

QWQmlComponent.__properties = [];
QW_PROPERTY({ ctor: QWQmlComponent, name: "__componentCtor" });
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

    // Fetch file from url, evaluate and set as __componentCtor.
    if (fileName != '') {
        QW_SET(this, 0, qw_evalJS(qw_fetchData([engine.baseUrl + fileName + ".js"])));
    }

    this.createObject = function(parent)
    {
        return this.create(QWQmlEngine.contextForObject(this), parent);
    }

    this.create = function(context, parent)
    {
        var ctor = QW_GET(this, 0);
        var object = new ctor(parent);
        object.__ctx.__parentContext = context;
        while(object.__ctx.__pendingBindingEvaluations.length) {
            var property = object.__ctx.__pendingBindingEvaluations.pop();

            if (!property.binding)
                continue; // Probably, the binding was overwritten by an explicit value. Ignore.
            property.update();
        }
        return object;
    }
}
QW_INHERIT(QWQmlComponent, QWObject);
