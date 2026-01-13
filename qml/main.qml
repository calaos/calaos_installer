import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"

Item {
    id: root

    implicitWidth: 1200
    implicitHeight: 800

    property int gridSize: 3
    property int cellSize: 80

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // Left panel - IOPalette
        IOPalette {
            id: itemPalette
            SplitView.minimumWidth: 150
            SplitView.preferredWidth: 200
            SplitView.maximumWidth: 300
            title: "IO Palette"

            onItemDragStarted: function(mouse, sourceItem, itemData) {
                console.log("Main.qml onItemDragStarted received")
                pageEditor.gridContainer.dragDropManager.startDragFromPalette(mouse, sourceItem, itemData)
            }

            onItemDragMoved: function(mouse, sourceItem, itemData) {
                pageEditor.gridContainer.dragDropManager.updateDrag(mouse, sourceItem, itemData)
            }

            onItemDragEnded: function(mouse, sourceItem, itemData) {
                pageEditor.gridContainer.dragDropManager.endDragFromPalette(mouse, sourceItem, itemData)
            }
        }

        // Center panel - Page Editor
        PageEditor {
            id: pageEditor
            SplitView.fillWidth: true
            SplitView.minimumWidth: 400
            gridSize: root.gridSize
            cellSize: root.cellSize
            draggedItem: draggedItem

            Component.onCompleted: {
                console.log("Setting draggedItem:", draggedItem)
                gridContainer.dragDropManager.draggedItem = draggedItem
                gridContainer.dragDropManager.flickable = pageEditor.flickable
                console.log("Setting flickable:", pageEditor.flickable)
            }

            onItemPlaced: function(coords, itemData) {
                console.log("Item placed:", itemData.itemText, "at", coords.row, coords.col)
            }

            onItemMoved: function(fromCoords, toCoords, itemData) {
                console.log("Item moved:", itemData.itemText, "from", fromCoords.row, fromCoords.col, "to", toCoords.row, toCoords.col)
            }

            onItemDeleted: function(coords, itemData) {
                console.log("Item deleted:", itemData.itemText, "at", coords.row, coords.col)
            }

            onPageChanged: function(pageIndex) {
                var pageInfo = pageEditor.getCurrentPageInfo()
                propertiesPanel.updatePageInfo(pageInfo.name, pageInfo.type)
                console.log("Switched to page:", pageInfo.name)
            }
        }

        // Right panel - Properties
        PropertiesPanel {
            id: propertiesPanel
            SplitView.minimumWidth: 200
            SplitView.preferredWidth: 250
            SplitView.maximumWidth: 350
            gridWidth: root.gridSize
            gridHeight: root.gridSize

            Component.onCompleted: {
                var pageInfo = pageEditor.getCurrentPageInfo()
                updatePageInfo(pageInfo.name, pageInfo.type)
            }
        }
    }

    DraggedItem {
        id: draggedItem
        cellSize: root.cellSize
        z: 1000
    }

    Item {
        anchors.fill: parent
        focus: true
        z: -1

        Keys.onPressed: function(event) {
            switch(event.key) {
            case Qt.Key_C:
                if (event.modifiers & Qt.ControlModifier) {
                    pageEditor.gridContainer.clearGrid()
                    console.log("Grid cleared")
                }
                break
            case Qt.Key_R:
                if (event.modifiers & Qt.ControlModifier) {
                    pageEditor.gridContainer.placePredefinedItem(0, 0, "RectangleSmall", "#ff0000", "Test", 1, 1)
                }
                break
            }
        }
    }
}
