import QtQuick
import "../components"

BaseGridItem {
    id: rectangleSmall

    itemName: "RectangleSmall"
    itemType: "RectangleSmall"
    itemColor: "#ff4444"
    itemText: "1×1"

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
