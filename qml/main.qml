import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"
import "utils/WidgetColors.js" as WidgetColors

Item {
    id: root

    implicitWidth: 1200
    implicitHeight: 800

    // Use model values if available, otherwise defaults
    // Support for non-square grids
    property int gridColumns: remoteUIModel ? remoteUIModel.gridW : 3  // Width
    property int gridRows: remoteUIModel ? remoteUIModel.gridH : 3     // Height
    property int cellSize: 80

    // Expose the model for child components
    property var model: remoteUIModel

    // Pages list property for UI binding
    property var pagesList: []

    Component.onCompleted: {
        pagesList = buildPagesList()
    }

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
            gridColumns: root.gridColumns
            gridRows: root.gridRows
            cellSize: root.cellSize
            draggedItem: draggedItem
            uiTitle: remoteUIName ? remoteUIName : "Remote UI"
            pages: root.pagesList

            Component.onCompleted: {
                console.log("Setting draggedItem:", draggedItem)
                gridContainer.dragDropManager.draggedItem = draggedItem
                gridContainer.dragDropManager.flickable = pageEditor.flickable
                console.log("Setting flickable:", pageEditor.flickable)

                // Load widgets from model for current page
                if (remoteUIModel && remoteUIModel.pageCount > 0) {
                    loadPageWidgets(0)
                }
            }

            onItemPlaced: function(coords, itemData) {
                console.log("Item placed:", itemData.itemText, "at", coords.row, coords.col)
                // Update the model when an item is placed
                if (remoteUIModel && remoteUIModel.currentPage()) {
                    var page = remoteUIModel.currentPage()
                    page.createWidget(itemData.ioId || "", itemData.itemType || "Unknown",
                                     coords.col, coords.row,
                                     itemData.itemWidth || 1, itemData.itemHeight || 1)
                }
            }

            onItemMoved: function(fromCoords, toCoords, itemData) {
                console.log("Item moved:", itemData.itemText, "from", fromCoords.row, fromCoords.col, "to", toCoords.row, toCoords.col)
                // Update widget position in the model
                updateWidgetPosition(itemData, toCoords.col, toCoords.row)
            }

            onItemDeleted: function(coords, itemData) {
                console.log("Item deleted:", itemData.itemText, "at", coords.row, coords.col)
                // Remove widget from model
                removeWidgetAtPosition(coords.col, coords.row)
            }

            onPageChanged: function(pageIndex) {
                var pageInfo = pageEditor.getCurrentPageInfo()
                propertiesPanel.updatePageInfo(pageInfo.name, pageInfo.type)
                console.log("Switched to page:", pageInfo.name)

                // Update the model's current page and load widgets
                if (remoteUIModel) {
                    remoteUIModel.currentPageIndex = pageIndex
                    loadPageWidgets(pageIndex)
                }
            }
        }

        // Right panel - Properties
        PropertiesPanel {
            id: propertiesPanel
            SplitView.minimumWidth: 200
            SplitView.preferredWidth: 250
            SplitView.maximumWidth: 350
            gridWidth: root.gridColumns
            gridHeight: root.gridRows

            Component.onCompleted: {
                var pageInfo = pageEditor.getCurrentPageInfo()
                updatePageInfo(pageInfo.name, pageInfo.type)
            }

            onGridWidthChanged: {
                if (remoteUIModel) {
                    remoteUIModel.gridW = gridWidth
                }
                root.gridColumns = gridWidth
            }

            onGridHeightChanged: {
                if (remoteUIModel) {
                    remoteUIModel.gridH = gridHeight
                }
                root.gridRows = gridHeight
            }
        }
    }

    DraggedItem {
        id: draggedItem
        cellSize: root.cellSize
        z: 1000
    }

    // Helper functions to interact with the model

    function loadPageWidgets(pageIndex) {
        if (!remoteUIModel) return

        var page = remoteUIModel.pageAt(pageIndex)
        if (!page) return

        console.log("loadPageWidgets: Loading page", pageIndex, "with", page.widgetCount, "widgets")

        // Clear the grid first
        pageEditor.gridContainer.clearGrid()

        // Use Qt.callLater to ensure grid is ready after clearing
        Qt.callLater(function() {
            // Load widgets from the page model
            for (var i = 0; i < page.widgetCount; i++) {
                var widget = page.widgetAt(i)
                if (widget) {
                    // Use size-based color from WidgetColors
                    var color = WidgetColors.getColorForSize(widget.w, widget.h)
                    var itemText = widget.w + "\u00d7" + widget.h
                    var itemName = widget.ioId || ("Widget_" + i)

                    console.log("Placing widget:", itemName, "at", widget.x, widget.y, "size:", widget.w, widget.h)

                    pageEditor.gridContainer.placePredefinedItem(
                        widget.y, widget.x,
                        "RectWidget", color,
                        itemText,
                        widget.w, widget.h
                    )
                }
            }
        })
    }

    function updateWidgetPosition(itemData, newX, newY) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        // Find the widget by its old position or ioId
        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.ioId === itemData.ioId) {
                widget.x = newX
                widget.y = newY
                break
            }
        }
    }

    function removeWidgetAtPosition(x, y) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.x === x && widget.y === y) {
                page.removeWidget(i)
                break
            }
        }
    }

    function getColorForWidgetType(widgetType, width, height) {
        // Now using size-based colors from WidgetColors.js
        return WidgetColors.getColorForSize(width || 1, height || 1)
    }

    // Build pages list from model for PageSelector
    function buildPagesList() {
        var list = []
        if (remoteUIModel) {
            for (var i = 0; i < remoteUIModel.pageCount; i++) {
                var page = remoteUIModel.pageAt(i)
                if (page) {
                    list.push({
                        name: page.name || ("Page " + (i + 1)),
                        type: page.pageType || "Default"
                    })
                }
            }
        }
        // Return default if no pages
        if (list.length === 0) {
            list.push({ name: "Page 1", type: "Default" })
        }
        return list
    }

    // Update pages list when model changes
    Connections {
        target: remoteUIModel
        function onPagesChanged() {
            root.pagesList = buildPagesList()
        }
    }
}
