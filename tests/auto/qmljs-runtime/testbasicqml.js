QUnit.test("Minimal - just an object", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimal/minimal.qml");
    var object = component.create();
    assert.ok(object, "Object exists!" );
});

QUnit.test("Minimal id test - empty object with an id.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimal/minimalidtest.qml");
    var object = component.create();
    assert.strictEqual(QWQmlEngine.contextForObject(object).nameForObject(object), "grandma", "Object id is correct.");
});

QUnit.test("Minimal property - object and objectName", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/properties/minimalproperty.qml");
    var object = component.create();
    assert.strictEqual(object.objectName.get(), "hugo", "Object name is correct.");
});

QUnit.test("Minimal property declaration - object and a self declared property", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/properties/minimalpropertydeclaration.qml");
    var object = component.create();
    assert.strictEqual(object.test.get(), "hello world!", "Property value is correct.");
});

QUnit.test("Minimal function declaration - object and a self declared function", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimal/minimalfunction.qml");
    var object = component.create();
    assert.strictEqual(typeof object.heyo, "function", "Function is valid.");
    assert.strictEqual(object.heyo(2), 42, "Function return value is correct.");
});

QUnit.test("Minimal binding - without reference to any non-constant value", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/bindings/minimalbindingwithoutreference.qml");
    var object = component.create();
    assert.strictEqual(object.objectName.get(), "300", "Property    value is correct.");
});

QUnit.test("Minimal binding with a reference to an own property", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/bindings/minimalbindingbasicreference.qml");
    var object = component.create();
    assert.strictEqual(object.objectName.get(), "hello", "Object name is correct.");
    assert.strictEqual(object.extendedName.get(), "hello world!", "Property value is correct.");
});

QUnit.test("Minimal element property - one element with a property that is an element itself (child element).", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/properties/minimalelementproperty.qml");
    var object = component.create();
    assert.strictEqual(typeof object.child.get(), "object", "Child is an object.");
    assert.strictEqual(object.child.get().objectName.get(), "childObject", "Child has correct object name.");
});

QUnit.test("Minimal element list - one element with a list property containing 5 child elements.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/properties/minimalelementlist.qml");
    var object = component.create();
    assert.ok(object.children.get() instanceof QWList, "Children list is a QWList.");
    assert.ok(object.children.get() instanceof Array, "Children list is an Array.");
    assert.strictEqual(typeof object.children.get()[0], "object", "First child is an object.");
    assert.strictEqual(object.children.get()[0].objectName.get(), "el1", "First child has correct object name.");
    assert.strictEqual(typeof object.children.get()[1], "object", "Second child is an object.");
    assert.strictEqual(object.children.get()[1].objectName.get(), "el2", "Second child has correct object name.");
    assert.strictEqual(typeof object.children.get()[2], "object", "Third child is an object.");
    assert.strictEqual(object.children.get()[2].objectName.get(), "el3", "Third child has correct object name.");
    assert.strictEqual(typeof object.children.get()[3], "object", "Fourth child is an object.");
    assert.strictEqual(object.children.get()[3].objectName.get(), "el4", "Fourth child has correct object name.");
    assert.strictEqual(typeof object.children.get()[4], "object", "Fifth child is an object.");
    assert.strictEqual(object.children.get()[4].objectName.get(), "el5", "Fifth child has correct object name.");
});

QUnit.test("Minimal module - one element with a type that is imported from a module.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/imports/minimalmodule.qml");
    var object = component.create();
    assert.ok(window.__qmljsTestModuleDefined, "Module is defined.");
    assert.strictEqual(object.bakingTime.get(), 20, "Inherited property is ok.");
    assert.strictEqual(object.topping.get().length, 4, "They didn't forget a topping in the kitchen (aka own property is ok).");
    assert.strictEqual(object.topping.get()[2], "tomato slices", "Tomato is still fresh (aka own property is ok).");
    assert.strictEqual(object.isCalzone.get(), false, "They made it right (aka unset property is ok).");
});

