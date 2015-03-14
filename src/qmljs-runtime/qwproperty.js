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

function QW_PROPERTY(data)
{
    if (data.object) {
        data.object.__properties[data.object.__propertyCount] = new QmlProperty(data);
        data.object.__propertyCount++;
    } else {
        data.ctor.__properties.push(new QmlProperty(data));
    }
}

function QW_GET(object, propertyIndex)
{
    if (!object.__properties[propertyIndex])
        object.__properties[propertyIndex] = object.constructor.__properties[propertyIndex].clone();

    return object.__properties[propertyIndex].value;
}

function QW_SET(object, propertyIndex, newValue)
{
    if (!object.__properties[propertyIndex])
        object.__properties[propertyIndex] = object.constructor.__properties[propertyIndex].clone();

    object.__properties[propertyIndex].value = newValue;
    object.__properties[propertyIndex].notify();
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
QmlProperty.prototype.clone = function() {
    return new QmlProperty({
        initialValue: this.value,
        get: this.get,
        set: this.set
    })
}
