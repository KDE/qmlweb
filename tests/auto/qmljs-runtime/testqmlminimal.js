QUnit.test("QML minimal test", function(assert) {
    var engine = new QWQmlEngine();
    var component = new QWQmlComponent(engine, "../data/qmlminimaltest.qml");
    var object = component.create();
    assert.ok(object, "Object exists!" );
    assert.ok(QW_GET(object, 0), "hugo", "Object name is correct.");
    assert.ok(QW_GET(object, 1), "hello world!", "Property value is correct.");
});