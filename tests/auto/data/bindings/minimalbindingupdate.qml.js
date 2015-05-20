QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.prop1 = new QWProperty({
        initialValue: 5
    });
    __0.prop2 = new QWProperty({});
    __0.prop2.bind(function() { return 7 * __0.prop1.get(); }, __);
}

__comp;
