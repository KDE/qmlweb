QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    QW_PROPERTY({
        object: __0,
        name: "someProp",
        initialValue: QmlPseudoModule.color("#ff0088")
    });
    QW_PROPERTY({
        object: __0,
        name: "otherProp",
        initialValue: QmlPseudoModule.color()
    });
    QW_SETATTR(__0, 2, 0, 0);
    QW_SETATTR(__0, 2, 1, 110);
    QW_SETATTR(__0, 2, 2, 0);
}

__comp;
