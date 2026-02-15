import QtQuick
import QtQuick.Controls
import "../utils/WidgetColors.js" as WidgetColors

Rectangle {
    id: rectWidget

    // Core properties
    property string itemName: "RectWidget"
    property string itemType: "RectWidget"
    property int itemWidth: 1    // Width in cells
    property int itemHeight: 1   // Height in cells
    property string itemText: itemWidth + "×" + itemHeight
    property string ioId: ""     // IO identifier from model
    property string nameOverride: ""  // Optional display name override
    property string ioName: ""    // IO display name (resolved from model)

    // Auto-calculated color based on size
    property color itemColor: WidgetColors.getColorForSize(itemWidth, itemHeight)
    property color textColor: WidgetColors.getTextColorForBackground(itemColor)
    property color borderColor: WidgetColors.getBorderColor(itemColor)

    // Visual properties
    property int borderWidth: 2
    property int itemRadius: 6

    // Grid properties
    property int cellSize: 80
    property bool isPaletteItem: false
    property int gridColumns: 4  // Max columns in grid (for resize limits)
    property int gridRows: 4     // Max rows in grid (for resize limits)
    property int startRow: 0     // Position in grid
    property int startCol: 0     // Position in grid

    // Selection state
    property bool isSelected: false
    property color selectionColor: "#4A90D9"

    // Error state - widget has no IO linked
    property bool hasError: !isPaletteItem && ioId === ""

    // Resize properties
    property bool isResizable: !isPaletteItem
    property bool isHovered: false
    property bool isResizing: false
    property int resizeHandleSize: 12

    // Resize preview properties
    property int previewWidth: itemWidth
    property int previewHeight: itemHeight
    property bool isResizeValid: true  // Whether current resize preview is valid (no collisions)

    // Signal to request resize validation from parent
    signal validateResize(int newWidth, int newHeight)

    // Long-press drag properties
    property bool isLongPressing: false
    property real longPressProgress: 0
    readonly property int longPressDuration: 350  // ms

    // Signals
    signal resizeRequested(int newWidth, int newHeight)
    signal clicked(var itemData)       // Single click - selection
    signal rightClicked(var itemData)  // Right click - context menu
    signal linkIORequested(var itemData)  // Link IO from context menu
    signal deleteRequested(var itemData)  // Delete from context menu

    // Expose MouseArea for external connections
    property alias mouseArea: mainMouseArea

    // Size calculation
    width: isPaletteItem ? Math.min(120, 30 + itemWidth * 20) : (cellSize + 2) * itemWidth - 2
    height: isPaletteItem ? Math.min(100, 30 + itemHeight * 20) : (cellSize + 2) * itemHeight - 2

    // Appearance - selection takes priority over hover
    color: itemColor
    border.color: isSelected ? selectionColor : (isHovered ? Qt.lighter(borderColor, 1.3) : borderColor)
    border.width: isSelected ? 3 : borderWidth
    radius: itemRadius

    // Selection halo effect
    Rectangle {
        id: selectionHalo
        visible: isSelected && !isPaletteItem
        anchors.centerIn: parent
        width: parent.width + 8
        height: parent.height + 8
        color: "transparent"
        border.color: selectionColor
        border.width: 2
        radius: itemRadius + 4
        opacity: 0.6
        z: -1
    }

    // Error indicator - red border for widgets without IO
    Rectangle {
        id: errorIndicator
        visible: hasError
        anchors.centerIn: parent
        width: parent.width + 4
        height: parent.height + 4
        color: "transparent"
        border.color: "#ff4444"
        border.width: 2
        radius: itemRadius + 2
        opacity: errorPulseAnimation.running ? 0.4 + 0.6 * errorPulse : 0.8
        z: -1

        property real errorPulse: 0

        // Subtle pulse animation for error state
        SequentialAnimation {
            id: errorPulseAnimation
            running: errorIndicator.visible
            loops: Animation.Infinite
            NumberAnimation {
                target: errorIndicator
                property: "errorPulse"
                from: 0
                to: 1
                duration: 800
                easing.type: Easing.InOutSine
            }
            NumberAnimation {
                target: errorIndicator
                property: "errorPulse"
                from: 1
                to: 0
                duration: 800
                easing.type: Easing.InOutSine
            }
        }
    }

    // Warning icon for error state
    Text {
        visible: hasError
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 4
        text: "⚠"
        font.pixelSize: 14
        color: "#ff4444"
        z: 10
    }

    // Stable hover detection using HoverHandler (avoids flickering)
    HoverHandler {
        id: widgetHoverHandler
        enabled: !isPaletteItem
    }

    // Timer to delay hiding hover state (prevents flickering during transitions)
    Timer {
        id: hoverDelayTimer
        interval: 100
        onTriggered: {
            if (!widgetHoverHandler.hovered) {
                rectWidget.isHovered = false
            }
        }
    }

    // Stable hover state management
    Connections {
        target: widgetHoverHandler
        function onHoveredChanged() {
            if (widgetHoverHandler.hovered) {
                hoverDelayTimer.stop()
                rectWidget.isHovered = true
            } else {
                // Delay before hiding to allow transition to resize handles
                hoverDelayTimer.restart()
            }
        }
    }

    // Update color when size changes
    onItemWidthChanged: updateColor()
    onItemHeightChanged: updateColor()

    function updateColor() {
        itemColor = WidgetColors.getColorForSize(itemWidth, itemHeight)
        textColor = WidgetColors.getTextColorForBackground(itemColor)
        borderColor = WidgetColors.getBorderColor(itemColor)
    }

    // Content area
    Item {
        anchors.fill: parent
        anchors.margins: borderWidth + 2

        // Size label
        Text {
            anchors.centerIn: parent
            text: rectWidget.itemText
            color: rectWidget.textColor
            font.bold: true
            font.pixelSize: isPaletteItem ? 11 : 14
        }

        // Item name (bottom)
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 2
            text: rectWidget.nameOverride !== "" ? rectWidget.nameOverride : (rectWidget.ioName !== "" ? rectWidget.ioName : "")
            color: rectWidget.textColor
            font.pixelSize: 9
            font.bold: rectWidget.nameOverride !== ""
            opacity: 0.8
            visible: !isPaletteItem && text !== ""
        }
    }

    // Main MouseArea for selection and drag
    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        // No dynamic margins - resize handles use higher z-index to intercept events
        hoverEnabled: false  // Hover is handled by HoverHandler
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        z: 1

        property bool dragActive: false
        property point pressPos: Qt.point(0, 0)

        // Signals for drag operations
        signal dragPressed(var mouse)
        signal dragMoved(var mouse)
        signal dragReleased(var mouse)

        onPressed: function(mouse) {
            if (rectWidget.isResizing) return

            pressPos = Qt.point(mouse.x, mouse.y)

            if (mouse.button === Qt.RightButton) {
                // Right click - select and show context menu
                rectWidget.rightClicked(rectWidget.getItemData())
                if (!isPaletteItem) {
                    contextMenu.popup()
                }
            } else if (mouse.button === Qt.LeftButton) {
                // Start long-press timer for drag
                rectWidget.isLongPressing = true
                rectWidget.longPressProgress = 0
                longPressTimer.start()
                longPressAnimation.start()
            }
        }

        onPositionChanged: function(mouse) {
            if (dragActive && pressed && !rectWidget.isResizing) {
                dragMoved(mouse)
            }
        }

        onReleased: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                longPressTimer.stop()
                longPressAnimation.stop()

                if (!dragActive && rectWidget.isLongPressing) {
                    // Short click - select the item
                    rectWidget.clicked(rectWidget.getItemData())
                }

                rectWidget.isLongPressing = false
                rectWidget.longPressProgress = 0
            }

            if (dragActive) {
                dragActive = false
                dragReleased(mouse)
            }
        }

        onCanceled: {
            longPressTimer.stop()
            longPressAnimation.stop()
            rectWidget.isLongPressing = false
            rectWidget.longPressProgress = 0
            dragActive = false
        }

        // Long-press timer to start drag
        Timer {
            id: longPressTimer
            interval: rectWidget.longPressDuration
            repeat: false
            onTriggered: {
                if (mainMouseArea.pressed && !rectWidget.isResizing) {
                    console.log("RectWidget long-press triggered - starting drag")
                    rectWidget.isLongPressing = false
                    mainMouseArea.dragActive = true
                    mainMouseArea.dragPressed({ x: mainMouseArea.pressPos.x, y: mainMouseArea.pressPos.y })
                }
            }
        }

        // Animation for long-press progress feedback
        NumberAnimation {
            id: longPressAnimation
            target: rectWidget
            property: "longPressProgress"
            from: 0
            to: 1
            duration: rectWidget.longPressDuration
            easing.type: Easing.Linear
        }
    }

    // Visual feedback for long-press (progress indicator)
    Rectangle {
        id: longPressIndicator
        visible: rectWidget.isLongPressing && !isPaletteItem
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 4
        width: (parent.width - 8) * rectWidget.longPressProgress
        height: 3
        radius: 1.5
        color: selectionColor
        opacity: 0.8
    }

    // Resize preview overlay
    Rectangle {
        id: resizePreview
        visible: rectWidget.isResizing && !isPaletteItem
        x: 0
        y: 0
        width: (cellSize + 2) * previewWidth - 2
        height: (cellSize + 2) * previewHeight - 2
        color: isResizeValid ? Qt.rgba(itemColor.r, itemColor.g, itemColor.b, 0.3) : Qt.rgba(1, 0, 0, 0.3)
        border.color: isResizeValid ? selectionColor : "#ff4444"
        border.width: 2
        radius: itemRadius
        z: 10

        // Preview size label
        Rectangle {
            anchors.centerIn: parent
            width: previewSizeText.width + 16
            height: previewSizeText.height + 8
            color: Qt.rgba(0, 0, 0, 0.7)
            radius: 4

            Text {
                id: previewSizeText
                anchors.centerIn: parent
                text: previewWidth + "×" + previewHeight
                color: "white"
                font.bold: true
                font.pixelSize: 14
            }
        }
    }

    // Resize handle - Right edge
    Rectangle {
        id: resizeRight
        visible: isResizable && isHovered && !isPaletteItem
        width: resizeHandleSize
        height: parent.height - resizeHandleSize
        anchors.right: parent.right
        anchors.top: parent.top
        color: rightDrag.active ? Qt.rgba(1,1,1,0.5) : Qt.rgba(1,1,1,0.3)
        radius: 2
        z: 10  // Higher than mainMouseArea to intercept events

        Rectangle {
            anchors.centerIn: parent
            width: 3
            height: 20
            radius: 1
            color: rectWidget.textColor
            opacity: 0.6
        }

        HoverHandler {
            id: rightHover
            cursorShape: Qt.SizeHorCursor
        }

        DragHandler {
            id: rightDrag
            target: null
            grabPermissions: PointerHandler.CanTakeOverFromAnything
            onActiveChanged: {
                if (active) {
                    rectWidget.isResizing = true
                    rectWidget.previewWidth = rectWidget.itemWidth
                    rectWidget.previewHeight = rectWidget.itemHeight
                    rectWidget.isResizeValid = true
                } else {
                    rectWidget.isResizing = false
                    console.log("Right resize released: isResizeValid =", rectWidget.isResizeValid, "preview:", previewWidth + "×" + rectWidget.itemHeight)
                    // Apply the resize only if valid
                    if (rectWidget.isResizeValid && previewWidth !== rectWidget.itemWidth) {
                        resizeRequested(previewWidth, rectWidget.itemHeight)
                    } else if (!rectWidget.isResizeValid) {
                        console.log("Resize BLOCKED due to collision")
                    }
                    // Reset preview
                    rectWidget.previewWidth = rectWidget.itemWidth
                    rectWidget.previewHeight = rectWidget.itemHeight
                    rectWidget.isResizeValid = true
                }
            }
            onCentroidChanged: {
                if (active) {
                    var newWidth = Math.max(1, Math.min(gridColumns - startCol,
                        Math.round((rectWidget.width + centroid.position.x) / (cellSize + 2))))
                    rectWidget.previewWidth = newWidth
                    rectWidget.validateResize(newWidth, rectWidget.itemHeight)
                }
            }
        }
    }

    // Resize handle - Bottom edge
    Rectangle {
        id: resizeBottom
        visible: isResizable && isHovered && !isPaletteItem
        width: parent.width - resizeHandleSize
        height: resizeHandleSize
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: bottomDrag.active ? Qt.rgba(1,1,1,0.5) : Qt.rgba(1,1,1,0.3)
        radius: 2
        z: 10  // Higher than mainMouseArea to intercept events

        Rectangle {
            anchors.centerIn: parent
            width: 20
            height: 3
            radius: 1
            color: rectWidget.textColor
            opacity: 0.6
        }

        HoverHandler {
            id: bottomHover
            cursorShape: Qt.SizeVerCursor
        }

        DragHandler {
            id: bottomDrag
            target: null
            grabPermissions: PointerHandler.CanTakeOverFromAnything
            onActiveChanged: {
                if (active) {
                    rectWidget.isResizing = true
                    rectWidget.previewWidth = rectWidget.itemWidth
                    rectWidget.previewHeight = rectWidget.itemHeight
                    rectWidget.isResizeValid = true
                } else {
                    rectWidget.isResizing = false
                    console.log("Bottom resize released: isResizeValid =", rectWidget.isResizeValid, "preview:", rectWidget.itemWidth + "×" + previewHeight)
                    // Apply the resize only if valid
                    if (rectWidget.isResizeValid && previewHeight !== rectWidget.itemHeight) {
                        resizeRequested(rectWidget.itemWidth, previewHeight)
                    } else if (!rectWidget.isResizeValid) {
                        console.log("Resize BLOCKED due to collision")
                    }
                    // Reset preview
                    rectWidget.previewWidth = rectWidget.itemWidth
                    rectWidget.previewHeight = rectWidget.itemHeight
                    rectWidget.isResizeValid = true
                }
            }
            onCentroidChanged: {
                if (active) {
                    var newHeight = Math.max(1, Math.min(gridRows - startRow,
                        Math.round((rectWidget.height + centroid.position.y) / (cellSize + 2))))
                    rectWidget.previewHeight = newHeight
                    rectWidget.validateResize(rectWidget.itemWidth, newHeight)
                }
            }
        }
    }

    // Resize handle - Corner
    Rectangle {
        id: resizeCorner
        visible: isResizable && isHovered && !isPaletteItem
        width: resizeHandleSize
        height: resizeHandleSize
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: cornerDrag.active ? Qt.rgba(1,1,1,0.6) : Qt.rgba(1,1,1,0.4)
        radius: 2
        z: 11  // Highest z to be above other handles

        // Corner grip dots
        Grid {
            anchors.centerIn: parent
            columns: 2
            spacing: 2
            Repeater {
                model: 4
                Rectangle {
                    width: 3
                    height: 3
                    radius: 1
                    color: rectWidget.textColor
                    opacity: 0.6
                }
            }
        }

        HoverHandler {
            id: cornerHover
            cursorShape: Qt.SizeFDiagCursor
        }

        DragHandler {
            id: cornerDrag
            target: null
            grabPermissions: PointerHandler.CanTakeOverFromAnything
            onActiveChanged: {
                if (active) {
                    rectWidget.isResizing = true
                    rectWidget.previewWidth = rectWidget.itemWidth
                    rectWidget.previewHeight = rectWidget.itemHeight
                    rectWidget.isResizeValid = true
                } else {
                    rectWidget.isResizing = false
                    console.log("Corner resize released: isResizeValid =", rectWidget.isResizeValid, "preview:", previewWidth + "×" + previewHeight)
                    // Apply the resize only if valid
                    if (rectWidget.isResizeValid && (previewWidth !== rectWidget.itemWidth || previewHeight !== rectWidget.itemHeight)) {
                        resizeRequested(previewWidth, previewHeight)
                    } else if (!rectWidget.isResizeValid) {
                        console.log("Resize BLOCKED due to collision")
                    }
                    // Reset preview
                    rectWidget.previewWidth = rectWidget.itemWidth
                    rectWidget.previewHeight = rectWidget.itemHeight
                    rectWidget.isResizeValid = true
                }
            }
            onCentroidChanged: {
                if (active) {
                    var newWidth = Math.max(1, Math.min(gridColumns - startCol,
                        Math.round((rectWidget.width + centroid.position.x) / (cellSize + 2))))
                    var newHeight = Math.max(1, Math.min(gridRows - startRow,
                        Math.round((rectWidget.height + centroid.position.y) / (cellSize + 2))))
                    rectWidget.previewWidth = newWidth
                    rectWidget.previewHeight = newHeight
                    rectWidget.validateResize(newWidth, newHeight)
                }
            }
        }
    }

    // Get item data for drag operations and selection
    function getItemData() {
        return {
            itemName: itemName,
            itemType: itemType,
            itemColor: itemColor,
            itemText: itemText,
            itemWidth: itemWidth,
            itemHeight: itemHeight,
            ioId: ioId,
            nameOverride: nameOverride,
            ioName: ioName,
            startRow: startRow,
            startCol: startCol
        }
    }

    // Context menu for right-click
    Menu {
        id: contextMenu

        MenuItem {
            text: "Link IO..."
            icon.source: "qrc:/img/icon_io.png"
            onTriggered: {
                rectWidget.linkIORequested(rectWidget.getItemData())
            }
        }

        MenuSeparator {}

        MenuItem {
            text: "Delete"
            icon.source: "qrc:/img/icon_delete.png"
            onTriggered: {
                deleteConfirmDialog.open()
            }
        }
    }

    // Delete confirmation dialog
    Dialog {
        id: deleteConfirmDialog
        title: "Delete Widget"
        modal: true
        anchors.centerIn: Overlay.overlay
        standardButtons: Dialog.Yes | Dialog.No

        Label {
            text: "Are you sure you want to delete this widget?"
            wrapMode: Text.WordWrap
        }

        onAccepted: {
            rectWidget.deleteRequested(rectWidget.getItemData())
        }
    }
}
