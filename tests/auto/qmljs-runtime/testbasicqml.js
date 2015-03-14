QUnit.test("QML minimal - just an object", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimal.qml");
    var object = component.create();
    assert.ok(object, "Object exists!" );
});

QUnit.test("QML minimal property - object and objectName", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalproperty.qml");
    var object = component.create();
    assert.ok(QW_GET(object, 0), "hugo", "Object name is correct.");
});

QUnit.test("QML minimal property declaration - object and a self declared property", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/minimalpropertydeclaration.qml");
    var object = component.create();
    assert.ok(QW_GET(object, 1), "hello world!", "Property value is correct.");
});