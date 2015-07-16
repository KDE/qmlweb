QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);

    this.innerOnCompletedCount = new QWProperty({
        initialValue: 0
    });

    QtQml20.Component.qmlAttachedProperties(this).completed.connect(this, function() {
        this.innerOnCompletedCount.set(this.innerOnCompletedCount.get() + 1);
    });
}

__comp;
