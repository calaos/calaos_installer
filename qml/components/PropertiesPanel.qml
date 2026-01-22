import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: propertiesPanel

    property string currentPageName: "Page 1"
    property string currentPageType: "Default"
    property int gridWidth: 4
    property int gridHeight: 4

    // Selected item properties
    property var selectedWidget: null
    property var selectedItemData: null

    // Widget types available from C++ context (set in DialogRemoteUIEditor.cpp)
    property var widgetTypes: availableWidgetTypes

    signal deleteItemRequested()
    signal clearGridRequested()

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

                    // No selection message
                    Label {
                        text: "No item selected"
                        color: "#999"
                        font.italic: true
                        visible: selectedWidget === null
                    }

                    // Item properties (visible when item is selected)
                    GridLayout {
                        visible: selectedWidget !== null
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 10
                        rowSpacing: 8

                        Label {
                            text: "Type:"
                            Layout.alignment: Qt.AlignVCenter
                        }

                        ComboBox {
                            id: itemTypeCombo
                            model: widgetTypes
                            Layout.fillWidth: true
                            currentIndex: selectedItemData ? Math.max(0, model.indexOf(selectedItemData.itemType)) : 0
                            onActivated: {
                                if (selectedWidget && currentText !== selectedWidget.itemType) {
                                    selectedWidget.itemType = currentText
                                    console.log("Item type changed to:", currentText)
                                }
                            }
                        }

                        Label {
                            text: "IO ID:"
                            Layout.alignment: Qt.AlignVCenter
                        }

                        TextField {
                            id: itemIoIdField
                            text: selectedItemData ? (selectedItemData.ioId || "") : ""
                            Layout.fillWidth: true
                            selectByMouse: true
                            placeholderText: "e.g., input_0"
                            onEditingFinished: {
                                if (selectedWidget) {
                                    selectedWidget.ioId = text
                                    console.log("Item ioId changed to:", text)
                                }
                            }
                        }

                        Label {
                            text: "Position:"
                            Layout.alignment: Qt.AlignVCenter
                        }

                        Row {
                            spacing: 5
                            Layout.fillWidth: true

                            Label { text: "X:"; anchors.verticalCenter: parent.verticalCenter }
                            SpinBox {
                                id: itemPosX
                                from: 0
                                to: gridWidth - 1
                                value: selectedItemData ? selectedItemData.startCol : 0
                                width: 60
                                editable: true
                                enabled: false  // Position is read-only (use drag to move)
                            }

                            Label { text: "Y:"; anchors.verticalCenter: parent.verticalCenter }
                            SpinBox {
                                id: itemPosY
                                from: 0
                                to: gridHeight - 1
                                value: selectedItemData ? selectedItemData.startRow : 0
                                width: 60
                                editable: true
                                enabled: false  // Position is read-only (use drag to move)
                            }
                        }

                        Label {
                            text: "Size:"
                            Layout.alignment: Qt.AlignVCenter
                        }

                        Row {
                            spacing: 5
                            Layout.fillWidth: true

                            Label { text: "W:"; anchors.verticalCenter: parent.verticalCenter }
                            SpinBox {
                                id: itemSizeW
                                from: 1
                                to: gridWidth
                                value: selectedItemData ? selectedItemData.itemWidth : 1
                                width: 60
                                editable: true
                                enabled: false  // Size is read-only (use resize handles)
                            }

                            Label { text: "H:"; anchors.verticalCenter: parent.verticalCenter }
                            SpinBox {
                                id: itemSizeH
                                from: 1
                                to: gridHeight
                                value: selectedItemData ? selectedItemData.itemHeight : 1
                                width: 60
                                editable: true
                                enabled: false  // Size is read-only (use resize handles)
                            }
                        }
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
                        text: "Delete Item"
                        Layout.fillWidth: true
                        visible: selectedWidget !== null
                        enabled: selectedWidget !== null

                        onClicked: {
                            console.log("Delete Item button clicked")
                            deleteItemRequested()
                        }
                    }

                    Button {
                        text: "Clear Grid"
                        Layout.fillWidth: true
                        onClicked: {
                            console.log("Clear Grid button clicked")
                            clearGridRequested()
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

    function updateSelectedItem(widget, itemData) {
        selectedWidget = widget
        selectedItemData = itemData
        if (itemData) {
            console.log("PropertiesPanel: Item selected -", itemData.itemType, "at", itemData.startCol, itemData.startRow)
        }
    }

    function clearSelectedItem() {
        selectedWidget = null
        selectedItemData = null
        console.log("PropertiesPanel: Selection cleared")
    }
}
