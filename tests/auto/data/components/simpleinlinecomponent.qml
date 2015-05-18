
QtObject {
    property QtObject obj: comp.createObject();
    property Component comp: Component {
        QtObject {
            objectName: "KDE"
        }
    }
}
