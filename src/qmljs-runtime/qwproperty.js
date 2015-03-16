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

// === pseudo-macros ===

// exports
var QW_PROPERTY,
    QW_GET,
    QW_SET,
    QW_BIND,
    QWQmlProperty,
    QWList;

(function() {

// Property that is currently beeing evaluated. Used to get the information, which property called
// the getter of a certain other property for evaluation and is thus dependent on it.
var evaluatingProperty = null;

QW_PROPERTY = function(data)
{
    if (data.object) {
        data.object.__properties[data.object.__propertyCount] = new QWQmlProperty(data);
        data.object.__propertyCount++;
    } else {
        data.ctor.__properties.push(new QWQmlProperty(data));
    }
}

QW_GET = function(object, propertyIndex)
{
    // We create properties when they are first accessed, to save time and memory, so if there is
    // no property, yet, create it.
    if (!object.__properties[propertyIndex]) {
        object.__properties[propertyIndex] = object.constructor.__properties[propertyIndex].clone();
    }
    var prop = object.__properties[propertyIndex];

    // If this call to QW_GET is due to a property that is dependent on this
    // one, we need it to take track of changes
    if (evaluatingProperty && !prop.notify.isConnected(evaluatingProperty, QWQmlProperty.prototype.update)) {
        prop.notify.connect(evaluatingProperty, QWQmlProperty.prototype.update);
    }

    return prop.value;
}

QW_SET = function(object, propertyIndex, newValue)
{
    // We create properties when they are first accessed, to save time and memory, so if there is
    // no property, yet, create it.
    if (!object.__properties[propertyIndex]) {
        object.__properties[propertyIndex] = object.constructor.__properties[propertyIndex].clone();
    }

    object.__properties[propertyIndex].value = newValue;
    object.__properties[propertyIndex].notify();
}

QW_BIND = function(object, propertyIndex, bindingFunction)
{
    // We create properties when they are first accessed, to save time and memory, so if there is
    // no property, yet, create it.
    if (!object.__properties[propertyIndex]) {
        object.__properties[propertyIndex] = object.constructor.__properties[propertyIndex].clone();
    }
    var prop = object.__properties[propertyIndex];

    // put down for subsequent evaluation
    object.__ctx.__pendingBindingEvaluations.push(prop);

    prop.binding = bindingFunction;
    prop.notify();
}

QW_GETATTR = function(object, propertyIndex, attributeIndex) {
    return object.__properties[propertyIndex].value[attributeIndex];
}

QW_SETATTR = function(object, propertyIndex, attributeIndex, newValue) {
    object.__properties[propertyIndex].value[attributeIndex] = newValue;
    object.__properties[propertyIndex].notify();
}

QW_NOTIFYSIG = function(object, propertyIndex) {
    return object.__properties[propertyIndex].notify;
}

// === QWQmlProperty ===

QWQmlProperty = function(data)
{
    this.value = data.initialValue;
    this.get = data.get;
    this.set = data.set;
    this.binding = null;
    this.interceptor = null;
    this.notify = QW_SIGNAL({});
}

QWQmlProperty.prototype.clone = function() {
    return new QWQmlProperty({
        initialValue: this.value,
        get: this.get,
        set: this.set
    })
}
QWQmlProperty.prototype.update = function() {
    if (!this.binding) {
        return;
    }

    var oldVal = this.val;
    evaluatingProperty = this;
    this.value = this.binding();
    evaluatingProperty = null;
    this.notify();
}

// === QWList ===

QWList = function(object) {
    Object.defineProperty(this, "__object", { configurable: false, enumerable: false, writable: false, value: object });
}
QWList.prototype = new Array();
QWList.prototype.append = Array.prototype.push;

})()
