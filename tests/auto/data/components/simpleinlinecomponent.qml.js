QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);
    var __0, __1;
    __0 = this;

    __0.obj = new QWProperty({});
    __0.obj.bind(function() { return __0.comp.get().createObject(); }, __);
    __0.comp = new QWProperty({
        initialValue: (function() {
            var __1 = new QtQml20.Component(__0);

            QW_INHERIT(__comp, QtQml20.QtObject);
            function __comp(parent) {
                QtQml20.QtObject.call(this, parent);
                var __ = new QWContext(this);

                this.objectName.set("KDE");
            }

            __1.__componentCtor.set(__comp);

            return __1;
        })()
    });
}

__comp;
