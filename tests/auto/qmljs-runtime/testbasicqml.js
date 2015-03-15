QUnit.test("Minimal - just an object", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimal.qml");
    var object = component.create();
    assert.ok(object, "Object exists!" );
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
