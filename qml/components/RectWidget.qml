import QtQuick
import "../utils/WidgetColors.js" as WidgetColors

Rectangle {
    id: rectWidget

    // Core properties
    property string itemName: "RectWidget"
    property string itemType: "RectWidget"
    property int itemWidth: 1    // Width in cells
    property int itemHeight: 1   // Height in cells
    property string itemText: itemWidth + "×" + itemHeight

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

    // Resize properties
    property bool isResizable: !isPaletteItem
    property bool isHovered: false
    property bool isResizing: false
    property int resizeHandleSize: 12

    // Signals
    signal resizeRequested(int newWidth, int newHeight)

    // Expose MouseArea for external connections
    property alias mouseArea: mainMouseArea

    // Size calculation
    width: isPaletteItem ? Math.min(120, 30 + itemWidth * 20) : (cellSize + 2) * itemWidth - 2
    height: isPaletteItem ? Math.min(100, 30 + itemHeight * 20) : (cellSize + 2) * itemHeight - 2

    // Appearance
    color: itemColor
    border.color: isHovered ? Qt.lighter(borderColor, 1.3) : borderColor
    border.width: borderWidth
    radius: itemRadius

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
            text: rectWidget.itemName !== "RectWidget" ? rectWidget.itemName : ""
            color: rectWidget.textColor
            font.pixelSize: 9
            opacity: 0.8
            visible: !isPaletteItem && text !== ""
        }
    }

    // Main MouseArea for drag
    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        anchors.rightMargin: isResizable && isHovered ? resizeHandleSize : 0
        anchors.bottomMargin: isResizable && isHovered ? resizeHandleSize : 0
        hoverEnabled: true
        z: 1

        property bool dragActive: false

        // Signals for drag operations
        signal dragPressed(var mouse)
        signal dragMoved(var mouse)
        signal dragReleased(var mouse)

        onEntered: rectWidget.isHovered = true
        onExited: if (!resizeRight.containsMouse && !resizeBottom.containsMouse && !resizeCorner.containsMouse) rectWidget.isHovered = false

        onPressed: function(mouse) {
            if (!rectWidget.isResizing) {
                console.log("RectWidget MouseArea pressed - size:", rectWidget.itemWidth + "×" + rectWidget.itemHeight)
                dragActive = true
                dragPressed(mouse)
            }
        }

        onPositionChanged: function(mouse) {
            if (dragActive && pressed && !rectWidget.isResizing) {
                dragMoved(mouse)
            }
        }

        onReleased: function(mouse) {
            dragActive = false
            dragReleased(mouse)
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
        color: containsMouse || rightDrag.active ? Qt.rgba(1,1,1,0.4) : Qt.rgba(1,1,1,0.2)
        radius: 2

        property bool containsMouse: rightMouseArea.containsMouse

        Rectangle {
            anchors.centerIn: parent
            width: 3
            height: 20
            radius: 1
            color: rectWidget.textColor
            opacity: 0.6
        }

        MouseArea {
            id: rightMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SizeHorCursor
            onEntered: rectWidget.isHovered = true
        }

        DragHandler {
            id: rightDrag
            target: null
            onActiveChanged: {
                if (active) {
                    rectWidget.isResizing = true
                } else {
                    rectWidget.isResizing = false
                    // Calculate new width based on drag
                    var newWidth = Math.max(1, Math.min(gridColumns - startCol,
                        Math.round((rectWidget.width + centroid.position.x) / (cellSize + 2))))
                    if (newWidth !== rectWidget.itemWidth) {
                        resizeRequested(newWidth, rectWidget.itemHeight)
                    }
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
        color: containsMouse || bottomDrag.active ? Qt.rgba(1,1,1,0.4) : Qt.rgba(1,1,1,0.2)
        radius: 2

        property bool containsMouse: bottomMouseArea.containsMouse

        Rectangle {
            anchors.centerIn: parent
            width: 20
            height: 3
            radius: 1
            color: rectWidget.textColor
            opacity: 0.6
        }

        MouseArea {
            id: bottomMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SizeVerCursor
            onEntered: rectWidget.isHovered = true
        }

        DragHandler {
            id: bottomDrag
            target: null
            onActiveChanged: {
                if (active) {
                    rectWidget.isResizing = true
                } else {
                    rectWidget.isResizing = false
                    var newHeight = Math.max(1, Math.min(gridRows - startRow,
                        Math.round((rectWidget.height + centroid.position.y) / (cellSize + 2))))
                    if (newHeight !== rectWidget.itemHeight) {
                        resizeRequested(rectWidget.itemWidth, newHeight)
                    }
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
        color: containsMouse || cornerDrag.active ? Qt.rgba(1,1,1,0.5) : Qt.rgba(1,1,1,0.3)
        radius: 2

        property bool containsMouse: cornerMouseArea.containsMouse

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

        MouseArea {
            id: cornerMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SizeFDiagCursor
            onEntered: rectWidget.isHovered = true
        }

        DragHandler {
            id: cornerDrag
            target: null
            onActiveChanged: {
                if (active) {
                    rectWidget.isResizing = true
                } else {
                    rectWidget.isResizing = false
                    var newWidth = Math.max(1, Math.min(gridColumns - startCol,
                        Math.round((rectWidget.width + centroid.position.x) / (cellSize + 2))))
                    var newHeight = Math.max(1, Math.min(gridRows - startRow,
                        Math.round((rectWidget.height + centroid.position.y) / (cellSize + 2))))
                    if (newWidth !== rectWidget.itemWidth || newHeight !== rectWidget.itemHeight) {
                        resizeRequested(newWidth, newHeight)
                    }
                }
            }
        }
    }

    // Get item data for drag operations
    function getItemData() {
        return {
            itemName: itemName,
            itemType: itemType,
            itemColor: itemColor,
            itemText: itemText,
            itemWidth: itemWidth,
            itemHeight: itemHeight
        }
    }
}
