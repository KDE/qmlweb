QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);

    QW_SIGNAL({ object: this,
                name: "someSignal"
              });

    this.someSignal.connect(this, function(foo) {
        console.log("hello world!");
        foo.ok(true, "Signal handler was called.");
    });
}

__comp;



