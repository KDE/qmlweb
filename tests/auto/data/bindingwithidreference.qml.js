QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0, __1;
    __0 = __.main = this;

    __0.prop1 = new QWProperty({});
    __0.prop1.bind(function() { return __1.prop3.get() - 5; }, __);
    __0.prop2 = new QWProperty({
        initialValue: 5
    });
    __0.child = new QWProperty ({
        initialValue: (function() {
            __1 = __.foo = new QtQml20.QtObject(__0);
            __1.prop3 = new QWProperty({});
            __1.prop3.bind(function() { return __0.prop2.get() + 10; }, __);
            return __1;
        })()
    });
}

__comp;
