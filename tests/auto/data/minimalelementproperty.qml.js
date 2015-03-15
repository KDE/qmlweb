QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __0 = this;

    QW_PROPERTY({
        object: __0,
        name: "child",
        initialValue: (function() {
            var __1 = new QtQml20.QtObject(__0);
            QW_SET(__1, 0, "childObject");
            return __1;
        })()
    });
}

__comp;


