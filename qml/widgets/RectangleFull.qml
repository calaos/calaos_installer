import QtQuick
import "../components"

BaseGridItem {
    id: rectangleFull

    itemName: "RectangleFull"
    itemType: "RectangleFull"
    itemColor: "#44ff44"
    itemText: "4×1"

    Rectangle {
        anchors.fill: parent
        color: itemColor
        border.color: "#333333"
        border.width: 1
        radius: 4

        Text {
            anchors.centerIn: parent
            text: itemText
            color: "white"
            font.bold: true
            font.pixelSize: 12
        }
    }
}
