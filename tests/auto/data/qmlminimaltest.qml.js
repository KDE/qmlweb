QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);

    QW_SET(this, 0, "hugo");

    QW_PROPERTY({
        object: this,
        name: "test",
        initialValue: "hello world!"
    });
}

__comp;