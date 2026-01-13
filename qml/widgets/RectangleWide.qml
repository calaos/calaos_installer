import QtQuick
import "../components"

BaseGridItem {
    id: rectangleWide

    itemName: "RectangleWide"
    itemType: "RectangleWide"
    itemColor: "#4444ff"
    itemText: "2×1"

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
