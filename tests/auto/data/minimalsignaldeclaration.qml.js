QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);

    QW_SIGNAL({ object: this,
                name: "someSignal"
              });
}

__comp;


