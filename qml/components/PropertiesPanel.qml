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

    // IO info for the selected item (populated via validateIO or ioSelected signal)
    property bool ioExists: false
    property string ioId: ""
    property string ioName: ""
    property string roomName: ""
    property string widgetType: ""

    // Widget types available from C++ context (set in DialogRemoteUIEditor.cpp)
    property var widgetTypes: availableWidgetTypes

    signal deleteItemRequested()
    signal clearGridRequested()
    signal ioChanged(string newIoId, string newWidgetType)

    color: "#f0f0f0"
    border.color: "#ccc"
    border.width: 1

    // Expose minimum content width for SplitView
    readonly property int minimumContentWidth: propertiesColumn.implicitWidth + 32

    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        contentWidth: availableWidth

        ColumnLayout {
            id: propertiesColumn
            width: Math.max(implicitWidth, propertiesPanel.width - 32)
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
                        Layout.minimumWidth: implicitWidth
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
                        Layout.minimumWidth: implicitWidth
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
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 8

                    // No selection message
                    Label {
                        text: "No item selected"
                        color: "#999"
                        font.italic: true
                        visible: selectedWidget === null
                    }

                    // Item properties (visible when item is selected)
                    ColumnLayout {
                        visible: selectedWidget !== null
                        Layout.fillWidth: true
                        spacing: 12

                        // Widget Type (readonly, auto-detected from IO)
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Widget Type:"
                            }

                            TextField {
                                id: itemTypeField
                                text: widgetType || "Not linked"
                                Layout.fillWidth: true
                                readOnly: true
                                color: widgetType ? "black" : "#999"
                                font.italic: !widgetType
                            }
                        }

                        // IO Selection
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Linked IO:"
                            }

                            // IO info or warning
                            Rectangle {
                                Layout.fillWidth: true
                                implicitHeight: ioDisplayColumn.implicitHeight + 8
                                Layout.minimumHeight: 40
                                color: !ioId ? "#f5f5f5" : (ioExists ? "#e8f5e9" : "#ffebee")
                                border.color: !ioId ? "#ccc" : (ioExists ? "#81c784" : "#e57373")
                                border.width: 1
                                radius: 4

                                ColumnLayout {
                                    id: ioDisplayColumn
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.margins: 4
                                    spacing: 2

                                    // Not linked state
                                    Label {
                                        visible: !ioId
                                        text: "Not linked to any IO"
                                        color: "#999"
                                        font.italic: true
                                        Layout.fillWidth: true
                                        wrapMode: Text.WordWrap
                                    }

                                    // IO exists state
                                    Label {
                                        visible: ioId && ioExists
                                        text: ioName || ""
                                        font.bold: true
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        visible: ioId && ioExists
                                        text: roomName ? ("📍 " + roomName) : ""
                                        color: "#666"
                                        font.pixelSize: 11
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }

                                    // IO not found state (warning)
                                    RowLayout {
                                        visible: ioId && !ioExists
                                        Layout.fillWidth: true
                                        spacing: 4

                                        Label {
                                            text: "⚠️"
                                            font.pixelSize: 14
                                        }
                                        ColumnLayout {
                                            spacing: 0
                                            Label {
                                                text: "IO not found"
                                                color: "#c62828"
                                                font.bold: true
                                            }
                                            Label {
                                                text: ioId
                                                color: "#666"
                                                font.pixelSize: 10
                                                elide: Text.ElideRight
                                            }
                                        }
                                    }
                                }
                            }

                            // Select IO button
                            Button {
                                text: ioId ? "Change IO..." : "Select IO..."
                                Layout.fillWidth: true
                                onClicked: {
                                    console.log("Opening IO selector with current:", ioId)
                                    remoteUIEditor.openIOSelector(ioId)
                                }
                            }
                        }

                        // Position
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Position:"
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                Label { text: "X:" }
                                SpinBox {
                                    id: itemPosX
                                    from: 0
                                    to: gridWidth - 1
                                    value: selectedItemData ? selectedItemData.startCol : 0
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: implicitWidth
                                    editable: true
                                    enabled: false
                                }

                                Label { text: "Y:" }
                                SpinBox {
                                    id: itemPosY
                                    from: 0
                                    to: gridHeight - 1
                                    value: selectedItemData ? selectedItemData.startRow : 0
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: implicitWidth
                                    editable: true
                                    enabled: false
                                }
                            }
                        }

                        // Size
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Size:"
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                Label { text: "W:" }
                                SpinBox {
                                    id: itemSizeW
                                    from: 1
                                    to: gridWidth
                                    value: selectedItemData ? selectedItemData.itemWidth : 1
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: implicitWidth
                                    editable: true
                                    enabled: false
                                }

                                Label { text: "H:" }
                                SpinBox {
                                    id: itemSizeH
                                    from: 1
                                    to: gridHeight
                                    value: selectedItemData ? selectedItemData.itemHeight : 1
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: implicitWidth
                                    editable: true
                                    enabled: false
                                }
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

            // Validate the IO and update display
            var currentIoId = itemData.ioId || ""
            if (currentIoId) {
                var ioInfo = remoteUIEditor.validateIO(currentIoId)
                ioId = currentIoId
                ioExists = ioInfo.exists
                ioName = ioInfo.ioName || ""
                roomName = ioInfo.roomName || ""
                widgetType = ioInfo.widgetType || ""
            } else {
                // No IO linked
                ioId = ""
                ioExists = false
                ioName = ""
                roomName = ""
                widgetType = ""
            }
        } else {
            clearIOInfo()
        }
    }

    function clearSelectedItem() {
        selectedWidget = null
        selectedItemData = null
        clearIOInfo()
        console.log("PropertiesPanel: Selection cleared")
    }

    function clearIOInfo() {
        ioId = ""
        ioExists = false
        ioName = ""
        roomName = ""
        widgetType = ""
    }

    // Called when an IO is selected from the dialog
    function onIOSelected(newIoId, newIoName, newRoomName, newWidgetType) {
        ioId = newIoId
        ioExists = true
        ioName = newIoName
        roomName = newRoomName
        widgetType = newWidgetType

        console.log("PropertiesPanel: IO selected -", newIoId, newIoName, "in", newRoomName, "widget:", newWidgetType)

        // Emit signal to update the widget and model
        ioChanged(newIoId, newWidgetType)
    }
}
