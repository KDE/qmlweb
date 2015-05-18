var JS = __engine.importSharedJS("shared.compiled.js");

QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = __.karl = this;

    __0.x = new QWProperty({
        bind: function() { return JS.xProp; },
        ctx: __
    });
    __0.ok = new QWProperty({
        initialValue: true
    });

    __0.increase = function() {
        __0.x.set(JS.add(__0.x.get(), 5));
    }
}

__comp;

