import QtQuick
import "../components"

BaseGridItem {
    id: lightBig

    // Basic configuration
    itemName: "LightBig"
    itemType: "light"
    itemColor: "#E91E63"  // Pink/Magenta
    itemText: "Light"
    itemWidth: 4
    itemHeight: 2

    // Custom style
    borderColor: "#C2185B"
    borderWidth: 3
    itemRadius: 8
    textColor: "transparent"  // Hide text by default
    textSize: 14

    // Custom content anchored directly to parent
    Item {
        id: customContent
        anchors.fill: parent
        anchors.margins: borderWidth
    z: 1  // Above default content

    // Main panel with gradient
        Rectangle {
                anchors.fill: parent
                anchors.margins: 6
                radius: 6
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#F48FB1" }
                    GradientStop { position: 0.3; color: "#EC407A" }
                    GradientStop { position: 0.7; color: "#E91E63" }
                    GradientStop { position: 1.0; color: "#AD1457" }
                }

                // Simulated LED grid
                Grid {
                    anchors.fill: parent
                    anchors.margins: 8
                    rows: 3
                    columns: 6
                    spacing: 4

                    Repeater {
                        model: 18
                        Rectangle {
                            width: (parent.width - (parent.columns - 1) * parent.spacing) / parent.columns
                            height: (parent.height - (parent.rows - 1) * parent.spacing) / parent.rows
                            radius: width / 2
                            color: "#FFFFFF"
                            opacity: 0.8

                            // Random animation for each LED
                            SequentialAnimation on opacity {
                                running: true
                                loops: Animation.Infinite
                                NumberAnimation {
                                    to: Math.random() * 0.5 + 0.3
                                    duration: Math.random() * 1000 + 500
                                }
                                NumberAnimation {
                                    to: Math.random() * 0.3 + 0.8
                                    duration: Math.random() * 1000 + 500
                                }
                            }
                        }
                    }
                }

                // Highlight effect
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: parent.height * 0.3
                    anchors.margins: 4
                    radius: 4
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#FFFFFF"; }
                        GradientStop { position: 1.0; color: "transparent"; }
                    }
                    opacity: 0.3
                }
        }

    // Center title
        Text {
                anchors.centerIn: parent
                text: "✨ BIG LIGHT ✨"
                color: "white"
                font.pixelSize: parent.height * 0.15
                font.bold: true
                style: Text.Outline
                styleColor: "#333333"
        }

    // Size badge
        Rectangle {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 4
                width: 30
                height: 15
                color: "#FFFFFF"
                radius: 7

                Text {
                    anchors.centerIn: parent
                    text: "4×2"
                    color: "#E91E63"
                    font.pixelSize: 8
                    font.bold: true
            }
        }
    }
}
