(function() {
    QW_INHERIT(Pastry, QWObject);

    function Oven(parent)
    {
        QWObject.call(this, parent);

        this.temperature = new QWProperty({ type: "int", initialValue: 20 });
        this.content = new QWProperty({ type: "list<Pastry>", initialValue: new QWList() });
    }

    function Pastry(parent)
    {
        QWObject.call(this, parent);

        this.bakingTime = new QWProperty({ type: "int" });
    }

    QW_INHERIT(Cake, Pastry);
    function Cake(parent)
    {
        Pastry.call(this, parent);

    }

    QW_INHERIT(Cake, Pastry);
    function Pizza(parent)
    {
        Pastry.call(this, parent);

        this.isCalzone = new QWProperty({ type: "bool", initialValue: false });
        this.topping = new QWProperty({ type: "var", initialValue: ["tomato sauce", "mozzarella"] });
    }

    return {
        Pastry: Pastry,
        Cake: Cake,
        Pizza: Pizza
    }
})();
