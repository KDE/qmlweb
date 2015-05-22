var GetterSetterModule01 = __engine.importModule("GetterSetterModule", 0, 1);

QW_INHERIT(__comp, GetterSetterModule01.Rect);
function __comp(parent) {
    GetterSetterModule01.Rect.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.width.set(35);
}

__comp;
