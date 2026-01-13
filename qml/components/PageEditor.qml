import QtQuick
import QtQuick.Controls

Rectangle {
    id: pageEditor

    property int gridSize: 4
    property int cellSize: 80
    property string uiTitle: "Remote UI Title"
    property string originalTitle: uiTitle
    property alias gridContainer: gridContainer
    property alias pageSelector: pageSelector
    property var draggedItem: null
    property alias flickable: gridFlickable

    signal itemPlaced(var coords, var itemData)
    signal itemMoved(var fromCoords, var toCoords, var itemData)
    signal itemDeleted(var coords, var itemData)
    signal pageChanged(int pageIndex)

    color: "#f8f8f8"
    border.color: "#ddd"
    border.width: 1

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Title
        Rectangle {
            width: parent.width
            height: 50
            color: "transparent"

            Row {
                anchors.centerIn: parent
                spacing: 10

                TextField {
                    id: titleField
                    text: uiTitle
                    font.pixelSize: 24
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    selectByMouse: true
                    width: Math.max(200, contentWidth + 40)

                    onTextChanged: {
                        originalTitle = pageEditor.originalTitle
                    }

                    onEditingFinished: {
                        if (!applyButton.visible && text.trim() !== uiTitle) {
                            uiTitle = text.trim()
                            originalTitle = uiTitle
                            console.log("UI title changed to:", uiTitle)
                        }
                    }
                }

                // Apply/Cancel buttons - only visible when text is modified
                Row {
                    spacing: 5
                    visible: titleField.text.trim() !== originalTitle && titleField.text.trim() !== ""

                    Button {
                        id: applyButton
                        width: 32
                        height: 32
                        text: "✓"
                        font.pixelSize: 16
                        font.bold: true
                        background: Rectangle {
                            color: parent.pressed ? "#4CAF50" : parent.hovered ? "#66BB6A" : "#81C784"
                            radius: 4
                        }
                        onClicked: {
                            uiTitle = titleField.text.trim()
                            originalTitle = uiTitle
                            console.log("UI title applied:", uiTitle)
                        }
                    }

                    Button {
                        id: cancelButton
                        width: 32
                        height: 32
                        text: "✕"
                        font.pixelSize: 16
                        font.bold: true
                        background: Rectangle {
                            color: parent.pressed ? "#F44336" : parent.hovered ? "#EF5350" : "#E57373"
                            radius: 4
                        }
                        onClicked: {
                            titleField.text = originalTitle
                            console.log("UI title changes cancelled")
                        }
                    }
                }
            }
        }

        // Page Selector
        PageSelector {
            id: pageSelector
            width: parent.width

            onPageSelected: function(index) {
                console.log("Page selected:", index)
                pageChanged(index)
            }

            onPageAdded: function(index) {
                console.log("Page added at index:", index)
            }

            onPageRenamed: function(index, newName) {
                console.log("Page", index, "renamed to:", newName)
            }

            onPageDeleted: function(index) {
                console.log("Page", index, "deleted")
            }

            onPageDuplicated: function(index) {
                console.log("Page", index, "duplicated")
            }

            onPageTypeChanged: function(index, newType) {
                console.log("Page", index, "type changed to:", newType)
            }
        }

        // Grid in Flickable
        Rectangle {
            width: parent.width
            height: parent.height - titleField.height - pageSelector.height - 30
            color: "#ffffff"
            border.color: "#ccc"
            border.width: 1
            radius: 4

            Flickable {
                id: gridFlickable
                anchors.fill: parent
                anchors.margins: 5
                contentWidth: gridContainer.width
                contentHeight: gridContainer.height
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                property bool isDragging: false

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
                ScrollBar.horizontal: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }

                // Disable flickable when dragging
                interactive: !isDragging

                onInteractiveChanged: {
                    console.log("Flickable interactive changed to:", interactive, "isDragging:", isDragging)
                }

                GridContainer {
                    id: gridContainer
                    gridSize: pageEditor.gridSize
                    cellSize: pageEditor.cellSize

                    onItemPlaced: function(coords, itemData) {
                        pageEditor.itemPlaced(coords, itemData)
                    }

                    onItemMoved: function(fromCoords, toCoords, itemData) {
                        pageEditor.itemMoved(fromCoords, toCoords, itemData)
                    }

                    onItemDeleted: function(coords, itemData) {
                        pageEditor.itemDeleted(coords, itemData)
                    }
                }
            }
        }
    }

    // Helper functions
    function clearGrid() {
        gridContainer.clearGrid()
    }

    function addTestItem() {
        gridContainer.placePredefinedItem(0, 0, "RectangleSmall", "#ff0000", "Test", 1, 1)
    }

    function getCurrentPageInfo() {
        return {
            name: pageSelector.pages[pageSelector.currentPageIndex].name,
            type: pageSelector.pages[pageSelector.currentPageIndex].type,
            index: pageSelector.currentPageIndex
        }
    }

    function updateGridSize(newSize) {
        if (newSize !== gridSize) {
            gridSize = newSize
            console.log("Grid size updated to:", gridSize)
        }
    }

    Component.onCompleted: {
        console.log("PageEditor completed with grid size:", gridSize)
    }
}
