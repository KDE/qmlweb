var _Local = __engine.loadLocal(["PropertyOverride.qml"]);

QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.obj = new QWProperty({
        initialValue: (function() {
            var __1 = new _Local.PropertyOverride(__0);
            __1.objectName.set("karl");
            __1.greeting.set("Hey, guys!");
            __1.x.bind(function() { return 5 * __1.objectName.get().length; }, __);
            __1.y.bind(function() { return 3 * __1.x.get(); }, __);

            return __1;
        })()
    });
}

__comp;

