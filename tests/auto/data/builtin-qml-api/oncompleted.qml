
OnCompletedComponent {
    property int outerOnCompletedCount: 0

    Component.onCompleted: outerOnCompletedCount++;
}
