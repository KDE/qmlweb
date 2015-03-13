QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    Q_PROPERTY({
        object: this,
        name: "test",
        initialValue: "hello world!"
    });
}

__comp;