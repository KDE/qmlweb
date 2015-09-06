(function() {
    function A(parent)
    {
        this.minimalProp = new QWProperty({  });
        this.propWithType = new QWProperty({ type: A });
    }

    B.prototype.prototypeMethod = function() {
        console.log("Baking for " + this.bakingTime.get() + " minutes.");
    }
    B.prototype.prototypeMethod2 = function(prop) {
        console.log("Baking for " + this.bakingTime.get() + " minutes.");
    }
    function B(parent)
    {
        this.minimalProp = new QWProperty({ });

        this.otherPropWithType = new QWProperty({ type: A });

        this.someSignal = QWSignal();
        this.anotherSignal = QWSignal([{type: A, name: "arg1"}, {type: A, name: "arg2"}]);

        this.constructorMethod = function(prop1, prop2) {
            console.log("output!");
        }
    }

    var C = function(parent)
    {
        B.call(this, parent);

        this.yaPropWithType = new QWProperty({ type: C });
    }
    QW_INHERIT(C, B);

    QW_INHERIT(D_, B);
    function D_(parent)
    {
        B.call(this, parent);

        this.readonlyProp = new QWProperty({ type: D_, readonly: true, initialValue: false });
        this.constantProp = new QWProperty({ type: B, constant: true });
    }

    QW_INHERIT(E, B);
    function E(parent)
    {
        B.call(this, parent);

        this.fourthPropWithType = new QWProperty({ type: D_ });
        this.constantProp = new QWProperty({ type: B, constant: true });
    }

    __engine.registerModule({
        A: A,
        B: B,
        C: C,
        D: D_
        // E is not registered on purpose. That's part of the test.
    });
})();
