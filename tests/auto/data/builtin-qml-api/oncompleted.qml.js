var _Local = __engine.loadLocal(["OnCompletedComponent.qml"]);

QW_INHERIT(__comp, _Local.OnCompletedComponent);
function __comp(parent) {
    _Local.OnCompletedComponent.call(this, parent);
    var __ = new QWContext(this);

    this.outerOnCompletedCount = new QWProperty({
        initialValue: 0
    });

    QtQml20.Component.qmlAttachedProperties(this).completed.connect(this, function() {
        this.outerOnCompletedCount.set(this.outerOnCompletedCount.get() + 1);
    });
}

__comp;
