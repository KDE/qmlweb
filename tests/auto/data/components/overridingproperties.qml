
QtObject {
    property QtObject obj: MinimalComponent {
        objectName: "karl"
        greeting: "Hey, guys!"
        x: 5 * objectName.length
        y: 3*x
    }
}


