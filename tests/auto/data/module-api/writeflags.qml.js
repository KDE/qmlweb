QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.a = new QWProperty({
        initialValue: 35
    });
    __0.b = new QWProperty({
        bind: function() { return __0.a.get(); },
        ctx: __
    });
}

__comp;
