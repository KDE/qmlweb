QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);

    this.a = new QWProperty({
        type: Boolean
    });
    this.b = new QWProperty({
        type: Number
    });
    this.c = new QWProperty({
        type: QWInt
    });
    this.d = new QWProperty({
        type: Number
    });
    this.e = new QWProperty({
        type: String
    });
    this.f = new QWProperty({
        type: QWUrl
    });
    this.g = new QWProperty({
        type: QWVar
    });
    this.h = new QWProperty({
        type: QtQml20.QtObject
    });
    this.i = new QWProperty({
        type: QtQml20.Component
    });
    this.j = new QWProperty({
        type: QWList
    });
}

__comp;