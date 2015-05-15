QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = this;

    __0.someProp = new QWProperty({
        initialValue: QmlPseudoModule.color("#ff0088")
    });
    __0.otherProp = new QWProperty({
        initialValue: QmlPseudoModule.color()
    });
    __0.otherProp.setAttr(0, 0);
    __0.otherProp.setAttr(1, 110);
    __0.otherProp.setAttr(2, 0);
}

__comp;
