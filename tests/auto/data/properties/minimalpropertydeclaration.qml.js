QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();

    this.test = new QWProperty({
        initialValue: "hello world!"
    });
}

__comp;
