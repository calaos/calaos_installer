import QtQuick
import "../components"

BaseGridItem {
    id: lightMedium

    // Basic configuration
    itemName: "LightMedium"
    itemType: "light"
    itemColor: "#FF9800"  // Orange
    itemText: "Light"
    itemWidth: 4
    itemHeight: 1

    // Custom style
    borderColor: "#F57C00"
    borderWidth: 2
    itemRadius: 6
    textColor: "transparent"  // Hide text by default
    textSize: 12

    // Custom content anchored directly to parent
    Item {
        id: customContent
        anchors.fill: parent
        anchors.margins: borderWidth
    z: 1  // Above default content        // Main light strip
        Rectangle {
                anchors.fill: parent
                anchors.margins: 4
                color: "transparent"
                border.color: "#FFB74D"
                border.width: 1
                radius: 4

                // Light gradient
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 2
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#FFE082" }
                        GradientStop { position: 0.5; color: "#FFCC02" }
                        GradientStop { position: 1.0; color: "#FFE082" }
                    }
                    radius: 2
                    opacity: 0.6
                }

                // Sweep animation
                Rectangle {
                    id: sweepLight
                    width: parent.width * 0.2
                    height: parent.height
                    color: "#FFFFFF"
                    opacity: 0.4
                    radius: 2

                    SequentialAnimation on x {
                        running: true
                        loops: Animation.Infinite
                        NumberAnimation {
                            from: -sweepLight.width
                            to: parent.width
                            duration: 2000
                        }
                        PauseAnimation { duration: 1000 }
                    }
                }
        }

    // Distributed icons
        Row {
                anchors.centerIn: parent
                spacing: parent.width * 0.15
                Repeater {
                    model: 4
                    Text {
                        text: "🔆"
                        font.pixelSize: parent.parent.height * 0.3
                        color: "white"
                    }
                }
        }

    // Label
        Text {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 4
                text: "Medium"
                color: "white"
                font.pixelSize: 8
                font.bold: true
        }
    }
}
