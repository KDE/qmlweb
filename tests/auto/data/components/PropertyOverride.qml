
QtObject {
    objectName: "hugo"
    property string message: "I don't have a message, I'm only a dummy text."
    property string greeting: "Hello, " + objectName + "!"
    property string closing: "Bye, " + objectName + "!"
    property int x: 27
    property int y: 2 * x
}
