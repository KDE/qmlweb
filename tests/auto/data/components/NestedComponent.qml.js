var _Local = __engine.loadLocal(["PropertyOverride.qml"]);

QW_INHERIT(__comp, _Local.PropertyOverride);
function __comp(parent) {
    _Local.PropertyOverride.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.z = new QWProperty({
        bind: function() { return __0.x.get() + __0.y.get(); },
        ctx: __
    });
    __0.objectName.set("Hans");
}

__comp;
