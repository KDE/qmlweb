__engine.importModule("TestModule", 0, 1);

QW_INHERIT(__comp, TestModule01.Pizza);
function __comp(parent) {
    TestModule01.Pizza.call(this, parent);
    var __ = this.__ctx = new QWContext();
    var __0 = __.pizzaRucola = this;

    __0.bakingTime.set(20);
    __0.topping.set(["tomato sauce", "mozzarella", "tomato slices", "rucola"]);
}

__comp;

