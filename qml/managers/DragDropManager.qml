import QtQuick
import "../utils/GridUtils.js" as GridUtils

Item {
    id: dragDropManager

    // Configurable properties
    property var draggedItem: null
    property var gridRepeater: null
    property var multiItemContainer: null
    property var flickable: null
    property int gridColumns: 4  // Width (number of columns)
    property int gridRows: 4     // Height (number of rows)
    property int cellSize: 80

    // Signals
    signal itemPlaced(var coords, var itemData)
    signal itemMoved(var fromCoords, var toCoords, var itemData)
    signal itemDeleted(var coords, var itemData)

    // Start drag from palette
    function startDragFromPalette(mouse, paletteItem, itemData) {
        console.log("DragDropManager startDragFromPalette - draggedItem:", draggedItem)
        if (!draggedItem) {
            console.log("ERROR: draggedItem is null!")
            return
        }

        draggedItem.updateFromItemData(itemData)

        // Position draggedItem under the mouse
        var globalPos = paletteItem.mapToItem(null, mouse.x, mouse.y)
        console.log("Global position:", globalPos.x, globalPos.y)
        draggedItem.updatePosition(globalPos.x, globalPos.y)
        draggedItem.show()
        console.log("DraggedItem shown, visible:", draggedItem.visible)
    }

    // Start drag from grid
    function startDragFromGrid(mouse, gridCell, itemData) {
        console.log("DragDropManager startDragFromGrid - flickable:", flickable)
        if (!draggedItem) return

        // Disable flickable during drag
        if (flickable) {
            console.log("Disabling flickable, was interactive:", flickable.interactive)
            flickable.isDragging = true
            console.log("Flickable isDragging set to true, interactive now:", flickable.interactive)
        }

        draggedItem.updateFromItemData(itemData)

        // Position draggedItem under the mouse
        var globalPos = gridCell.mapToItem(null, mouse.x, mouse.y)
        draggedItem.updatePosition(globalPos.x, globalPos.y)
        draggedItem.show()
    }

    // Update position during drag
    function updateDrag(mouse, sourceItem, itemData) {
        if (!draggedItem || !draggedItem.visible) return

        var globalPos = sourceItem.mapToItem(null, mouse.x, mouse.y)
        draggedItem.updatePosition(globalPos.x, globalPos.y)

        // Update highlight
        GridUtils.updateHighlight(globalPos.x, globalPos.y, gridRepeater, gridColumns, gridRows, draggedItem)
    }

    // End drag from palette
    function endDragFromPalette(mouse, paletteItem, itemData) {
        if (!draggedItem) return

        var globalPos = paletteItem.mapToItem(null, mouse.x, mouse.y)
        var targetCell = GridUtils.findCellAtPosition(globalPos.x, globalPos.y, gridRepeater)

        if (targetCell) {
            var coords = GridUtils.indexToCellCoords(targetCell.cellIndex, gridColumns)
            if (GridUtils.canPlaceItem(coords.row, coords.col, itemData.itemWidth,
                                     itemData.itemHeight, gridColumns, gridRows, gridRepeater)) {
                // Signal item placement
                itemPlaced(coords, itemData)
                console.log("Item", itemData.itemWidth + "×" + itemData.itemHeight, "placé à la cellule", targetCell.cellIndex)
            } else {
                console.log("Placement rejeté - item", itemData.itemWidth + "×" + itemData.itemHeight,
                          "ne peut pas être placé à", coords.row, coords.col)
            }
        }

        // Cleanup
        cleanup()
    }

    // End drag from grid
    function endDragFromGrid(mouse, sourceItem, itemData) {
        if (!draggedItem) return

        var globalPos = sourceItem.mapToItem(null, mouse.x, mouse.y)
        var targetCell = GridUtils.findCellAtPosition(globalPos.x, globalPos.y, gridRepeater)
        var moved = false

        if (targetCell) {
            var coords = GridUtils.indexToCellCoords(targetCell.cellIndex, gridColumns)
            var sourceCoords = GridUtils.indexToCellCoords(itemData.sourceCellIndex, gridColumns)

            // Check if moving to a new position
            if (coords.row !== sourceCoords.row || coords.col !== sourceCoords.col) {
                // Prepare source coordinates to ignore
                var ignoreSourceCoords = {
                    row: sourceCoords.row,
                    col: sourceCoords.col,
                    width: itemData.itemWidth,
                    height: itemData.itemHeight
                }

                if (GridUtils.canPlaceItem(coords.row, coords.col, itemData.itemWidth,
                                         itemData.itemHeight, gridColumns, gridRows, gridRepeater, ignoreSourceCoords)) {
                    // Signal item movement
                    itemMoved(sourceCoords, coords, itemData)
                    console.log("Item", itemData.itemWidth + "×" + itemData.itemHeight, "déplacé vers cellule", targetCell.cellIndex)
                    moved = true
                } else {
                    console.log("Move rejected - item cannot be placed")
                }
            }
        }

        if (!moved) {
            // Restore item to original position
            restoreOriginalItem(sourceItem, itemData)
        }

        // Cleanup
        cleanup()
    }

    // Handle right click (delete)
    function handleRightClick(itemData) {
        var coords = GridUtils.indexToCellCoords(itemData.sourceCellIndex, gridColumns)
        itemDeleted(coords, itemData)
    }

    // Restore original item on failure
    function restoreOriginalItem(sourceItem, itemData) {
        if (sourceItem && sourceItem.restoreItem) {
            sourceItem.restoreItem()
        } else if (sourceItem && sourceItem.restoreOpacity) {
            sourceItem.restoreOpacity()
        }
    }

    // Cleanup after drag
    function cleanup() {
        if (draggedItem) {
            draggedItem.hide()
        }
        if (gridRepeater) {
            GridUtils.clearAllHighlights(gridRepeater)
        }
        // Re-enable flickable after drag
        if (flickable) {
            console.log("Re-enabling flickable, setting isDragging to false")
            flickable.isDragging = false
            console.log("Flickable interactive now:", flickable.interactive)
        }
    }
}
