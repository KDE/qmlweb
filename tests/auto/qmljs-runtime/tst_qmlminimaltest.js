QUnit.test("QML minimal test", function(assert) {
    var engine = new QmlEngine();
    var component = new QmlComponent(engine, "data/qmlminimaltest.qml");
    var object = component.create();
    assert.ok(object, "Object exists!" );
    assert.ok(QW_GET(object, 0), "hugo", "Object name is correct.");
    assert.ok(QW_GET(object, 1), "hello world!", "Property value is correct.");
});