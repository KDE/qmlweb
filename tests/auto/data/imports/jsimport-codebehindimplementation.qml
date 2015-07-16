import "codebehindimplementation.js" as JS

QtObject {
    id: karl

    property int x: JS.xProp

    function increase() {
        JS.add(5);
    }
}
