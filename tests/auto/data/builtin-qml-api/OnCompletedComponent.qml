
QtObject {
    property int innerOnCompletedCount: 0

    Component.onCompleted: innerOnCompletedCount++;
}
