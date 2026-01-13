import QtQuick
import "../components"

BaseGridItem {
    id: rectangleLarge

    itemName: "RectangleLarge"
    itemType: "RectangleLarge"
    itemColor: "#8844ff"
    itemText: "4×2"

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
