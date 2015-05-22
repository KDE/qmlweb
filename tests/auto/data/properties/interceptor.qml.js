var GetterSetterModule01 = __engine.importModule("GetterSetterModule", 0, 1);

QW_INHERIT(__comp, GetterSetterModule01.Rect);
function __comp(parent) {
    GetterSetterModule01.Rect.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.width.set(35);
    __0.interceptor = new QWProperty({
        initialValue: (function() {
            var __1 = new GetterSetterModule01.TheInterceptor(__0);
            return __1;
        })()
    });
}

__comp;
