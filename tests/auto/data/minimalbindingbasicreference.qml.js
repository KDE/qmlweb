QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this;

    QW_PROPERTY({
        object: __,
        name: "extendedName"
    });
    QW_SET(__, 0, "hello");
    QW_BIND(__, 1, function() { return QW_GET(__, 0) + " world!"; });
}

__comp;

