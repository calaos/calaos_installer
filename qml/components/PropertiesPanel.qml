import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: propertiesPanel

    property string currentPageName: "Page 1"
    property string currentPageType: "Default"
    property int gridWidth: 4
    property int gridHeight: 4

    color: "#f0f0f0"
    border.color: "#ccc"
    border.width: 1

    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        clip: true

        ColumnLayout {
            id: propertiesColumn
            width: propertiesPanel.width - 20
            spacing: 15

            // Remote UI Properties Box
            GroupBox {
                title: "Remote UI Properties"
                Layout.fillWidth: true

                GridLayout {
                    anchors.fill: parent
                    columns: 2
                    columnSpacing: 10
                    rowSpacing: 8

                    Label {
                        text: "Grid Width:"
                        Layout.alignment: Qt.AlignVCenter
                    }

                    SpinBox {
                        from: 2
                        to: 8
                        value: gridWidth
                        Layout.fillWidth: true
                        onValueChanged: {
                            gridWidth = value
                        }
                    }

                    Label {
                        text: "Grid Height:"
                        Layout.alignment: Qt.AlignVCenter
                    }

                    SpinBox {
                        from: 2
                        to: 8
                        value: gridHeight
                        Layout.fillWidth: true
                        onValueChanged: {
                            gridHeight = value
                        }
                    }
                }
            }

            // Current Page Properties Box
            GroupBox {
                title: "Current Page Properties"
                Layout.fillWidth: true

                GridLayout {
                    anchors.fill: parent
                    columns: 2
                    columnSpacing: 10
                    rowSpacing: 8

                    Label {
                        text: "Name:"
                        Layout.alignment: Qt.AlignVCenter
                    }

                    TextField {
                        id: pageNameField
                        text: currentPageName
                        Layout.fillWidth: true
                        selectByMouse: true
                        onEditingFinished: {
                            if (text.trim() !== currentPageName) {
                                currentPageName = text.trim()
                                console.log("Page name changed to:", currentPageName)
                            }
                        }
                    }

                    Label {
                        text: "Type:"
                        Layout.alignment: Qt.AlignVCenter
                    }

                    ComboBox {
                        id: pageTypeCombo
                        model: ["Default", "Dashboard", "Settings", "Custom"]
                        currentIndex: model.indexOf(currentPageType)
                        Layout.fillWidth: true
                        onCurrentTextChanged: {
                            if (currentText !== currentPageType) {
                                currentPageType = currentText
                                console.log("Page type changed to:", currentPageType)
                            }
                        }
                    }
                }
            }

            // Item Properties Box
            GroupBox {
                title: "Selected Item Properties"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    Label {
                        text: "No item selected"
                        color: "#999"
                        font.italic: true
                    }
                }
            }

            // Actions Box
            GroupBox {
                title: "Actions"
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    Button {
                        text: "Clear Grid"
                        Layout.fillWidth: true
                        onClicked: {
                            console.log("Clear Grid button clicked")
                        }
                    }

                    Button {
                        text: "Reset Properties"
                        Layout.fillWidth: true
                        onClicked: {
                            gridWidth = 4
                            gridHeight = 4
                            console.log("Properties reset to defaults")
                        }
                    }
                }
            }

            // Spacer to push content to top
            Item {
                Layout.fillHeight: true
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
