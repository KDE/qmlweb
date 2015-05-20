QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);

    this.test = new QWProperty({
        initialValue: "hello world!"
    });
}

__comp;
