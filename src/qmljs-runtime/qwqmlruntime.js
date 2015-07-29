/*
 * Qml.js Runtime - Bringing QML's power to the web.
 *
 * Copyright (C) 2011  Lauri Paimen <lauri@paimen.info>
 * Copyright (C) 2015 Anton Kreuzkamp <kde-development@akreuzkamp.de>
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

// exports
var qw_fetchData,
    qw_evalJS,
    qw_import;

(function() {

var __ = null;

// === helper pseudo-macros ===

QW_INHERIT = function(ctor, baseClass)
{
    ctor.prototype = Object.create(baseClass.prototype);
    ctor.prototype.constructor = ctor;
    __ = ctor.prototype;
}

// === helper functions ===

/**
 * Eval JavaScript. Used to evaluate modules and compiled QML from
 * within the same scope.
 * @param __src The sources to be evaluated
 * @param __engine The QWQmlEngine used. This one is only there, to be available in the evaluated code.
 */
qw_evalJS = function(__src, __engine) {
    return eval(__src);
}

/**
 * Get URL contents.
 * @param urls {Array} Urls to fetch.
 * @return {mixed} String of contents or false in errors.
 */
qw_fetchData = function(urls)
{
    var xhr = new XMLHttpRequest();
    xhr.open("GET", urls[0], false);
    xhr.send(null);
    if (xhr.status != 200 && xhr.status != 0) { // 0 if accessing with file://
        console.log("Retrieving " + urls[0] + " failed: " + xhr.responseText, xhr);
        return false;
    }
    return xhr.responseText;
}

})();