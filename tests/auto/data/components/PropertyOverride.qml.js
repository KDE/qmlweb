
QW_INHERIT(__comp, QtQml20.QtObject);
function __comp(parent) {
    QtQml20.QtObject.call(this, parent);
    var __ = new QWContext(this);
    var __0 = this;

    __0.objectName.set("hugo");
    __0.message = new QWProperty({
        initialValue: "I don't have a message, I'm only a dummy text."
    });
    __0.greeting = new QWProperty({
        bind: function() { return "Hello, " + __0.objectName.get() + "!"; },
        ctx: __
    });
    __0.closing = new QWProperty({
        bind: function() { return "Bye, " + __0.objectName.get() + "!"; },
        ctx: __
    });
    __0.x = new QWProperty({
        initialValue: 27
    });
    __0.y = new QWProperty({
        bind: function() { return 2*__0.x.get() },
        ctx: __
    });
}

__comp;
