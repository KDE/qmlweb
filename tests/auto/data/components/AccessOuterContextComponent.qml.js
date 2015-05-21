QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);

    this.c = new QWProperty({
        bind: function() { return __.find("a") - 43; },
        ctx: __
    });
    this.profession = new QWProperty({
        bind: function() { return __.find("bob").profession.get(); },
        ctx: __
    });


    QtQml20.Component.qmlAttachedProperties(this).completed.connect(this, function() {
        __.findAndSet("b", 232);
    });
}

__comp;
