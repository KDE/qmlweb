/* @license

  Copyright (c) 2011 Lauri Paimen <lauri@paimen.info>
  Copyright (c) 2015 Anton Kreuzkamp <akreuzkamp@web.de>
  Copyright (c) 2015 Jan Gerrit Marker <jangerrit@weiler-marker.com>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

      * Redistributions of source code must retain the above
        copyright notice, this list of conditions and the following
        disclaimer.

      * Redistributions in binary form must reproduce the above
        copyright notice, this list of conditions and the following
        disclaimer in the documentation and/or other materials
        provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER “AS IS” AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.
*/

// Exports
var QObject,
    QmlEngine,
    QmlComponent;

// Library
(function(){
var __ = null;

// === "macros" ===

function QW_INHERIT(ctor, baseClass) {
    ctor.prototype = Object.create(baseClass.prototype);
    ctor.prototype.constructor = baseClass;
    __ = ctor.prototype;
}

function QW_PROPERTY(data) {
    data.obj.__properties.push(new QmlProperty(data.initialValue));
}

/**
 * Get URL contents.
 * @param urls {Array} Urls to fetch.
 * @return {mixed} String of contents or false in errors.
 */
function fetchData(urls) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", urls[0], false);
    xhr.send(null);
    if (xhr.status != 200 && xhr.status != 0) { // 0 if accessing with file://
        console.log("Retrieving " + urls[0] + " failed: " + xhr.responseText, xhr);
        return false;
    }
    return xhr.responseText;
}

// === QmlWebEngine ===

QmlEngine = function() {

}
__ = QmlEngine.prototype;
__.baseUrl = "";

// === Signal ===

/**
 * Creates and returns a signal with the parameters specified in @p params.
 *
 * @param params Array with the parameters of the signal. Each element has to be
 *               an object with the two properties "type" and "name" specifying
 *               the datatype of the parameter and its name. The type is
 *               currently ignored.
 * @param options Options that allow finetuning of the signal.
 */
function QWSignal(params, options) {
    options = options || {};
    var connectedSlots = [];
    var obj = options.obj

    var signal = function() {
        for (var i in connectedSlots)
            connectedSlots[i].slot.apply(connectedSlots[i].thisObj, arguments);
    };
    signal.parameters = params || [];
    signal.connect = function() {
        if (arguments.length == 1)
            connectedSlots.push({thisObj: window, slot: arguments[0]});
        else if (typeof arguments[1] == 'string' || arguments[1] instanceof String) {
            if (arguments[0].__tidyupList && arguments[0] !== obj)
                arguments[0].__tidyupList.push(this);
            connectedSlots.push({thisObj: arguments[0], slot: arguments[0][arguments[1]]});
        } else {
            if (arguments[0].__tidyupList && (!obj || (arguments[0] !== obj && arguments[0] !== obj.__parent)))
                arguments[0].__tidyupList.push(this);
            connectedSlots.push({thisObj: arguments[0], slot: arguments[1]});
        }

        // Notify object of connect
        if (options.obj && options.obj.__connectNotify) {
            options.obj.__connectNotify(options);
        }
    }
    signal.disconnect = function() {
        var callType = arguments.length == 1 ? (arguments[0] instanceof Function ? 1 : 2)
                       : (typeof arguments[1] == 'string' || arguments[1] instanceof String) ? 3 : 4;
        for (var i = 0; i < connectedSlots.length; i++) {
            var item = connectedSlots[i];
            if ((callType == 1 && item.slot == arguments[0])
                || (callType == 2 && item.thisObj == arguments[0])
                || (callType == 3 && item.thisObj == arguments[0] && item.slot == arguments[0][arguments[1]])
                || (item.thisObj == arguments[0] && item.slot == arguments[1])
            ) {
                if (item.thisObj)
                    item.thisObj.__tidyupList.splice(item.thisObj.__tidyupList.indexOf(this), 1);
                connectedSlots.splice(i, 1);
                i--; // We have removed an item from the list so the indexes shifted one backwards
            }
        }

        // Notify object of disconnect
        if (options.obj && options.obj.__disconnectNotify) {
            options.obj.__disconnectNotify(options);
        }
    }
    signal.isConnected = function() {
        var callType = arguments.length == 1 ? 1
                       : (typeof arguments[1] == 'string' || arguments[1] instanceof String) ? 2 : 3;
        for (var i in connectedSlots) {
            var item = connectedSlots[i];
            if ((callType == 1 && item.slot == arguments[0])
                || (callType == 2 && item.thisObj == arguments[0] && item.slot == arguments[0][arguments[1]])
                || (item.thisObj == arguments[0] && item.slot == arguments[1])
            )
                return true;
        }
        return false;
    }
    return signal;
}

// === QmlProperty ===
function QmlProperty(data)
{
    this.value = data.initialValue;
    this.get = data.get;
    this.set = data.set;
    this.binding = null;
    this.animation = null;
    this.notify = QWSignal();
}


// === QtObject ===

QObject = function(parent) {
    QW_PROPERTY({obj: this, name:"", });
    this.__parent = parent;
    if (parent && parent.__tidyupList)
        parent.__tidyupList.push(this);
    // List of things to tidy up when deleting this object.
    if (!this.__tidyupList)
        this.__tidyupList = [];
}
QObject.delete = function(obj) {
    while (obj.__tidyupList.length > 0) {
        var item = obj.__tidyupList[0];
        if (item instanceof QObject)
            QObject.delete(item);
        else // It must be a signal
            item.disconnect(obj);
    }
    //FIXME: garbage collecting and properties?
//     for (var i in obj.__properties) {
//         var prop = obj.__properties[i];
//         while (prop.__tidyupList.length > 0)
//             prop.__tidyupList[0].disconnect(prop);
//     }

    if (obj.__parent && obj.__parent.__tidyupList)
        obj.__parent.__tidyupList.splice(obj.__parent.__tidyupList.indexOf(obj), 1);
}

// === QmlWebComponent ===

QmlComponent = function(engine) {
    var fileName = '', parent = null;
    if (typeof arguments[1] == "string") {
        fileName = arguments[1];
        if (arguments[2] instanceof Object)
            parent = arguments[2];
    } else if (arguments[1] instanceof Object) {
        parent = arguments[1];
    }
    QObject.call(this, parent);

    var componentCtor = eval(fetchData([engine.baseUrl + fileName + ".js"]));

    this.create = function(context) {
        componentCtor();
    }
}
QW_INHERIT(QmlComponent, QObject);

var QtQml20 = {
    QtObject: QObject,
    Component: QmlComponent
};


})();