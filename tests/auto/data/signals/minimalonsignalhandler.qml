
QtObject {
    signal someSignal(var foo);

    onSomeSignal: {
        console.log("hello world!");
        foo.ok(true, "Signal handler was called.");
    }
}
