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
            uiTitle: remoteUIModel ? remoteUIModel.title : "Remote UI"
            pages: root.pagesList

            // Sync title changes back to model
            onUiTitleChanged: {
                if (remoteUIModel && remoteUIModel.title !== uiTitle) {
                    remoteUIModel.title = uiTitle
                }
            }

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
                        newPage.pageType = "default"
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
                        newPage.pageType = srcPage ? srcPage.pageType : "default"
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
                    // Update displayed IO name on the widget
                    propertiesPanel.selectedWidget.ioName = propertiesPanel.ioName || ""

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

            onNameOverrideChanged: function(newName) {
                console.log("Name override changed:", newName)
                if (propertiesPanel.selectedWidget) {
                    propertiesPanel.selectedWidget.nameOverride = newName

                    if (remoteUIModel && propertiesPanel.selectedItemData) {
                        var data = propertiesPanel.selectedItemData
                        updateWidgetName(data.startCol, data.startRow, newName)
                    }
                }
            }

            onCategoryChanged: function(newCategory) {
                console.log("Category changed to:", newCategory)
                if (propertiesPanel.selectedWidget && propertiesPanel.selectedItemData) {
                    var widget = propertiesPanel.selectedWidget
                    var data = propertiesPanel.selectedItemData

                    if (newCategory === "Clock") {
                        // Switch to Clock: set type to "Clock", clear IO
                        widget.itemType = "Clock"
                        widget.widgetCategory = "Clock"
                        widget.ioId = ""
                        widget.ioName = ""
                        widget.nameOverride = ""
                        updateWidgetCategory(data.startCol, data.startRow, "Clock")
                    } else {
                        // Switch to IO: clear type, set category
                        widget.itemType = "Unknown"
                        widget.widgetCategory = "IO"
                        updateWidgetCategory(data.startCol, data.startRow, "IO")
                    }
                }
            }

            onClockPropertyChanged: function(key, value) {
                console.log("Clock property changed:", key, "=", value)
                if (remoteUIModel && propertiesPanel.selectedItemData) {
                    var data = propertiesPanel.selectedItemData
                    updateWidgetClockProperty(data.startCol, data.startRow, key, value)
                }
            }
        }
    }

    // Connect remoteUIEditor signals for IO selection
    Connections {
        target: remoteUIEditor

        function onIoSelected(ioId, ioName, roomName, widgetType) {
            console.log("IO selected from dialog:", ioId, ioName, "in", roomName)

            // Check if this is from context menu Link IO
            if (root.pendingLinkWidget && root.pendingLinkItemData) {
                var widget = root.pendingLinkWidget
                var itemData = root.pendingLinkItemData

                // Update the widget visually
                widget.ioId = ioId
                widget.itemType = widgetType
                widget.ioName = ioName

                // Update the model
                updateWidgetIO(itemData.startCol, itemData.startRow, ioId, widgetType)

                // Update properties panel if this widget is selected
                if (propertiesPanel.selectedWidget === widget) {
                    propertiesPanel.onIOSelected(ioId, ioName, roomName, widgetType)
                }

                // Clear pending
                root.pendingLinkWidget = null
                root.pendingLinkItemData = null
            } else {
                // Normal flow from properties panel
                propertiesPanel.onIOSelected(ioId, ioName, roomName, widgetType)
            }
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
            // Update the underlying model with new size
            updateWidgetSize(itemData, itemData.itemWidth, itemData.itemHeight)

            // Update properties panel if the resized widget is currently selected
            if (propertiesPanel.selectedWidget === widget) {
                propertiesPanel.updateSelectedItem(widget, itemData)
            }
        }

        function onLinkIORequested(widget, itemData) {
            // Store the widget for later update when IO is selected
            root.pendingLinkWidget = widget
            root.pendingLinkItemData = itemData
            // Open the IO selector dialog
            var currentIoId = itemData.ioId || ""
            remoteUIEditor.openIOSelector(currentIoId)
        }
    }

    // Pending widget for Link IO from context menu
    property var pendingLinkWidget: null
    property var pendingLinkItemData: null

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
                    var cat = widget.category  // Derived from type in C++

                    // Resolve IO name for display (only for IO widgets)
                    var resolvedIoName = ""
                    if (cat === "IO" && widget.ioId) {
                        var ioInfo = remoteUIEditor.validateIO(widget.ioId)
                        if (ioInfo.exists) {
                            resolvedIoName = ioInfo.ioName || ""
                        }
                    }

                    if (cat === "Clock") {
                        itemName = "Clock"
                    }

                    console.log("Placing widget:", itemName, "category:", cat, "at", widget.x, widget.y, "size:", widget.w, widget.h, "ioId:", widget.ioId, "name:", widget.name)

                    pageEditor.gridContainer.placePredefinedItem(
                        widget.y, widget.x,
                        "RectWidget", color,
                        itemText,
                        widget.w, widget.h,
                        widget.ioId || "",
                        widget.name || "",
                        resolvedIoName,
                        cat
                    )

                    // For Clock widgets, set clock properties on the RectWidget after placement
                    if (cat === "Clock") {
                        setClockPropertiesOnWidget(widget.x, widget.y, widget)
                    }
                }
            }
        })
    }

    function updateWidgetPosition(itemData, newX, newY) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        var oldX = itemData.startCol
        var oldY = itemData.startRow
        var itemW = itemData.itemWidth || 1
        var itemH = itemData.itemHeight || 1

        // Validate new coordinates are within grid bounds
        if (newX < 0 || newY < 0 || newX + itemW > root.gridColumns || newY + itemH > root.gridRows) {
            console.log("updateWidgetPosition: Invalid coordinates", newX, newY, "for widget size", itemW, "x", itemH)
            return
        }

        // Find the widget by its original position
        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.x === oldX && widget.y === oldY) {
                widget.x = newX
                widget.y = newY
                console.log("Updated widget position from", oldX, oldY, "to", newX, newY)
                break
            }
        }
    }

    function updateWidgetSize(itemData, newWidth, newHeight) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        // Use position to find the widget (position doesn't change during resize)
        var x = itemData.startCol
        var y = itemData.startRow

        // Validate new size is within grid bounds
        if (newWidth < 1 || newHeight < 1 || x + newWidth > root.gridColumns || y + newHeight > root.gridRows) {
            console.log("updateWidgetSize: Invalid size", newWidth, "x", newHeight, "for widget at", x, y)
            return
        }

        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.x === x && widget.y === y) {
                widget.w = newWidth
                widget.h = newHeight
                console.log("Updated widget size at", x, y, "to", newWidth, "x", newHeight)
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

    function updateWidgetName(x, y, newName) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.x === x && widget.y === y) {
                widget.name = newName
                console.log("Updated widget name at", x, y, "to", newName || "(cleared)")
                break
            }
        }
    }

    function updateWidgetCategory(x, y, newCategory) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.x === x && widget.y === y) {
                if (newCategory === "Clock") {
                    widget.type = "Clock"
                    widget.ioId = ""
                    widget.name = ""
                } else {
                    // Switching to IO: reset type, keep ioId empty until user selects
                    widget.type = "Unknown"
                }
                console.log("Updated widget category at", x, y, "to", newCategory, "type:", widget.type)
                break
            }
        }
    }

    function updateWidgetClockProperty(x, y, key, value) {
        if (!remoteUIModel || !remoteUIModel.currentPage()) return

        var page = remoteUIModel.currentPage()
        for (var i = 0; i < page.widgetCount; i++) {
            var widget = page.widgetAt(i)
            if (widget && widget.x === x && widget.y === y) {
                switch (key) {
                    case "clock_timezone":
                        widget.clockTimezone = value
                        break
                    case "clock_format":
                        widget.clockFormat = value
                        break
                    case "clock_show_date":
                        widget.clockShowDate = (value === "true")
                        break
                    case "clock_date_format":
                        widget.clockDateFormat = value
                        break
                    case "clock_seconds":
                        widget.clockSeconds = (value === "true")
                        break
                }
                console.log("Updated clock property at", x, y, key, "=", value)
                break
            }
        }
    }

    function setClockPropertiesOnWidget(x, y, modelWidget) {
        // Find the visual RectWidget at position x,y and set clock-specific properties
        var widgets = pageEditor.gridContainer.multiItemContainer.children
        for (var i = 0; i < widgets.length; i++) {
            var w = widgets[i]
            if (w && w.startCol === x && w.startRow === y) {
                w.clockTimezone = modelWidget.clockTimezone || "UTC"
                w.clockFormat = modelWidget.clockFormat || "24"
                w.clockShowDate = modelWidget.clockShowDate !== undefined ? modelWidget.clockShowDate : true
                w.clockDateFormat = modelWidget.clockDateFormat || "DD/MM/YYYY"
                w.clockSeconds = modelWidget.clockSeconds !== undefined ? modelWidget.clockSeconds : false
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
                        type: page.pageType || "default"
                    })
                }
            }
        }
        // Return default if no pages
        if (list.length === 0) {
            list.push({ name: "Page 1", type: "default" })
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

    // Function to highlight an invalid widget (called from C++ after validation fails)
    function highlightInvalidWidget(pageIndex, widgetX, widgetY) {
        console.log("Highlighting invalid widget at page", pageIndex, "position", widgetX, widgetY)

        // Switch to the page containing the invalid widget
        if (remoteUIModel && pageIndex !== remoteUIModel.currentPageIndex) {
            remoteUIModel.currentPageIndex = pageIndex
            pageEditor.pageSelector.currentPageIndex = pageIndex
            loadPageWidgets(pageIndex)
        }

        // Find and select the widget at the specified position
        Qt.callLater(function() {
            var widgets = pageEditor.gridContainer.multiItemContainer.children
            for (var i = 0; i < widgets.length; i++) {
                var w = widgets[i]
                if (w && w.startCol === widgetX && w.startRow === widgetY) {
                    // Select this widget
                    pageEditor.gridContainer.selectWidget(w)
                    break
                }
            }
        })
    }
}
