(function() {
    QW_INHERIT(Pastry, QWObject);
    function Pastry(parent)
    {
        QWObject.call(this, parent);

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

    }

    return {
        Pastry: Pastry,
        Cake: Cake,
        Pizza: Pizza
    }
})();
