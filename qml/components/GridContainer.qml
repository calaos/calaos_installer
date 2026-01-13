import QtQuick
import "../managers"

Rectangle {
    id: gridContainer

    property int gridSize: 4
    property int cellSize: 80
    property color backgroundColor: "#171717"
    property color borderColor: "#888888"

    property alias gridManager: gridManager
    property alias dragDropManager: dragDropManager
    property alias gridRepeater: gridRepeater
    property alias multiItemContainer: multiItemContainer
    signal itemPlaced(var coords, var itemData)
    signal itemMoved(var fromCoords, var toCoords, var itemData)
    signal itemDeleted(var coords, var itemData)

    width: gridSize * cellSize + 20
    height: gridSize * cellSize + 20
    color: backgroundColor
    border.color: borderColor
    border.width: 2
    radius: 2

    Grid {
        id: mainGrid
        anchors.centerIn: parent
        //anchors.verticalCenterOffset: 15
        rows: gridSize
        columns: gridSize
        spacing: 2

        Repeater {
            id: gridRepeater
            model: gridSize * gridSize

            Rectangle {
                id: gridCell

                property int cellIndex: index
                property int gridRows: gridSize
                property int gridColumns: gridSize
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

                readonly property int cellRow: Math.floor(cellIndex / gridSize)
                readonly property int cellColumn: cellIndex % gridSize

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
                    gridManager.freeCells(Math.floor(cellIndex / gridSize), cellIndex % gridSize, itemWidth, itemHeight)
                }

                function restoreItem() {
                    placedItem.opacity = 1.0
                    resetColor()
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

            multiItem.rightClicked.connect(function(itemData) {
                dragDropManager.handleRightClick(itemData)
            })
        }
    }

    GridManager {
        id: gridManager
        gridRepeater: gridRepeater
        multiItemContainer: multiItemContainer
        multiItemComponent: multiItemComponent
        gridSize: gridContainer.gridSize
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
    }

    DragDropManager {
        id: dragDropManager
        gridRepeater: gridRepeater
        multiItemContainer: multiItemContainer
        gridSize: gridContainer.gridSize
        cellSize: gridContainer.cellSize

        onItemPlaced: function(coords, itemData) {
            gridManager.occupyCells(coords.row, coords.col, itemData.itemWidth,
                                  itemData.itemHeight, itemData.itemType,
                                  itemData.itemColor, itemData.itemText, itemData.itemName)
        }

        onItemMoved: function(fromCoords, toCoords, itemData) {
            gridManager.moveItem(fromCoords.row, fromCoords.col, toCoords.row, toCoords.col,
                               itemData.itemWidth, itemData.itemHeight, itemData.itemType,
                               itemData.itemColor, itemData.itemText, itemData.itemName)
        }

        onItemDeleted: function(coords, itemData) {
            gridManager.freeCells(coords.row, coords.col, itemData.itemWidth, itemData.itemHeight)
        }
    }

    Component {
        id: multiItemComponent
        Rectangle {
            id: multiItem

            property string itemType: "rectangle"
            property string itemName: ""
            property color itemColor: "#ff0000"
            property string itemText: "Item"
            property int itemWidth: 1
            property int itemHeight: 1
            property int startRow: 0
            property int startCol: 0
            property var gridManager: null
            property int cellSize: 80
            signal dragStarted(var mouse, var itemData)
            signal dragMoved(var mouse, var itemData)
            signal dragEnded(var mouse, var itemData)
            signal rightClicked(var itemData)

            color: "transparent"
            border.color: "transparent"
            border.width: 0
            radius: 0


            Loader {
                id: customItemLoader
                anchors.fill: parent

                source: {
                    console.log("MultiItem customLoader - itemType:", multiItem.itemType, "itemName:", multiItem.itemName)
                    // Use itemName to identify specific component
                    switch(multiItem.itemName) {
                        case "RectangleSmall": return "../widgets/RectangleSmall.qml"
                        case "RectangleWide": return "../widgets/RectangleWide.qml"
                        case "RectangleFull": return "../widgets/RectangleFull.qml"
                        case "RectangleSquare": return "../widgets/RectangleSquare.qml"
                        case "RectangleLarge": return "../widgets/RectangleLarge.qml"
                        case "LightSmall": return "../widgets/LightSmall.qml"
                        case "LightMedium": return "../widgets/LightMedium.qml"
                        case "LightBig": return "../widgets/LightBig.qml"
                        default: return ""
                    }
                }

                onLoaded: {
                    if (item) {
                        item.itemWidth = multiItem.itemWidth
                        item.itemHeight = multiItem.itemHeight
                        item.isPaletteItem = false
                    }
                }
            }


            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                property bool dragActive: false

                onPressed: function(mouse) {
                    if (mouse.button === Qt.RightButton) {
                        console.log("Right click - deleting multi-item:", multiItem.itemText)

                        var itemData = {
                            itemType: multiItem.itemType,
                            itemName: multiItem.itemName,
                            itemColor: multiItem.itemColor,
                            itemText: multiItem.itemText,
                            itemWidth: multiItem.itemWidth,
                            itemHeight: multiItem.itemHeight,
                            startRow: multiItem.startRow,
                            startCol: multiItem.startCol,
                            sourceType: "grid",
                            sourceCellIndex: multiItem.startRow * gridSize + multiItem.startCol,
                        }

                        rightClicked(itemData)
                        return
                    }

                    if (mouse.button === Qt.LeftButton) {
                        console.log("Pressed multi-item:", multiItem.itemText)
                        dragActive = true
                        multiItem.opacity = 0.3

                        var itemData = {
                            itemType: multiItem.itemType,
                            itemName: multiItem.itemName,
                            itemColor: multiItem.itemColor,
                            itemText: multiItem.itemText,
                            itemWidth: multiItem.itemWidth,
                            itemHeight: multiItem.itemHeight,
                            startRow: multiItem.startRow,
                            startCol: multiItem.startCol,
                            sourceType: "grid",
                            sourceCellIndex: multiItem.startRow * gridSize + multiItem.startCol,
                        }

                        dragStarted(mouse, itemData)
                    }
                }

                onPositionChanged: function(mouse) {
                    if (dragActive && pressed && pressedButtons & Qt.LeftButton) {
                        var itemData = {
                            itemType: multiItem.itemType,
                            itemName: multiItem.itemName,
                            itemColor: multiItem.itemColor,
                            itemText: multiItem.itemText,
                            itemWidth: multiItem.itemWidth,
                            itemHeight: multiItem.itemHeight,
                            startRow: multiItem.startRow,
                            startCol: multiItem.startCol,
                            sourceType: "grid",
                            sourceCellIndex: multiItem.startRow * gridSize + multiItem.startCol,
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
                        itemName: multiItem.itemName,
                        itemColor: multiItem.itemColor,
                        itemText: multiItem.itemText,
                        itemWidth: multiItem.itemWidth,
                        itemHeight: multiItem.itemHeight,
                        startRow: multiItem.startRow,
                        startCol: multiItem.startCol,
                        sourceType: "grid",
                        sourceCellIndex: multiItem.startRow * gridSize + multiItem.startCol,
                    }

                    dragEnded(mouse, itemData)
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
        gridManager.initializeCells()
    }
    function placePredefinedItem(row, col, itemType, itemColor, itemText, itemWidth, itemHeight) {
        if (gridManager.canPlaceItem(row, col, itemWidth, itemHeight)) {
            gridManager.occupyCells(row, col, itemWidth, itemHeight, itemType, itemColor, itemText, itemType)
            return true
        }
        return false
    }

    function clearGrid() {
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
}
