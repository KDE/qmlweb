QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    QW_PROPERTY({
        object: __0,
        name: "prop1",
        initialValue: 5
    });
    QW_PROPERTY({
        object: __0,
        name: "prop2"
    });
    QW_BIND(__0, 2, function() { return 7 * QW_GET(__0, 1); });
}

__comp;
