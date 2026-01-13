import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: propertiesPanel

    property string currentPageName: "Page 1"
    property string currentPageType: "Default"
    property int gridWidth: 4
    property int gridHeight: 4
    property int uiWidth: 800
    property int uiHeight: 600

    color: "#f0f0f0"
    border.color: "#ccc"
    border.width: 1

    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: propertiesColumn.width
        contentHeight: propertiesColumn.height

        Column {
            id: propertiesColumn
            width: propertiesPanel.width - 20
            spacing: 15

            // Remote UI Properties Box
            Rectangle {
                width: parent.width
                height: 160
                color: "white"
                border.color: "#ddd"
                border.width: 1
                radius: 4

                Column {
                    id: remoteUIColumn
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Text {
                        text: "Remote UI Properties"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#333"
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#eee"
                    }

                    GridLayout {
                        width: parent.width
                        columns: 2
                        columnSpacing: 10
                        rowSpacing: 5

                        Text {
                            text: "Width:"
                            font.pixelSize: 11
                            color: "#666"
                        }

                        SpinBox {
                            from: 400
                            to: 2000
                            value: uiWidth
                            Layout.preferredWidth: 80
                            onValueChanged: {
                                uiWidth = value
                            }
                        }

                        Text {
                            text: "Height:"
                            font.pixelSize: 11
                            color: "#666"
                        }

                        SpinBox {
                            from: 300
                            to: 1500
                            value: uiHeight
                            Layout.preferredWidth: 80
                            onValueChanged: {
                                uiHeight = value
                            }
                        }

                        Text {
                            text: "Grid:"
                            font.pixelSize: 11
                            color: "#666"
                        }

                        Row {
                            spacing: 5
                            SpinBox {
                                from: 2
                                to: 8
                                value: gridWidth
                                Layout.preferredWidth: 60
                                onValueChanged: {
                                    gridWidth = value
                                }
                            }
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: "×"
                                font.pixelSize: 11
                            }
                            SpinBox {
                                from: 2
                                to: 8
                                value: gridHeight
                                Layout.preferredWidth: 60
                                onValueChanged: {
                                    gridHeight = value
                                }
                            }
                        }
                    }
                }
            }

            // Current Page Properties Box
            Rectangle {
                width: parent.width
                height: 140
                color: "white"
                border.color: "#ddd"
                border.width: 1
                radius: 4

                Column {
                    id: pageColumn
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Text {
                        text: "Current Page Properties"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#333"
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#eee"
                    }

                    GridLayout {
                        width: parent.width
                        columns: 2
                        columnSpacing: 10
                        rowSpacing: 8

                        Text {
                            text: "Name:"
                            font.pixelSize: 11
                            color: "#666"
                        }

                        TextField {
                            id: pageNameField
                            text: currentPageName
                            font.pixelSize: 11
                            Layout.preferredWidth: 120
                            selectByMouse: true
                            onEditingFinished: {
                                if (text.trim() !== currentPageName) {
                                    currentPageName = text.trim()
                                    console.log("Page name changed to:", currentPageName)
                                }
                            }
                        }

                        Text {
                            text: "Type:"
                            font.pixelSize: 11
                            color: "#666"
                        }

                        ComboBox {
                            id: pageTypeCombo
                            model: ["Default", "Dashboard", "Settings", "Custom"]
                            currentIndex: model.indexOf(currentPageType)
                            font.pixelSize: 11
                            Layout.preferredWidth: 120
                            onCurrentTextChanged: {
                                if (currentText !== currentPageType) {
                                    currentPageType = currentText
                                    console.log("Page type changed to:", currentPageType)
                                }
                            }
                        }
                    }
                }
            }

            // Item Properties Box (placeholder)
            Rectangle {
                width: parent.width
                height: 80
                color: "white"
                border.color: "#ddd"
                border.width: 1
                radius: 4

                Column {
                    id: itemColumn
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Text {
                        text: "Selected Item Properties"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#333"
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#eee"
                    }

                    Text {
                        text: "No item selected"
                        font.pixelSize: 11
                        color: "#999"
                        font.italic: true
                    }
                }
            }

            // Actions Box
            Rectangle {
                width: parent.width
                height: 120
                color: "white"
                border.color: "#ddd"
                border.width: 1
                radius: 4

                Column {
                    id: actionsColumn
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Text {
                        text: "Actions"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#333"
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#eee"
                    }

                    Column {
                        width: parent.width
                        spacing: 5

                        Button {
                            width: parent.width
                            text: "Clear Grid"
                            font.pixelSize: 11
                            onClicked: {
                                console.log("Clear Grid button clicked")
                            }
                        }

                        Button {
                            width: parent.width
                            text: "Reset Properties"
                            font.pixelSize: 11
                            onClicked: {
                                uiWidth = 800
                                uiHeight = 600
                                gridWidth = 4
                                gridHeight = 4
                                console.log("Properties reset to defaults")
                            }
                        }
                    }
                }
            }
        }
    }

    function updatePageInfo(pageName, pageType) {
        currentPageName = pageName
        currentPageType = pageType
        pageNameField.text = pageName
        pageTypeCombo.currentIndex = pageTypeCombo.model.indexOf(pageType)
    }
}