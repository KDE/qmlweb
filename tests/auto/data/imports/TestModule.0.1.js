(function() {
    QW_INHERIT(Pastry, QWObject);

    if (window.__qmljsTestModuleDefined)
        throw "TestModule defined twice.";
    window.__qmljsTestModuleDefined = true;

    function Oven(parent)
    {
        QWObject.call(this, parent);

        this.temperature = new QWProperty({ type: QWInt, initialValue: 20 });
        this.content = new QWProperty({ type: QWList, typeArg: Pastry, initialValue: new QWList() });
    }

    Pastry.prototype.bake = function() {
        console.log("Baking for " + this.bakingTime.get() + " minutes.");
    }
    function Pastry(parent)
    {
        QWObject.call(this, parent);

        this.baked = new QWProperty({ type: Boolean, readonly: true, initialValue: false });

        this.bakingTime = new QWProperty({ type: QWInt });

        this.bakingFinished = QWSignal();

        this.eat = function() {
            console.log("mnompf!");
        }
    }

    var Cake = function(parent)
    {
        Pastry.call(this, parent);

        this.containsRawEgg = new QWProperty({ type: Boolean });
    }
    QW_INHERIT(Cake, Pastry);

    QW_INHERIT(Pizza, Pastry);
    function Pizza(parent)
    {
        Pastry.call(this, parent);

        this.isCalzone = new QWProperty({ type: Boolean, initialValue: false });
        this.topping = new QWProperty({ type: QWVar, initialValue: ["tomato sauce", "mozzarella"] });
    }

    __engine.registerModule({
        Pastry: Pastry,
        Cake: Cake,
        Pizza: Pizza
    });
})();
