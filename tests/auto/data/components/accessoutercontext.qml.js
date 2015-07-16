var _Local = __engine.loadLocal(["AccessOuterContextComponent.qml"]);

QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.a = new QWProperty({
        initialValue: 467
    });
    __0.b = new QWProperty({
        initialValue: 323
    });
    __0.obj = new QWProperty({
        initialValue: (function() {
            var __1 = __.id.bob = new QtQml20.QtObject(__0);
            __1.profession = new QWProperty({
                initialValue: "Builder"
            });
            return __1;
        })()
    });
    __0.comp = new QWProperty({
        initialValue: (function() {
            var __1 = new _Local.AccessOuterContextComponent(__0);
            return __1;
        })()
    });
}

__comp;


