
QtObject {
    property int a: 467
    property int b: 323
    property QtObject obj: QtObject {
        id: bob
        property string profession: "Builder"
    }

    property QtObject comp: AccessOuterContextComponent {}
}


