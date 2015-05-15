QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    __0.children = new QWProperty({
        initialValue: new QWList(__0)
    });
    __0.children.get().append((function() {
            var __1 = new QtQml20.QtObject(__0);
            __1.objectName.set("el1");
            return __1;
        })());
    __0.children.get().append((function() {
            var __1 = new QtQml20.QtObject(__0);
            __1.objectName.set("el2");
            return __1;
        })());
    __0.children.get().append((function() {
            var __1 = new QtQml20.QtObject(__0);
            __1.objectName.set("el3");
            return __1;
        })());
    __0.children.get().append((function() {
            var __1 = new QtQml20.QtObject(__0);
            __1.objectName.set("el4");
            return __1;
        })());
    __0.children.get().append((function() {
            var __1 = new QtQml20.QtObject(__0);
            __1.objectName.set("el5");
            return __1;
        })());
}

__comp;
