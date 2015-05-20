QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);

    this.objectName.bind(function() { return "" + (3*100+5-5+Math.sin(1-1)); }, __);
}

__comp;

