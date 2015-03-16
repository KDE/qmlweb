QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    QW_PROPERTY({
        object: __0,
        name: "extendedName"
    });
    QW_SET(__0, 0, "hello");
    QW_BIND(__0, 1, function() { return QW_GET(__0, 0) + " world!"; }, __);
}

__comp;

