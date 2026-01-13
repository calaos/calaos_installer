import QtQuick
import "../components"

BaseGridItem {
    id: lightSmall

    // Basic configuration
    itemName: "LightSmall"
    itemType: "light"
    itemColor: "#FFE135"  // Light yellow
    itemText: "Light"
    itemWidth: 1
    itemHeight: 1

    // Custom style
    borderColor: "#FFC107"
    borderWidth: 2
    itemRadius: 8
    textColor: "transparent"  // Hide text by default
    textSize: 16

    // Custom content anchored directly to parent
    Item {
        id: customContent
        anchors.fill: parent
        anchors.margins: borderWidth
    z: 1  // Above default content

    // Light halo effect
        Rectangle {
            anchors.centerIn: parent
            width: parent.width * 0.6
            height: parent.height * 0.6
            color: "transparent"
            border.color: "#FFE135"
            border.width: 1
            radius: width / 2
            opacity: 0.7

            // Pulsing animation
            SequentialAnimation on opacity {
                running: true
                loops: Animation.Infinite
                NumberAnimation { to: 0.3; duration: 1000 }
                NumberAnimation { to: 0.7; duration: 1000 }
            }
        }

    // Lamp icon
        Text {
            anchors.centerIn: parent
            text: "💡"
            font.pixelSize: parent.height * 0.5
            color: "#333333"
        }

    // Identification text
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 2
            text: "S"
            color: "#666666"
            font.pixelSize: 8
            font.bold: true
        }
    }
}
