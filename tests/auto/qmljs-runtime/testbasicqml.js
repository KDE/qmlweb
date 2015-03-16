QUnit.test("Minimal - just an object", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimal.qml");
    var object = component.create();
    assert.ok(object, "Object exists!" );
});

QUnit.test("Minimal id test - empty object with an id.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalidtest.qml");
    var object = component.create();
    assert.strictEqual(QWQmlEngine.contextForObject(object).nameForObject(object), "grandma", "Object id is correct.");
});

QUnit.test("Minimal property - object and objectName", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalproperty.qml");
    var object = component.create();
    assert.strictEqual(QW_GET(object, 0), "hugo", "Object name is correct.");
});

QUnit.test("Minimal property declaration - object and a self declared property", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalpropertydeclaration.qml");
    var object = component.create();
    assert.strictEqual(QW_GET(object, 1), "hello world!", "Property value is correct.");
});

QUnit.test("Minimal function declaration - object and a self declared function", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalfunction.qml");
    var object = component.create();
    assert.strictEqual(typeof object.heyo, "function", "Function is valid.");
    assert.strictEqual(object.heyo(2), 42, "Function return value is correct.");
});

QUnit.test("Minimal binding - without reference to any non-constant value", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalbindingwithoutreference.qml");
    var object = component.create();
    assert.strictEqual(QW_GET(object, 0), "300", "Property    value is correct.");
});

QUnit.test("Minimal binding with a reference to an own property", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalbindingbasicreference.qml");
    var object = component.create();
    assert.strictEqual(QW_GET(object, 0), "hello", "Object name is correct.");
    assert.strictEqual(QW_GET(object, 1), "hello world!", "Property value is correct.");
});

QUnit.test("Minimal element property - one element with a property that is an element itself (child element).", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalelementproperty.qml");
    var object = component.create();
    assert.strictEqual(typeof QW_GET(object, 1), "object", "Child is an object.");
    assert.strictEqual(QW_GET(QW_GET(object, 1), 0), "childObject", "Child has correct object name.");
});

QUnit.test("Minimal element list - one element with a list property containing 5 child elements.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalelementlist.qml");
    var object = component.create();
    assert.ok(QW_GET(object, 1) instanceof QWList, "Children list is a QWList.");
    assert.ok(QW_GET(object, 1) instanceof Array, "Children list is an Array.");
    assert.strictEqual(typeof QW_GET(object, 1)[0], "object", "First child is an object.");
    assert.strictEqual(QW_GET(QW_GET(object, 1)[0], 0), "el1", "First child has correct object name.");
    assert.strictEqual(typeof QW_GET(object, 1)[1], "object", "Second child is an object.");
    assert.strictEqual(QW_GET(QW_GET(object, 1)[1], 0), "el2", "Second child has correct object name.");
    assert.strictEqual(typeof QW_GET(object, 1)[2], "object", "Third child is an object.");
    assert.strictEqual(QW_GET(QW_GET(object, 1)[2], 0), "el3", "Third child has correct object name.");
    assert.strictEqual(typeof QW_GET(object, 1)[3], "object", "Fourth child is an object.");
    assert.strictEqual(QW_GET(QW_GET(object, 1)[3], 0), "el4", "Fourth child has correct object name.");
    assert.strictEqual(typeof QW_GET(object, 1)[4], "object", "Fifth child is an object.");
    assert.strictEqual(QW_GET(QW_GET(object, 1)[4], 0), "el5", "Fifth child has correct object name.");
});

QUnit.test("Minimal basic type with an attribute.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalbasictypewithattributes.qml");
    var object = component.create();
    assert.strictEqual(QW_GETATTR(object, 1, 0), 255, "Attribute r has the correct value.");
    assert.strictEqual(QW_GETATTR(object, 1, 1), 0, "Attribute g has the correct value.");
    assert.strictEqual(QW_GETATTR(object, 1, 2), 136, "Attribute b has the correct value.");
    assert.strictEqual(QW_GETATTR(object, 2, 0), 0, "Second property's r has the correct value.");
    assert.strictEqual(QW_GETATTR(object, 2, 1), 110, "Second property's g has the correct value.");
    assert.strictEqual(QW_GETATTR(object, 2, 2), 0, "Second property's b has the correct value.");
});

QUnit.test("Minimal signal declaration - no use.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalsignaldeclaration.qml");
    var object = component.create();
    assert.strictEqual(typeof object.someSignal, "function", "Signal is a function.");
    assert.strictEqual(typeof object.someSignal.connect, "function", "Signal has a connect method.");
});

QUnit.test("Minimal onSignal handler.", function(assert) {
    assert.expect(2);
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalonsignalhandler.qml");
    var object = component.create();
    assert.strictEqual(typeof object.someSignal, "function", "Signal is a function.");
    object.someSignal(assert);
});

QUnit.test("Minimal property binding update - Changing a property another property is binded to.", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalbindingupdate.qml");
    var object = component.create();
    assert.strictEqual(QW_GET(object, 2), 35, "Property value is correct before changing it.");
    QW_SET(object, 1, 19);
    assert.strictEqual(QW_GET(object, 2), 133, "Property value is correct after changing it.");
});
