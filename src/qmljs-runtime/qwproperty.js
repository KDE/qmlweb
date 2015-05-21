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
var QWProperty,
    QWList;

function QWInt(val)
{
    return val|0;
}
function QWUrl(val)
{
    if (val)
        return String(val);
    else
        return String();
}
function QWVar(val)
{
    return val;
}

(function() {

// Property that is currently beeing evaluated. Used to get the information, which property called
// the getter of a certain other property for evaluation and is thus dependent on it.
var evaluatingProperty = null;

// === QWProperty ===

QWProperty = function(data)
{
    this.value = data.initialValue;
    this.getter = data.get;
    this.setter = data.set;
    this.binding = null;
    this.interceptor = null;
    this.notify = QWSignal();

    if (data.bind)
        this.bind(data.bind, data.ctx);

    if (data.type && !("initialValue" in data)) {
        switch (data.type) {
            case Boolean:
            case Number:
            case QWInt:
            case String:
            case QWUrl:
            case QWVar:
                this.value = data.type();
                break;
            default:
                this.value = new data.type();
        }
    }
}

QWProperty.prototype.clone = function() {
    return new QWProperty({
        initialValue: this.value,
        get: this.get,
        set: this.set
    })
}

QWProperty.prototype.get = function()
{
    // If this call to get is due to a property that is dependent on this
    // one, we need it to track changes
    if (evaluatingProperty && !this.notify.isConnected(evaluatingProperty, QWProperty.prototype.update)) {
        this.notify.connect(evaluatingProperty, QWProperty.prototype.update);
    }

    if (this.getter) {
        return this.getter()
    } else {
        return this.value;
    }
}

QWProperty.prototype.set = function(newValue)
{
    if (this.setter) {
        this.setter(newValue);
    } else {
        this.value = newValue;
    }
    this.binding = null;
    this.notify();
}

QWProperty.prototype.update = function() {
    if (!this.binding) {
        return;
    }

    var oldVal = this.val;
    evaluatingProperty = this;
    this.value = this.binding();
    evaluatingProperty = null;
    this.notify();
}

QWProperty.prototype.bind = function(bindingFunction, context)
{
    // put down for subsequent evaluation
    context.pendingBindingEvaluations.push(this);

    this.binding = bindingFunction;
    this.notify();
}

QWProperty.prototype.getAttr = function(attributeIndex) {
    return this.value[attributeIndex];
}

QWProperty.prototype.setAttr = function(attributeIndex, newValue) {
    this.value[attributeIndex] = newValue;
    this.notify();
}

// === QWList ===

QWList = function(object) {
    Object.defineProperty(this, "__object", { configurable: false, enumerable: false, writable: false, value: object });
}
QWList.prototype = new Array();
QWList.prototype.append = Array.prototype.push;

})()
