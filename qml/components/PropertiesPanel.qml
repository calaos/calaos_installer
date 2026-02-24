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
    property string widgetNameOverride: ""

    // Widget category for the selected item: "IO" or "Clock"
    property string widgetCategory: "IO"

    // Clock properties for the selected item
    property string clockTimezone: "UTC"
    property string clockFormat: "24"
    property bool clockShowDate: true
    property string clockDateFormat: "DD/MM/YYYY"
    property bool clockSeconds: false

    // Widget types available from C++ context (set in DialogRemoteUIEditor.cpp)
    property var widgetTypes: availableWidgetTypes

    signal deleteItemRequested()
    signal clearGridRequested()
    signal ioChanged(string newIoId, string newWidgetType)
    signal nameOverrideChanged(string newName)
    signal categoryChanged(string newCategory)
    signal clockPropertyChanged(string key, string value)

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
                        model: ["default"]
                        currentIndex: 0
                        Layout.fillWidth: true
                        enabled: false  // Disabled for now, only default type supported
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

                        // Widget Category selector
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Widget Category:"
                            }

                            ComboBox {
                                id: categoryCombo
                                model: availableCategories
                                Layout.fillWidth: true
                                currentIndex: {
                                    var idx = availableCategories.indexOf(widgetCategory)
                                    return idx >= 0 ? idx : 0
                                }
                                onActivated: function(index) {
                                    var newCat = availableCategories[index]
                                    if (newCat !== widgetCategory) {
                                        console.log("Category changed to:", newCat)
                                        widgetCategory = newCat
                                        categoryChanged(newCat)
                                    }
                                }
                            }
                        }

                        // ============================================
                        // IO Panel — visible only when category is IO
                        // ============================================
                        ColumnLayout {
                            id: ioPanelSection
                            visible: widgetCategory === "IO"
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
                                            text: roomName ? ("\ud83d\udccd " + roomName) : ""
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
                                                text: "\u26a0\ufe0f"
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

                            // Display Name Override
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                Label {
                                    text: "Display Name:"
                                }

                                Label {
                                    text: "Optional \u2014 overrides the IO name on the remote UI"
                                    color: "#888"
                                    font.pixelSize: 11
                                    font.italic: true
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 4

                                    TextField {
                                        id: nameOverrideField
                                        text: widgetNameOverride
                                        Layout.fillWidth: true
                                        selectByMouse: true
                                        placeholderText: ioName ? ioName : "Use IO name"
                                        onTextEdited: {
                                            var newVal = text.trim()
                                            if (newVal !== widgetNameOverride) {
                                                widgetNameOverride = newVal
                                                nameOverrideChanged(newVal)
                                            }
                                        }
                                        Keys.onReturnPressed: function(event) { event.accepted = true; focus = false }
                                        Keys.onEnterPressed: function(event) { event.accepted = true; focus = false }
                                    }

                                    Button {
                                        text: "\u2715"
                                        implicitWidth: 32
                                        implicitHeight: 32
                                        visible: widgetNameOverride !== ""
                                        ToolTip.text: "Clear name override (use IO name)"
                                        ToolTip.visible: hovered
                                        onClicked: {
                                            widgetNameOverride = ""
                                            nameOverrideField.text = ""
                                            nameOverrideChanged("")
                                            console.log("Name override cleared")
                                        }
                                    }
                                }
                            }
                        }

                        // ================================================
                        // Clock Panel — visible only when category is Clock
                        // ================================================
                        ColumnLayout {
                            id: clockPanelSection
                            visible: widgetCategory === "Clock"
                            Layout.fillWidth: true
                            spacing: 12

                            // Timezone
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                Label {
                                    text: "Timezone:"
                                }

                                ComboBox {
                                    id: clockTimezoneCombo
                                    model: availableTimezones
                                    Layout.fillWidth: true
                                    editable: true
                                    currentIndex: {
                                        var idx = availableTimezones.indexOf(clockTimezone)
                                        return idx >= 0 ? idx : availableTimezones.indexOf("UTC")
                                    }
                                    onActivated: function(index) {
                                        var tz = availableTimezones[index]
                                        if (tz !== clockTimezone) {
                                            clockTimezone = tz
                                            clockPropertyChanged("clock_timezone", tz)
                                        }
                                    }
                                    onAccepted: {
                                        // When user types and presses Enter in editable mode
                                        var tz = editText
                                        if (availableTimezones.indexOf(tz) >= 0 && tz !== clockTimezone) {
                                            clockTimezone = tz
                                            clockPropertyChanged("clock_timezone", tz)
                                        }
                                    }
                                }
                            }

                            // Time Format
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                Label {
                                    text: "Time Format:"
                                }

                                ComboBox {
                                    id: clockFormatCombo
                                    model: ["24h", "12h"]
                                    Layout.fillWidth: true
                                    currentIndex: clockFormat === "12" ? 1 : 0
                                    onActivated: function(index) {
                                        var fmt = index === 0 ? "24" : "12"
                                        if (fmt !== clockFormat) {
                                            clockFormat = fmt
                                            clockPropertyChanged("clock_format", fmt)
                                        }
                                    }
                                }
                            }

                            // Show Date
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                CheckBox {
                                    id: clockShowDateCheck
                                    text: "Show Date"
                                    checked: clockShowDate
                                    onToggled: {
                                        if (checked !== clockShowDate) {
                                            clockShowDate = checked
                                            clockPropertyChanged("clock_show_date", checked ? "true" : "false")
                                        }
                                    }
                                }
                            }

                            // Show Seconds
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4

                                CheckBox {
                                    id: clockSecondsCheck
                                    text: "Show Seconds"
                                    checked: clockSeconds
                                    onToggled: {
                                        if (checked !== clockSeconds) {
                                            clockSeconds = checked
                                            clockPropertyChanged("clock_seconds", checked ? "true" : "false")
                                        }
                                    }
                                }
                            }

                            // Date Format (visible only if Show Date is checked)
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                visible: clockShowDate

                                Label {
                                    text: "Date Format:"
                                }

                                ComboBox {
                                    id: clockDateFormatCombo
                                    model: availableDateFormats
                                    Layout.fillWidth: true
                                    currentIndex: {
                                        var idx = availableDateFormats.indexOf(clockDateFormat)
                                        return idx >= 0 ? idx : 0
                                    }
                                    onActivated: function(index) {
                                        var fmt = availableDateFormats[index]
                                        if (fmt !== clockDateFormat) {
                                            clockDateFormat = fmt
                                            clockPropertyChanged("clock_date_format", fmt)
                                        }
                                    }
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
                            deleteItemConfirmDialog.open()
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
            console.log("PropertiesPanel: Item selected -", itemData.itemType, "category:", itemData.widgetCategory || "IO", "at", itemData.startCol, itemData.startRow)

            // Set the category
            widgetCategory = itemData.widgetCategory || "IO"

            if (widgetCategory === "IO") {
                // Validate the IO and update display
                var currentIoId = itemData.ioId || ""
                widgetNameOverride = itemData.nameOverride || ""
                nameOverrideField.text = widgetNameOverride
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
                // Reset clock properties to defaults when viewing IO widget
                clockTimezone = "UTC"
                clockFormat = "24"
                clockShowDate = true
                clockDateFormat = "DD/MM/YYYY"
            } else if (widgetCategory === "Clock") {
                // Set clock properties from itemData
                clockTimezone = itemData.clockTimezone || "UTC"
                clockFormat = itemData.clockFormat || "24"
                clockShowDate = itemData.clockShowDate !== undefined ? itemData.clockShowDate : true
                clockDateFormat = itemData.clockDateFormat || "DD/MM/YYYY"
                clockSeconds = itemData.clockSeconds !== undefined ? itemData.clockSeconds : false
                // Clear IO info
                ioId = ""
                ioExists = false
                ioName = ""
                roomName = ""
                widgetType = "Clock"
                widgetNameOverride = ""
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
        widgetNameOverride = ""
        widgetCategory = "IO"
        clockTimezone = "UTC"
        clockFormat = "24"
        clockShowDate = true
        clockDateFormat = "DD/MM/YYYY"
        clockSeconds = false
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

    // Delete item confirmation dialog
    Dialog {
        id: deleteItemConfirmDialog
        title: "Delete Widget"
        modal: true
        anchors.centerIn: Overlay.overlay
        standardButtons: Dialog.Yes | Dialog.No

        Label {
            text: "Are you sure you want to delete this widget?"
            wrapMode: Text.WordWrap
        }

        onAccepted: {
            deleteItemRequested()
        }
    }
}
