import QtQuick
import "../utils/GridUtils.js" as GridUtils

Rectangle {
    id: multiItem

    property string itemType: "RectangleSmall"
    property color itemColor: "#ff0000"
    property string itemText: "Item"
    property int itemWidth: 1
    property int itemHeight: 1
    property int startRow: 0
    property int startCol: 0

    // Signals
    signal dragStarted(var mouse, var itemData)
    signal dragMoved(var mouse, var itemData)
    signal dragEnded(var mouse, var itemData)
    signal rightClicked(var itemData)

    // Properties for external logic
    property var gridManager: null
    property int cellSize: 80

    color: itemColor
    border.color: "#333333"
    border.width: 2
    radius: 4

    Text {
        anchors.centerIn: parent
        text: multiItem.itemText
        color: "white"
        font.bold: true
        font.pixelSize: 10
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        property bool dragActive: false

        onPressed: function(mouse) {
            // Handle right click - delete
            if (mouse.button === Qt.RightButton) {
                console.log("Clic droit - suppression multi-item:", multiItem.itemText)

                var itemData = {
                    itemType: multiItem.itemType,
                    itemColor: multiItem.itemColor,
                    itemText: multiItem.itemText,
                    itemWidth: multiItem.itemWidth,
                    itemHeight: multiItem.itemHeight,
                    startRow: multiItem.startRow,
                    startCol: multiItem.startCol,
                    sourceType: "grid",
                    sourceCellIndex: GridUtils.cellCoordsToIndex(multiItem.startRow, multiItem.startCol, 4) // TODO: gridSize from parent
                }

                rightClicked(itemData)
                return
            }

            // Handle left click - drag
            if (mouse.button === Qt.LeftButton) {
                console.log("Pressed multi-item:", multiItem.itemText)
                dragActive = true

                // Make item semi-transparent during drag
                multiItem.opacity = 0.3

                var itemData = {
                    itemType: multiItem.itemType,
                    itemColor: multiItem.itemColor,
                    itemText: multiItem.itemText,
                    itemWidth: multiItem.itemWidth,
                    itemHeight: multiItem.itemHeight,
                    startRow: multiItem.startRow,
                    startCol: multiItem.startCol,
                    sourceType: "grid",
                    sourceCellIndex: GridUtils.cellCoordsToIndex(multiItem.startRow, multiItem.startCol, 4) // TODO: gridSize from parent
                }

                dragStarted(mouse, itemData)
            }
        }

        onPositionChanged: function(mouse) {
            if (dragActive && pressed && pressedButtons & Qt.LeftButton) {
                var itemData = {
                    itemType: multiItem.itemType,
                    itemColor: multiItem.itemColor,
                    itemText: multiItem.itemText,
                    itemWidth: multiItem.itemWidth,
                    itemHeight: multiItem.itemHeight,
                    startRow: multiItem.startRow,
                    startCol: multiItem.startCol,
                    sourceType: "grid",
                    sourceCellIndex: GridUtils.cellCoordsToIndex(multiItem.startRow, multiItem.startCol, 4) // TODO: gridSize from parent
                }

                dragMoved(mouse, itemData)
            }
        }

        onReleased: function(mouse) {
            if (mouse.button !== Qt.LeftButton) return

            console.log("Released multi-item")
            dragActive = false

            var itemData = {
                itemType: multiItem.itemType,
                itemColor: multiItem.itemColor,
                itemText: multiItem.itemText,
                itemWidth: multiItem.itemWidth,
                itemHeight: multiItem.itemHeight,
                startRow: multiItem.startRow,
                startCol: multiItem.startCol,
                sourceType: "grid",
                sourceCellIndex: GridUtils.cellCoordsToIndex(multiItem.startRow, multiItem.startCol, 4) // TODO: gridSize from parent
            }

            dragEnded(mouse, itemData)
        }
    }

    // Restore opacity
    function restoreOpacity() {
        multiItem.opacity = 1.0
    }

    // Delete this item
    function deleteSelf() {
        if (gridManager) {
            gridManager.freeCells(startRow, startCol, itemWidth, itemHeight)
        }
    }
}