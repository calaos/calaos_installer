import QtQuick
import "../utils/GridUtils.js" as GridUtils

Item {
    id: gridManager

    property var gridRepeater: null
    property var multiItemContainer: null
    property var multiItemComponent: null
    property int gridSize: 4
    property int cellSize: 80

    signal itemPlaced(var coords, var itemData)
    signal itemMoved(var fromCoords, var toCoords, var itemData)
    signal itemDeleted(var coords, var itemData)

    function occupyCells(startRow, startCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName) {
        createMultiItem(startRow, startCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName)

    // Mark all cells as occupied
        for (var row = startRow; row < startRow + itemHeight; row++) {
            for (var col = startCol; col < startCol + itemWidth; col++) {
                var index = GridUtils.cellCoordsToIndex(row, col, gridSize)
                var cell = gridRepeater.itemAt(index)
                if (cell) {
                    cell.markAsOccupied(itemWidth, itemHeight)
                }
            }
        }
    }

    function createMultiItem(startRow, startCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName) {
        if (!multiItemComponent || !multiItemContainer) {
            console.error("MultiItem component or container not available")
            return
        }

        var item = multiItemComponent.createObject(multiItemContainer)

        if (item) {
            item.itemType = itemType
            item.itemName = itemName || itemType
            item.itemColor = itemColor
            item.itemText = itemText
            item.itemWidth = itemWidth
            item.itemHeight = itemHeight
            item.startRow = startRow
            item.startCol = startCol
            item.gridManager = gridManager
            item.cellSize = cellSize
            console.log("GridManager createMultiItem - itemType:", itemType)

            // Calculate position and size
            var firstCellIndex = GridUtils.cellCoordsToIndex(startRow, startCol, gridSize)
            var firstCell = gridRepeater.itemAt(firstCellIndex)
            if (firstCell) {
                var cellPos = firstCell.mapToItem(multiItemContainer, 0, 0)
                item.x = cellPos.x
                item.y = cellPos.y
                item.width = itemWidth * cellSize + (itemWidth - 1) * 2
                item.height = itemHeight * cellSize + (itemHeight - 1) * 2
            }

            console.log("Multi-item created:", itemText, "position:", item.x, item.y, "size:", item.width, item.height, "visible:", item.visible)
        }
    }

    function freeCells(startRow, startCol, itemWidth, itemHeight) {
    // Remove MultiItem from container
        console.log("freeCells: Looking for MultiItem to delete at", startRow, startCol)
        for (var i = 0; i < multiItemContainer.children.length; i++) {
            var child = multiItemContainer.children[i]
            if (child && child.startRow === startRow && child.startCol === startCol) {
                console.log("freeCells: MultiItem found and deleted at", startRow, startCol)
                child.destroy()
                break
            }
        }

    // Clear all cells
        for (var row = startRow; row < startRow + itemHeight; row++) {
            for (var col = startCol; col < startCol + itemWidth; col++) {
                var index = GridUtils.cellCoordsToIndex(row, col, gridSize)
                var cell = gridRepeater.itemAt(index)
                if (cell) {
                    cell.clearCell()
                }
            }
        }
    }

    function moveItem(fromRow, fromCol, toRow, toCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName) {
        freeCells(fromRow, fromCol, itemWidth, itemHeight)
        occupyCells(toRow, toCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName)
    }

    function canPlaceItem(startRow, startCol, itemWidth, itemHeight) {
        return GridUtils.canPlaceItem(startRow, startCol, itemWidth, itemHeight, gridSize, gridRepeater)
    }

    function initializeCells() {
        for (var i = 0; i < gridRepeater.count; i++) {
            var cell = gridRepeater.itemAt(i)
            if (cell) {
                cell.gridManager = gridManager
            }
        }
    }
}