QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __0 = this;

    QW_PROPERTY({
        object: __0,
        name: "children",
        initialValue: new QWList(__0)
    });
    QW_GET(__0, 1).append((function() {
            var __1 = new QtQml20.QtObject(__0);
            QW_SET(__1, 0, "el1");
            return __1;
        })());
    QW_GET(__0, 1).append((function() {
            var __1 = new QtQml20.QtObject(__0);
            QW_SET(__1, 0, "el2");
            return __1;
        })());
    QW_GET(__0, 1).append((function() {
            var __1 = new QtQml20.QtObject(__0);
            QW_SET(__1, 0, "el3");
            return __1;
        })());
    QW_GET(__0, 1).append((function() {
            var __1 = new QtQml20.QtObject(__0);
            QW_SET(__1, 0, "el4");
            return __1;
        })());
    QW_GET(__0, 1).append((function() {
            var __1 = new QtQml20.QtObject(__0);
            QW_SET(__1, 0, "el5");
            return __1;
        })());
}

__comp;
