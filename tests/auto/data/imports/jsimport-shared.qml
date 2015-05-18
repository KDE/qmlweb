import "shared.js" as JS

QtObject {
    id: karl

    property int x: JS.xProp
    property bool ok: true

    function increase() {
        x = JS.add(x, 5);
    }
}
