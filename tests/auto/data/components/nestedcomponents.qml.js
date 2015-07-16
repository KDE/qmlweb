var _Local = __engine.loadLocal(["NestedComponent.qml"]);

QW_INHERIT(__comp, _Local.NestedComponent);
function __comp(parent) {
    _Local.NestedComponent.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.greeting.set("Hey, guys!");
    __0.x.bind(function() { return 5 * __0.objectName.get().length; }, __);
    __0.y.bind(function() { return 3 * __0.x.get(); }, __);
}

__comp;
