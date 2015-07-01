(function() {
    QW_INHERIT(Pastry, QWObject);

    if (window.__qmljsTestModuleDefined)
        throw "TestModule defined twice.";
    window.__qmljsTestModuleDefined = true;

    function Oven(parent)
    {
        QWObject.call(this, parent);

        this.temperature = new QWProperty({ type: QWInt, initialValue: 20 });
        this.content = new QWProperty({ type: "list<Pastry>", initialValue: new QWList() });
    }

    function Pastry(parent)
    {
        QWObject.call(this, parent);

        this.bakingTime = new QWProperty({ type: QWInt });
    }

    var Cake = function(parent)
    {
        Pastry.call(this, parent);

    }
    QW_INHERIT(Cake, Pastry);

    QW_INHERIT(Cake, Pastry);
    function Pizza(parent)
    {
        Pastry.call(this, parent);

        this.isCalzone = new QWProperty({ type: Boolean, initialValue: false });
        this.topping = new QWProperty({ type: QWVar, initialValue: ["tomato sauce", "mozzarella"] });
    }

    return {
        Pastry: Pastry,
        Cake: Cake,
        Pizza: Pizza
    }
})();
