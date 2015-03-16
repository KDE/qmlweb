QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0, __1;
    __0 = __.main = this;

    QW_PROPERTY({
        object: __0,
        name: "prop1"
    });
    QW_BIND(__0, 1, function() { return QW_GET(__1, 1) - 5; }, __);
    QW_PROPERTY({
        object: __0,
        name: "prop2",
        initialValue: 5
    });
    QW_PROPERTY({
        object: __0,
        name: "child",
        initialValue: (function() {
            __1 = __.foo = new QtQml20.QtObject(__0);
            QW_PROPERTY({
                object: __1,
                name: "prop3"
            });
            QW_BIND(__1, 1, function() { return QW_GET(__0, 2) + 10; }, __);
            return __1;
        })()
    });
}

__comp;
