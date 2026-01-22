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
                if (remoteUIModel) {
                    console.log("Current page index in model:", remoteUIModel.currentPageIndex)
                    var page = remoteUIModel.currentPage()
                    if (page) {
                        console.log("Creating widget on page:", page.name, "widgetCount before:", page.widgetCount)
                        page.createWidget(itemData.ioId || "", itemData.itemType || "Unknown",
                                         coords.col, coords.row,
                                         itemData.itemWidth || 1, itemData.itemHeight || 1)
                        console.log("Widget created, widgetCount after:", page.widgetCount)
                    } else {
                        console.log("ERROR: currentPage() returned null!")
                    }
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

                // Clear selection when changing pages
                pageEditor.gridContainer.clearSelection()

                // Update the model's current page and load widgets
                if (remoteUIModel) {
                    remoteUIModel.currentPageIndex = pageIndex
                    loadPageWidgets(pageIndex)
                }
            }

            // Page management - sync with model
            onPageAddRequested: {
                if (remoteUIModel) {
                    var newPage = remoteUIModel.addPage()
                    if (newPage) {
                        newPage.name = "Page " + remoteUIModel.pageCount
                        newPage.pageType = "Default"
                        console.log("Created new page:", newPage.name, "at index", remoteUIModel.pageCount - 1)
                        // Rebuild pages list from model
                        root.pagesList = buildPagesList()
                        // Set the current page to the new one
                        var newIndex = remoteUIModel.pageCount - 1
                        remoteUIModel.currentPageIndex = newIndex
                        pageEditor.pageSelector.currentPageIndex = newIndex
                        console.log("Set currentPageIndex to", newIndex)
                        // Load the new empty page (clear the grid)
                        loadPageWidgets(newIndex)
                    }
                }
            }

            onPageDeleteRequested: function(pageIndex) {
                if (remoteUIModel && remoteUIModel.pageCount > 1) {
                    remoteUIModel.removePage(pageIndex)
                    root.pagesList = buildPagesList()
                    // Adjust current page index
                    var newIndex = remoteUIModel.currentPageIndex
                    pageEditor.pageSelector.currentPageIndex = newIndex
                    // Load current page after deletion
                    loadPageWidgets(newIndex)
                }
            }

            onPageDuplicateRequested: function(pageIndex) {
                if (remoteUIModel) {
                    // TODO: Implement full page duplication with widgets in model
                    // For now, just add a new page with same name/type
                    var newPage = remoteUIModel.addPage()
                    if (newPage) {
                        var srcPage = remoteUIModel.pageAt(pageIndex)
                        newPage.name = srcPage ? (srcPage.name + " Copy") : "Page Copy"
                        newPage.pageType = srcPage ? srcPage.pageType : "Default"
                        root.pagesList = buildPagesList()
                        var newIndex = remoteUIModel.pageCount - 1
                        remoteUIModel.currentPageIndex = newIndex
                        pageEditor.pageSelector.currentPageIndex = newIndex
                        loadPageWidgets(newIndex)
                    }
                }
            }

            onPageRenameRequested: function(pageIndex, newName) {
                if (remoteUIModel) {
                    var page = remoteUIModel.pageAt(pageIndex)
                    if (page) {
                        page.name = newName
                        root.pagesList = buildPagesList()
                    }
                }
            }

            onPageTypeChangeRequested: function(pageIndex, newType) {
                if (remoteUIModel) {
                    var page = remoteUIModel.pageAt(pageIndex)
                    if (page) {
                        page.pageType = newType
                        root.pagesList = buildPagesList()
                    }
                }
            }
        }

        // Right panel - Properties
        PropertiesPanel {
            id: propertiesPanel
            SplitView.minimumWidth: minimumContentWidth
            SplitView.preferredWidth: Math.max(260, minimumContentWidth)
            SplitView.maximumWidth: 600
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

            onDeleteItemRequested: {
                console.log("Delete item requested from PropertiesPanel")
                pageEditor.gridContainer.deleteSelectedItem()
            }

            onClearGridRequested: {
                console.log("Clear grid requested from PropertiesPanel")
                pageEditor.gridContainer.clearGrid()
            }

            onIoChanged: function(newIoId, newWidgetType) {
                console.log("IO changed:", newIoId, "widget type:", newWidgetType)
                // Update the selected widget with new IO
                if (propertiesPanel.selectedWidget) {
                    propertiesPanel.selectedWidget.ioId = newIoId
                    propertiesPanel.selectedWidget.itemType = newWidgetType

                    // Update the model
                    if (remoteUIModel && propertiesPanel.selectedItemData) {
                        var page = remoteUIModel.currentPage()
                        if (page) {
                            // Find the widget in the model and update it
                            var data = propertiesPanel.selectedItemData
                            updateWidgetIO(data.startCol, data.startRow, newIoId, newWidgetType)
                        }
                    }
                }
            }
        }
    }

    // Connect remoteUIEditor signals for IO selection
    Connections {
        target: remoteUIEditor

        function onIoSelected(ioId, ioName, roomName, widgetType) {
            console.log("IO selected from dialog:", ioId, ioName, "in", roomName)
            propertiesPanel.onIOSelected(ioId, ioName, roomName, widgetType)
        }
    }

    // Keyboard handling for Escape to deselect
    Keys.onEscapePressed: {
        pageEditor.gridContainer.clearSelection()
    }

    // Make sure root can receive keyboard focus
    focus: true

    // Connect grid container selection signals to properties panel
    Connections {
        target: pageEditor.gridContainer

        function onItemSelected(widget, itemData) {
            propertiesPanel.updateSelectedItem(widget, itemData)
        }

        function onItemDeselected() {
            propertiesPanel.clearSelectedItem()
        }

        function onItemResized(widget, itemData) {
            // Update properties panel if the resized widget is currently selected
            if (propertiesPanel.selectedWidget === widget) {
                propertiesPanel.updateSelectedItem(widget, itemData)
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
        if (!page) {
            console.log("loadPageWidgets: No page at index", pageIndex)
            return
        }

        console.log("loadPageWidgets: Loading page", pageIndex, "(", page.name, ") with", page.widgetCount, "widgets")

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

                    console.log("Placing widget:", itemName, "at", widget.x, widget.y, "size:", widget.w, widget.h, "ioId:", widget.ioId)

                    pageEditor.gridContainer.placePredefinedItem(
                        widget.y, widget.x,
                        "RectWidget", color,
                        itemText,
                        widget.w, widget.h,
                        widget.ioId || ""
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

    function updateWidgetIO(x, y, newIoId, newWidgetType) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.x === x && widget.y === y) {
                widget.ioId = newIoId
                widget.type = newWidgetType
                console.log("Updated widget IO at", x, y, "to", newIoId, newWidgetType)
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