QUnit.test("Minimal module - ensure that modules aren't loaded twice.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/imports/minimalmodule.qml");
    var object = component.create();
    assert.ok(window.__qmljsTestModuleDefined, "Module is defined.");
    assert.strictEqual(object.bakingTime.get(), 20, "Inherited property is ok.");
    assert.strictEqual(object.topping.get().length, 4, "They didn't forget a topping in the kitchen (aka own property is ok).");
    assert.strictEqual(object.topping.get()[2], "tomato slices", "Tomato is still fresh (aka own property is ok).");
    assert.strictEqual(object.isCalzone.get(), false, "They made it right (aka unset property is ok).");
});

QUnit.test("Minimal basic type with an attribute.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/properties/minimalbasictypewithattributes.qml");
    var object = component.create();
    assert.strictEqual(object.someProp.getAttr(0), 255, "Attribute r has the correct value.");
    assert.strictEqual(object.someProp.getAttr(1), 0, "Attribute g has the correct value.");
    assert.strictEqual(object.someProp.getAttr(2), 136, "Attribute b has the correct value.");
    assert.strictEqual(object.otherProp.getAttr(0), 0, "Second property's r has the correct value.");
    assert.strictEqual(object.otherProp.getAttr(1), 110, "Second property's g has the correct value.");
    assert.strictEqual(object.otherProp.getAttr(2), 0, "Second property's b has the correct value.");
});

QUnit.test("Minimal signal declaration - no use.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/signals/minimalsignaldeclaration.qml");
    var object = component.create();
    assert.strictEqual(typeof object.someSignal, "function", "Signal is a function.");
    assert.strictEqual(typeof object.someSignal.connect, "function", "Signal has a connect method.");
});

QUnit.test("Minimal onSignal handler.", function(assert) {
    assert.expect(2);
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/signals/minimalonsignalhandler.qml");
    var object = component.create();
    assert.strictEqual(typeof object.someSignal, "function", "Signal is a function.");
    object.someSignal(assert);
});

QUnit.test("Minimal property binding update - Changing a property another property is binded to.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/bindings/minimalbindingupdate.qml");
    var object = component.create();
    assert.strictEqual(object.prop2.get(), 35, "Property value is correct before changing it.");
    object.prop1.set(19);
    assert.strictEqual(object.prop2.get(), 133, "Property value is correct after changing it.");
});

QUnit.test("Binding with id reference - Binding to a property that belongs to another object using its id.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/bindings/bindingwithidreference.qml");
    var object = component.create();
    assert.strictEqual(object.child.get().prop3.get(), 15, "Prop3 value is correct before changing Prop2.");
    assert.strictEqual(object.prop1.get(), 10, "Prop1 value is correct before changing Prop2.");
    object.prop2.set(19);
    assert.strictEqual(object.child.get().prop3.get(), 29, "Prop3 value is correct after changing Prop2.");
    assert.strictEqual(object.prop1.get(), 24, "Prop1 value is correct after changing Prop2.");
});

QUnit.test("Inline component - simplest case", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/components/simpleinlinecomponent.qml");
    var object = component.create();
    assert.ok(object.comp.get(), "comp exists.");
    assert.ok(object.comp.get() instanceof QWQmlComponent, "comp is actually a component.");
    assert.ok(object.obj.get(), "obj exists.");
    assert.ok(object.obj.get() instanceof QWObject, "obj is actually a QtObject.");
    assert.strictEqual(object.obj.get().objectName.get(), "KDE", "obj has the correct Object name.");
});

QUnit.test("File component - simplest case", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/components/simplefilecomponent.qml");
    var object = component.create();
    assert.ok(object.obj.get(), "obj exists.");
    assert.ok(object.obj.get() instanceof QWObject, "obj is actually a QtObject.");
    assert.strictEqual(object.obj.get().objectName.get(), "hugo", "obj has the correct Object name.");

    // Ensure that files aren't loaded twice - will throw an exception, if it is.
    var engine = new QWQmlEngine();
    var component2 = new QWQmlComponent(engine, "../data/simplefilecomponent.qml");
    var object2 = component.create();
});
