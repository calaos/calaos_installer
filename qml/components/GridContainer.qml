import QtQuick
import "../managers"

Rectangle {
    id: gridContainer

    // Support for non-square grids
    property int gridColumns: 4  // Width (number of columns)
    property int gridRows: 4     // Height (number of rows)
    property int gridSize: gridColumns  // Deprecated: for backward compatibility
    property int cellSize: 80

    // Computed cell count - triggers Repeater rebuild when dimensions change
    readonly property int cellCount: gridColumns * gridRows
    property color backgroundColor: "#171717"
    property color borderColor: "#888888"

    // Selection management
    property var selectedWidget: null
    property int selectedCellIndex: -1

    property alias gridManager: gridManager
    property alias dragDropManager: dragDropManager
    property alias gridRepeater: gridRepeater
    property alias multiItemContainer: multiItemContainer
    signal itemPlaced(var coords, var itemData)
    signal itemMoved(var fromCoords, var toCoords, var itemData)
    signal itemDeleted(var coords, var itemData)
    signal itemSelected(var widget, var itemData)
    signal itemDeselected()
    signal linkIORequested(var widget, var itemData)
    signal itemResized(var widget, var itemData)

    width: gridColumns * cellSize + 20
    height: gridRows * cellSize + 20
    color: backgroundColor
    border.color: borderColor
    border.width: 2
    radius: 2

    // Click on background to deselect
    MouseArea {
        anchors.fill: parent
        z: -1
        onClicked: {
            gridContainer.clearSelection()
        }
    }

    Grid {
        id: mainGrid
        anchors.centerIn: parent
        //anchors.verticalCenterOffset: 15
        rows: gridRows
        columns: gridColumns
        spacing: 2

        Repeater {
            id: gridRepeater
            model: gridContainer.cellCount

            Rectangle {
                id: gridCell

                property int cellIndex: index
                property int gridRowCount: gridRows
                property int gridColumnCount: gridColumns
                property alias hasItem: placedItem.visible
                property alias itemType: placedItem.itemType
                property alias itemColor: placedItem.itemColor
                property alias itemText: placedItem.itemText
                property int itemWidth: 1
                property int itemHeight: 1
                property bool isMainCell: true
                property bool isOccupied: false
                property var gridManager: null

                width: cellSize
                height: cellSize
                color: "#171717"
                border.color: Qt.lighter(color, 1.8)
                border.width: 1

                readonly property int cellRow: Math.floor(cellIndex / gridColumns)
                readonly property int cellColumn: cellIndex % gridColumns

                Rectangle {
                    id: placedItem
                    property string itemType: "RectangleSmall"
                    property color itemColor: "#ff0000"
                    property string itemText: "Item"

                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
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



                function clearCell() {
                    placedItem.visible = false
                    isOccupied = false
                    isMainCell = false
                    itemWidth = 1
                    itemHeight = 1
                    color = "#171717"
                }

                function resetColor() {
                    color = "#171717"
                }



                function markAsOccupied(width, height) {
                    placedItem.visible = false
                    isOccupied = true
                    isMainCell = false
                    itemWidth = width
                    itemHeight = height
                    resetColor()
                }

                function deleteItem() {
                    if (!isMainCell || !isOccupied) return
                    console.log("Deleting item", itemWidth + "×" + itemHeight, "from cell", cellIndex)
                    gridManager.freeCells(Math.floor(cellIndex / gridColumns), cellIndex % gridColumns, itemWidth, itemHeight)
                }

                function restoreItem() {
                    placedItem.opacity = 1.0
                    resetColor()
                }

                // Click on empty cell to deselect
                MouseArea {
                    anchors.fill: parent
                    enabled: !parent.isOccupied
                    z: -1
                    onClicked: {
                        gridContainer.clearSelection()
                    }
                }

            }
        }
    }

    Item {
        id: multiItemContainer
        anchors.fill: mainGrid
        z: 100

        // Handle signals from dynamically created MultiItem instances
        function connectMultiItemSignals(multiItem) {
            multiItem.dragStarted.connect(function(mouse, itemData) {
                dragDropManager.startDragFromGrid(mouse, multiItem, itemData)
            })

            multiItem.dragMoved.connect(function(mouse, itemData) {
                dragDropManager.updateDrag(mouse, multiItem, itemData)
            })

            multiItem.dragEnded.connect(function(mouse, itemData) {
                dragDropManager.endDragFromGrid(mouse, multiItem, itemData)
            })

            // Click selects the item (no deletion on right-click anymore)
            multiItem.itemClicked.connect(function(itemData) {
                gridContainer.selectItem(multiItem, itemData)
            })

            multiItem.itemRightClicked.connect(function(itemData) {
                // Right-click also selects (no deletion)
                gridContainer.selectItem(multiItem, itemData)
            })

            multiItem.itemLinkIORequested.connect(function(itemData) {
                gridContainer.selectItem(multiItem, itemData)
                gridContainer.linkIORequested(multiItem, itemData)
            })

            multiItem.itemDeleteRequested.connect(function(itemData) {
                // Delete the widget
                var coords = { row: itemData.startRow, col: itemData.startCol }
                gridManager.freeCells(coords.row, coords.col, itemData.itemWidth, itemData.itemHeight)
                gridContainer.itemDeleted(coords, itemData)
                gridContainer.clearSelection()
            })
        }
    }

    GridManager {
        id: gridManager
        gridRepeater: gridRepeater
        multiItemContainer: multiItemContainer
        multiItemComponent: multiItemComponent
        gridColumns: gridContainer.gridColumns
        gridRows: gridContainer.gridRows
        cellSize: gridContainer.cellSize

        onItemPlaced: function(coords, itemData) {
            gridContainer.itemPlaced(coords, itemData)
        }

        onItemMoved: function(fromCoords, toCoords, itemData) {
            gridContainer.itemMoved(fromCoords, toCoords, itemData)
        }

        onItemDeleted: function(coords, itemData) {
            gridContainer.itemDeleted(coords, itemData)
        }

        onItemResized: function(widget, itemData) {
            gridContainer.itemResized(widget, itemData)
        }
    }

    DragDropManager {
        id: dragDropManager
        gridRepeater: gridRepeater
        multiItemContainer: multiItemContainer
        gridColumns: gridContainer.gridColumns
        gridRows: gridContainer.gridRows
        cellSize: gridContainer.cellSize

        onItemPlaced: function(coords, itemData) {
            gridManager.occupyCells(coords.row, coords.col, itemData.itemWidth,
                                  itemData.itemHeight, itemData.itemType,
                                  itemData.itemColor, itemData.itemText, itemData.itemName, itemData.ioId || "", itemData.nameOverride || "", itemData.ioName || "")
            // Propagate to parent (PageEditor -> main.qml) to update the model
            gridContainer.itemPlaced(coords, itemData)
        }

        onItemMoved: function(fromCoords, toCoords, itemData) {
            gridManager.moveItem(fromCoords.row, fromCoords.col, toCoords.row, toCoords.col,
                               itemData.itemWidth, itemData.itemHeight, itemData.itemType,
                               itemData.itemColor, itemData.itemText, itemData.itemName, itemData.ioId || "", itemData.nameOverride || "", itemData.ioName || "")
            // Propagate to parent to update the model
            gridContainer.itemMoved(fromCoords, toCoords, itemData)
        }

        onItemDeleted: function(coords, itemData) {
            gridManager.freeCells(coords.row, coords.col, itemData.itemWidth, itemData.itemHeight)
            // Propagate to parent to update the model
            gridContainer.itemDeleted(coords, itemData)
        }
    }

    Component {
        id: multiItemComponent

        RectWidget {
            id: multiItem

            property int startRow: 0
            property int startCol: 0
            property var gridManager: null
            property bool dragActive: false

            signal dragStarted(var mouse, var itemData)
            signal dragMoved(var mouse, var itemData)
            signal dragEnded(var mouse, var itemData)
            signal itemClicked(var itemData)
            signal itemRightClicked(var itemData)
            signal itemLinkIORequested(var itemData)
            signal itemDeleteRequested(var itemData)

            isPaletteItem: false
            isResizable: true
            gridColumns: gridContainer.gridColumns
            gridRows: gridContainer.gridRows

            // Handle resize request
            onResizeRequested: function(newWidth, newHeight) {
                console.log("Resize requested:", itemWidth + "×" + itemHeight, "->", newWidth + "×" + newHeight)
                if (gridManager) {
                    gridManager.tryResizeItem(startRow, startCol, itemWidth, itemHeight, newWidth, newHeight)
                }
            }

            // Handle resize validation (real-time preview feedback)
            onValidateResize: function(newWidth, newHeight) {
                console.log("onValidateResize: validating", newWidth + "×" + newHeight, "for item at", startRow, startCol, "current size:", itemWidth + "×" + itemHeight)
                if (gridManager) {
                    var isValid = gridManager.canResizeItem(startRow, startCol, itemWidth, itemHeight, newWidth, newHeight)
                    console.log("onValidateResize: result =", isValid)
                    multiItem.isResizeValid = isValid
                }
            }

            // Connect RectWidget's click signals to multiItem signals
            onClicked: function(itemData) {
                var data = multiItem.createItemData()
                multiItem.itemClicked(data)
            }

            onRightClicked: function(itemData) {
                var data = multiItem.createItemData()
                multiItem.itemRightClicked(data)
            }

            onLinkIORequested: function(itemData) {
                var data = multiItem.createItemData()
                multiItem.itemLinkIORequested(data)
            }

            onDeleteRequested: function(itemData) {
                var data = multiItem.createItemData()
                multiItem.itemDeleteRequested(data)
            }

            Connections {
                target: multiItem.mouseArea

                function onDragPressed(mouse) {
                    if (!multiItem.isResizing) {
                        console.log("Drag started on multi-item:", multiItem.itemName)
                        multiItem.dragActive = true
                        multiItem.opacity = 0.3
                        var itemData = multiItem.createItemData()
                        multiItem.dragStarted(mouse, itemData)
                    }
                }

                function onDragMoved(mouse) {
                    if (multiItem.dragActive && !multiItem.isResizing) {
                        var itemData = multiItem.createItemData()
                        multiItem.dragMoved(mouse, itemData)
                    }
                }

                function onDragReleased(mouse) {
                    if (multiItem.dragActive) {
                        console.log("Released multi-item")
                        multiItem.dragActive = false
                        var itemData = multiItem.createItemData()
                        multiItem.dragEnded(mouse, itemData)
                    }
                }
            }

            function createItemData() {
                return {
                    itemType: itemType,
                    itemName: itemName,
                    itemColor: itemColor,
                    itemText: itemText,
                    itemWidth: itemWidth,
                    itemHeight: itemHeight,
                    ioId: ioId,
                    nameOverride: nameOverride,
                    ioName: ioName,
                    startRow: startRow,
                    startCol: startCol,
                    sourceType: "grid",
                    sourceCellIndex: startRow * gridContainer.gridColumns + startCol,
                }
            }

            function restoreOpacity() {
                multiItem.opacity = 1.0
            }

            Component.onCompleted: {
                multiItemContainer.connectMultiItemSignals(this)
            }
        }
    }

    Component.onCompleted: {
        // Use Qt.callLater to ensure all property bindings have propagated
        // This fixes the bug where Repeater creates cells with default values (4x4)
        // before the actual grid dimensions are set from the parent
        Qt.callLater(function() {
            gridManager.initializeCells()
        })
    }

    // Handle grid dimension changes - preserve valid widgets
    onGridColumnsChanged: {
        Qt.callLater(function() {
            gridManager.validateItemsOnGridResize(gridColumns, gridRows)
        })
    }

    onGridRowsChanged: {
        Qt.callLater(function() {
            gridManager.validateItemsOnGridResize(gridColumns, gridRows)
        })
    }

    function placePredefinedItem(row, col, itemType, itemColor, itemText, itemWidth, itemHeight, ioId, nameOverride, ioName) {
        console.log("placePredefinedItem:", row, col, itemType, itemWidth + "×" + itemHeight, "ioId:", ioId || "none", "nameOverride:", nameOverride || "none", "ioName:", ioName || "none")
        if (gridManager.canPlaceItem(row, col, itemWidth, itemHeight)) {
            gridManager.occupyCells(row, col, itemWidth, itemHeight, itemType, itemColor, itemText, itemText, ioId || "", nameOverride || "", ioName || "")
            return true
        }
        console.log("placePredefinedItem: Cannot place item at", row, col)
        return false
    }

    function clearGrid() {
        clearSelection()
        for (var i = 0; i < gridRepeater.count; i++) {
            var cell = gridRepeater.itemAt(i)
            if (cell) {
                cell.clearCell()
            }
        }

        // Clear all MultiItem instances
        for (var j = multiItemContainer.children.length - 1; j >= 0; j--) {
            var child = multiItemContainer.children[j]
            if (child) {
                child.destroy()
            }
        }
    }

    // Selection management functions
    function selectItem(widget, itemData) {
        // Deselect previous item
        if (selectedWidget && selectedWidget !== widget) {
            selectedWidget.isSelected = false
        }

        // Select new item
        selectedWidget = widget
        selectedCellIndex = itemData.sourceCellIndex || (itemData.startRow * gridColumns + itemData.startCol)
        widget.isSelected = true

        console.log("Item selected:", itemData.itemName, "at cell", selectedCellIndex)
        itemSelected(widget, itemData)
    }

    // Select a widget by reference (used by highlightInvalidWidget)
    function selectWidget(widget) {
        if (widget && widget.createItemData) {
            var itemData = widget.createItemData()
            selectItem(widget, itemData)
        }
    }

    function clearSelection() {
        if (selectedWidget) {
            selectedWidget.isSelected = false
            selectedWidget = null
            selectedCellIndex = -1
            console.log("Selection cleared")
            itemDeselected()
        }
    }

    function deleteSelectedItem() {
        if (selectedWidget) {
            var itemData = selectedWidget.createItemData()
            var coords = {
                row: selectedWidget.startRow,
                col: selectedWidget.startCol
            }

            console.log("Deleting selected item:", itemData.itemName)

            // Free the cells and destroy the widget
            gridManager.freeCells(coords.row, coords.col, itemData.itemWidth, itemData.itemHeight)

            // Emit deletion signal
            itemDeleted(coords, itemData)

            // Clear selection
            clearSelection()
        }
    }
}
