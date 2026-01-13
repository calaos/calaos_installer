import QtQuick
import "../utils/GridUtils.js" as GridUtils

Rectangle {
    id: gridCell

    property int cellIndex: 0
    property int gridRows: 4
    property int gridColumns: 4
    property alias hasItem: placedItem.visible
    property alias itemType: placedItem.itemType
    property alias itemColor: placedItem.itemColor
    property alias itemText: placedItem.itemText

    // Properties for multi-cell items
    property int itemWidth: 1
    property int itemHeight: 1
    property bool isMainCell: true  // true if this is the main cell of the item
    property bool isOccupied: false // true if occupied by a multi-cell item

    // Signals
    signal itemPressed(var mouse, var cellIndex)
    signal itemDragged(var mouse, var cellIndex)
    signal itemReleased(var mouse, var cellIndex)
    signal itemRightClicked(var cellIndex)

    // Exposed properties for external logic
    property var gridManager: null

    color: hasItem || isOccupied ? "#f8f8f8" : "#ffffff"
    border.color: "#dddddd"
    border.width: 1

    // Cell coordinate calculation
    readonly property int cellRow: Math.floor(cellIndex / gridColumns)
    readonly property int cellColumn: cellIndex % gridColumns

    // Item placed in cell
    Rectangle {
        id: placedItem

        property string itemType: "RectangleSmall"
        property color itemColor: "#ff0000"
        property string itemText: "Item"

        anchors.centerIn: parent
        width: parent.width - 8
        height: parent.height - 8
        color: itemColor
        border.color: "#333333"
        border.width: 1
        radius: 4
        visible: false

        Text {
            anchors.centerIn: parent
            text: itemText
            color: "white"
            font.bold: true
            font.pixelSize: 10
        }
    }

    // MouseArea for drag from grid
    MouseArea {
        id: gridCellMouseArea
        anchors.fill: parent
    enabled: placedItem.visible  // Only active if there is an item
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        property bool dragActive: false

        onPressed: function(mouse) {
            if (!placedItem.visible || !gridCell.isMainCell) return

            // Handle right click - delete
            if (mouse.button === Qt.RightButton) {
                console.log("Clic droit - suppression de l'item:", placedItem.itemText, "cellule", gridCell.cellIndex)
                itemRightClicked(gridCell.cellIndex)
                return
            }

            // Handle left click - drag
            if (mouse.button === Qt.LeftButton) {
                console.log("Pressed grid item:", placedItem.itemText, "in cell", gridCell.cellIndex)
                dragActive = true

                // Temporarily hide item in cell
                placedItem.visible = false
                gridCell.color = "#ffffff"

                itemPressed(mouse, gridCell.cellIndex)
            }
        }

        onPositionChanged: function(mouse) {
            if (dragActive && pressed && pressedButtons & Qt.LeftButton) {
                itemDragged(mouse, gridCell.cellIndex)
            }
        }

        onReleased: function(mouse) {
            if (mouse.button !== Qt.LeftButton) return

            console.log("Released grid item")
            dragActive = false
            itemReleased(mouse, gridCell.cellIndex)
        }
    }

    // Public functions
    function clearCell() {
        placedItem.visible = false
    // Restore default anchoring
        placedItem.anchors.left = undefined
        placedItem.anchors.top = undefined
        placedItem.anchors.leftMargin = 0
        placedItem.anchors.topMargin = 0
        placedItem.anchors.centerIn = gridCell
        placedItem.anchors.margins = 0
        placedItem.width = gridCell.width - 8
        placedItem.height = gridCell.height - 8
        isOccupied = false
        isMainCell = false
        itemWidth = 1
        itemHeight = 1
        color = "#ffffff"
    }

    function resetColor() {
        color = hasItem || isOccupied ? "#f8f8f8" : "#ffffff"
    }

    function placeItem(type, itemColor, text) {
        placedItem.itemType = type
        placedItem.itemColor = itemColor
        placedItem.itemText = text
        placedItem.visible = true
    // Restore normal positioning for 1x1 items
        placedItem.anchors.left = undefined
        placedItem.anchors.top = undefined
        placedItem.anchors.leftMargin = 0
        placedItem.anchors.topMargin = 0
        placedItem.anchors.centerIn = gridCell
        placedItem.anchors.margins = 0
        placedItem.width = gridCell.width - 8
        placedItem.height = gridCell.height - 8
        isOccupied = true
        isMainCell = true
        itemWidth = 1
        itemHeight = 1
        resetColor()
    }

    function markAsOccupied(width, height) {
    // Mark cell as occupied by multi-cell item (without display)
        placedItem.visible = false
        isOccupied = true
    isMainCell = false  // Not a main cell
        itemWidth = width
        itemHeight = height
        resetColor()
    }

    function deleteItem() {
        if (!isMainCell || !isOccupied) return

        console.log("Suppression de l'item", itemWidth + "×" + itemHeight, "depuis la cellule", cellIndex)

    // Notify grid manager
        if (gridManager) {
            var coords = GridUtils.indexToCellCoords(cellIndex, gridColumns)
            gridManager.freeCells(coords.row, coords.col, itemWidth, itemHeight)
        }
    }

    // Restore item if drag fails
    function restoreItem() {
        placedItem.visible = true
        resetColor()
    }
}