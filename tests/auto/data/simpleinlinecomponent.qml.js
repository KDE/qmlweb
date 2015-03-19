QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0, __1;
    __0 = this;

    QW_PROPERTY({
        object: __0,
        name: "obj"
    });
    QW_BIND(__0, 1, function() { return QW_GET(__0, 2).createObject(); }, __);
    QW_PROPERTY({
        object: __0,
        name: "comp",
        initialValue: (function() {
            var __1 = new QtQml20.Component(__0);

            QW_INHERIT(__comp, QtQml20.QtObject);
            function __comp(parent) {
                QtQml20.QtObject.call(this, parent);
                var __ = this.__ctx = new QWContext();

                QW_SET(this, 0, "KDE");
            }

            QW_SET(__1, 0, __comp);

            return __1;
        })()
    });
}

__comp;
