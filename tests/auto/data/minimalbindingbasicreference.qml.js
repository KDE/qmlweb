QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    __0.extendedName = new QWProperty({});
    __0.objectName.set("hello");
    __0.extendedName.bind(function() { return __0.objectName.get() + " world!"; }, __);
}

__comp;

