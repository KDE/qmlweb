
QtObject {
    id: main
    property int prop1: foo.prop3 - 5
    property int prop2: 5

    property QtObject child: QtObject {
        id: foo
        property int prop3: main.prop2 + 10
    }
}
