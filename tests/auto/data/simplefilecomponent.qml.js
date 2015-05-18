var _Local = __engine.loadLocal(["MinimalComponent.qml"]);

QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    __0.obj = new QWProperty({
        initialValue: (function() {
            var __1 = new _Local.MinimalComponent(__0);
            return __1;
        })()
    });
}

__comp;
