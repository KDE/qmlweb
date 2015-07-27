/*
 * Copyright (C) 2015  Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

QWObject.__properties = [];
function QWObject(parent)
{
    this.__parent = parent;
    if (parent && parent.__tidyupList)
        parent.__tidyupList.push(this);

    this.objectName = new QWProperty({ type: String, initialValue: "" });

    // List of things to tidy up when deleting this object.
    this.__tidyupList = [];
    // Qml context of this object (null doesn't mean, there's no context,
    // just that this object isn't the root of the context)
    this.__ctx = null;
}

QWObject.delete = function(obj)
{
    while (obj.__tidyupList.length > 0) {
        var item = obj.__tidyupList[0];
        if (item instanceof QWObject)
            QWObject.delete(item);
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
