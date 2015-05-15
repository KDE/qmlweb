QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    __0.child = new QWProperty({
        initialValue: (function() {
            var __1 = new QtQml20.QtObject(__0);
            __1.objectName.set("childObject");
            return __1;
        })()
    });
}

__comp;


