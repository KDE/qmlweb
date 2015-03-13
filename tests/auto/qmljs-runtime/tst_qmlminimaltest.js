QUnit.test("QML minimal test", function(assert) {
    var engine = new QmlEngine();
    var component = new QmlComponent(engine, "data/qmlminimaltest.qml");
    var object = component.create();
    assert.ok(object, "Object exists!" );
    assert.ok(object.test, "hallo", "Property value is correct.");
});