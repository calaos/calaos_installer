import QtQuick
import "../utils/GridUtils.js" as GridUtils

Item {
    id: gridManager

    property var gridRepeater: null
    property var multiItemContainer: null
    property var multiItemComponent: null
    property int gridColumns: 4  // Width (number of columns)
    property int gridRows: 4     // Height (number of rows)
    property int cellSize: 80

    signal itemPlaced(var coords, var itemData)
    signal itemMoved(var fromCoords, var toCoords, var itemData)
    signal itemDeleted(var coords, var itemData)
    signal itemResized(var widget, var itemData)

    function occupyCells(startRow, startCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName, ioId) {
        createMultiItem(startRow, startCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName, ioId || "")

    // Mark all cells as occupied
        for (var row = startRow; row < startRow + itemHeight; row++) {
            for (var col = startCol; col < startCol + itemWidth; col++) {
                var index = GridUtils.cellCoordsToIndex(row, col, gridColumns)
                var cell = gridRepeater.itemAt(index)
                if (cell) {
                    cell.markAsOccupied(itemWidth, itemHeight)
                }
            }
        }
    }

    function createMultiItem(startRow, startCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName, ioId) {
        if (!multiItemComponent || !multiItemContainer) {
            console.error("MultiItem component or container not available")
            return null
        }

        var item = multiItemComponent.createObject(multiItemContainer)

        if (item) {
            item.itemType = itemType
            item.itemName = itemName || itemText
            item.itemWidth = itemWidth
            item.itemHeight = itemHeight
            item.startRow = startRow
            item.startCol = startCol
            item.gridManager = gridManager
            item.cellSize = cellSize
            item.gridColumns = gridColumns
            item.gridRows = gridRows
            item.ioId = ioId || ""
            console.log("GridManager createMultiItem - type:", itemType, "name:", item.itemName, "size:", itemWidth + "×" + itemHeight, "ioId:", item.ioId)

            // Calculate position and size
            var firstCellIndex = GridUtils.cellCoordsToIndex(startRow, startCol, gridColumns)
            var firstCell = gridRepeater.itemAt(firstCellIndex)
            if (firstCell) {
                var cellPos = firstCell.mapToItem(multiItemContainer, 0, 0)
                item.x = cellPos.x
                item.y = cellPos.y
                item.width = (cellSize + 2) * itemWidth - 2
                item.height = (cellSize + 2) * itemHeight - 2
            }

            console.log("Multi-item created:", item.itemName, "position:", item.x, item.y, "size:", item.width, item.height)
            return item
        }
        return null
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
                var index = GridUtils.cellCoordsToIndex(row, col, gridColumns)
                var cell = gridRepeater.itemAt(index)
                if (cell) {
                    cell.clearCell()
                }
            }
        }
    }

    function moveItem(fromRow, fromCol, toRow, toCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName, ioId) {
        freeCells(fromRow, fromCol, itemWidth, itemHeight)
        occupyCells(toRow, toCol, itemWidth, itemHeight, itemType, itemColor, itemText, itemName, ioId || "")
    }

    function canPlaceItem(startRow, startCol, itemWidth, itemHeight) {
        return GridUtils.canPlaceItem(startRow, startCol, itemWidth, itemHeight, gridColumns, gridRows, gridRepeater)
    }

    function initializeCells() {
        for (var i = 0; i < gridRepeater.count; i++) {
            var cell = gridRepeater.itemAt(i)
            if (cell) {
                cell.gridManager = gridManager
                cell.clearCell()  // Ensure cells start clean
            }
        }
    }

    // Validate and preserve widgets when grid is resized
    function validateItemsOnGridResize(newCols, newRows) {
        console.log("validateItemsOnGridResize: newCols:", newCols, "newRows:", newRows)

        // First, clear all cell states (cells were recreated by Repeater)
        for (var i = 0; i < gridRepeater.count; i++) {
            var cell = gridRepeater.itemAt(i)
            if (cell) {
                cell.gridManager = gridManager
                cell.clearCell()
            }
        }

        // Check each MultiItem and remove those outside bounds
        var itemsToRemove = []
        var itemsToKeep = []

        for (var j = 0; j < multiItemContainer.children.length; j++) {
            var item = multiItemContainer.children[j]
            if (!item || item.startRow === undefined) continue

            var endRow = item.startRow + item.itemHeight
            var endCol = item.startCol + item.itemWidth

            // Check if item is completely outside the new grid
            if (item.startRow >= newRows || item.startCol >= newCols) {
                console.log("Item at", item.startRow, item.startCol, "is outside grid, removing")
                itemsToRemove.push(item)
            }
            // Check if item extends beyond the new grid - needs resizing or removal
            else if (endRow > newRows || endCol > newCols) {
                // Try to resize the item to fit
                var newWidth = Math.min(item.itemWidth, newCols - item.startCol)
                var newHeight = Math.min(item.itemHeight, newRows - item.startRow)

                if (newWidth > 0 && newHeight > 0) {
                    console.log("Resizing item at", item.startRow, item.startCol, "from",
                                item.itemWidth + "×" + item.itemHeight, "to", newWidth + "×" + newHeight)
                    item.itemWidth = newWidth
                    item.itemHeight = newHeight
                    itemsToKeep.push(item)
                } else {
                    itemsToRemove.push(item)
                }
            } else {
                // Item is completely within the new grid
                itemsToKeep.push(item)
            }
        }

        // Remove invalid items
        for (var k = 0; k < itemsToRemove.length; k++) {
            itemsToRemove[k].destroy()
        }

        // Reposition and re-mark cells for valid items
        for (var m = 0; m < itemsToKeep.length; m++) {
            var keepItem = itemsToKeep[m]
            repositionMultiItem(keepItem)
            markCellsForItem(keepItem)
        }

        console.log("Grid resize complete: kept", itemsToKeep.length, "items, removed", itemsToRemove.length)
    }

    // Reposition a MultiItem based on its startRow/startCol
    function repositionMultiItem(item) {
        var firstCellIndex = GridUtils.cellCoordsToIndex(item.startRow, item.startCol, gridColumns)
        var firstCell = gridRepeater.itemAt(firstCellIndex)

        if (firstCell) {
            var cellPos = firstCell.mapToItem(multiItemContainer, 0, 0)
            item.x = cellPos.x
            item.y = cellPos.y
            item.width = item.itemWidth * cellSize + (item.itemWidth - 1) * 2
            item.height = item.itemHeight * cellSize + (item.itemHeight - 1) * 2
        }
    }

    // Mark cells as occupied for an item
    function markCellsForItem(item) {
        for (var row = item.startRow; row < item.startRow + item.itemHeight; row++) {
            for (var col = item.startCol; col < item.startCol + item.itemWidth; col++) {
                var index = GridUtils.cellCoordsToIndex(row, col, gridColumns)
                var cell = gridRepeater.itemAt(index)
                if (cell) {
                    cell.markAsOccupied(item.itemWidth, item.itemHeight)
                }
            }
        }
    }

    // Try to resize an item - check for collisions and boundaries
    function tryResizeItem(startRow, startCol, oldWidth, oldHeight, newWidth, newHeight) {
        console.log("tryResizeItem: from", oldWidth + "×" + oldHeight, "to", newWidth + "×" + newHeight, "at", startRow, startCol)

        // Find the MultiItem
        var targetItem = null
        for (var i = 0; i < multiItemContainer.children.length; i++) {
            var item = multiItemContainer.children[i]
            if (item && item.startRow === startRow && item.startCol === startCol) {
                targetItem = item
                break
            }
        }

        if (!targetItem) {
            console.log("tryResizeItem: Item not found")
            return false
        }

        // Check boundaries
        if (startRow + newHeight > gridRows || startCol + newWidth > gridColumns) {
            console.log("tryResizeItem: Would exceed grid boundaries")
            return false
        }

        // Check for collisions with other items (ignoring cells occupied by this item)
        for (var row = startRow; row < startRow + newHeight; row++) {
            for (var col = startCol; col < startCol + newWidth; col++) {
                // Skip cells that are already part of this item
                if (row < startRow + oldHeight && col < startCol + oldWidth) {
                    continue
                }

                var index = GridUtils.cellCoordsToIndex(row, col, gridColumns)
                var cell = gridRepeater.itemAt(index)

                if (cell && (cell.hasItem || cell.isOccupied)) {
                    console.log("tryResizeItem: Cell", row, col, "is occupied")
                    return false
                }
            }
        }

        // Clear old cells
        for (var r = startRow; r < startRow + oldHeight; r++) {
            for (var c = startCol; c < startCol + oldWidth; c++) {
                var idx = GridUtils.cellCoordsToIndex(r, c, gridColumns)
                var oldCell = gridRepeater.itemAt(idx)
                if (oldCell) {
                    oldCell.clearCell()
                }
            }
        }

        // Update item dimensions
        targetItem.itemWidth = newWidth
        targetItem.itemHeight = newHeight
        targetItem.width = newWidth * cellSize + (newWidth - 1) * 2
        targetItem.height = newHeight * cellSize + (newHeight - 1) * 2

        // Mark new cells
        markCellsForItem(targetItem)

        console.log("tryResizeItem: Success")

        // Emit signal with updated item data for PropertiesPanel
        var itemData = {
            itemType: targetItem.itemType,
            itemName: targetItem.itemName,
            itemColor: targetItem.itemColor,
            itemText: newWidth + "×" + newHeight,
            itemWidth: newWidth,
            itemHeight: newHeight,
            ioId: targetItem.ioId || "",
            startRow: startRow,
            startCol: startCol,
            sourceType: "grid",
            sourceCellIndex: startRow * gridColumns + startCol
        }
        itemResized(targetItem, itemData)

        return true
    }
}