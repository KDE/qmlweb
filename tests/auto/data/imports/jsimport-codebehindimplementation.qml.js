QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = __.karl = this;
    var JS = eval(__engine.fetchJSResource("codebehindimplementation.compiled.js"));

    __0.x = new QWProperty({
        bind: function() { return JS.xProp; },
        ctx: __
    });

    __0.increase = function() {
        JS.add(5);
    }
}

__comp;

