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

function QWContext(rootObject)
{
    this.parent = null;
    this.children = [];
    this.pendingBindingEvaluations = [];
    this.rootObject = rootObject;
    // List of the uses of Component attached property in this context
    this.componentAttached = [];
    // Hash of objects in this context that have an id, key is the id.
    this.id = {};

    var other;
    if (other = rootObject.__ctx) { // the other context is an inner context and will thus become a child context.
        other.parent = this;
        this.children.push(other);
    } else if (other = QWQmlEngine.contextForObject(rootObject)) { // the other context is the outer context.
        this.parent = other;
        other.children.push(this);
    }

    rootObject.__ctx = this;
}

QWContext.prototype.nameForObject = function(obj)
{
    for (var name in this.id) {
        if (this.id[name] == obj)
            return name;
    }
}

QWContext.prototype.evaluatePendingBindings = function()
{
    for (var i in this.children) {
        this.children[i].evaluatePendingBindings();
    }
    while(this.pendingBindingEvaluations.length) {
        var property = this.pendingBindingEvaluations.pop();

        if (!property.binding)
            continue; // Probably, the binding was overwritten by an explicit value. Ignore.
        property.update();
    }
}

/**
 * Finalizes initialization of the context, including child contexts (recursively).
 *
 * Basically, it means calling Component.onCompleted handlers.
 */
QWContext.prototype.finalizeInit = function()
{
    for (var i in this.children) {
        this.children[i].finalizeInit();
    }
    for(var i in this.componentAttached) {
        var attached = this.componentAttached[i];

        attached.completed();
    }
}

/**
 * Finds objects or properties by name in this context or parent contexts.
 *
 * If @p name refers to a property, it will return the value directly, as
 * returned by QWProperty.get().
 */
QWContext.prototype.find = function(name)
{
    var ctx = this;
    while (ctx) {
        if (name in ctx.id) {
            return ctx.id[name];
        } else if (name in ctx.rootObject) {
            return ctx.rootObject[name].get();
        }
        ctx = ctx.parent;
    }

    throw new ReferenceError("Can't find '" + name + "'.");
}

/**
 * Finds objects or properties by name in this context or parent contexts
 * and assigns @p newVal to them.
 */
QWContext.prototype.findAndSet = function(name, newVal)
{
    var ctx = this;
    while (ctx) {
        if (name in ctx.id) {
            throw new TypeError("'" + name + "' refers to an id and thus isn't writable.");
        } else if (name in ctx.rootObject) {
            ctx.rootObject[name].set(newVal);
            return;
        }
        ctx = ctx.parent;
    }

    throw new ReferenceError("Can't find '" + name + "'.");
}
