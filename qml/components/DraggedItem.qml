import QtQuick

Rectangle {
    id: draggedItem

    property string itemType: "RectangleSmall"
    property color itemColor: "#ff0000"
    property string itemText: "Item"
    property string sourceType: "palette" // "palette" or "grid"
    property int sourceCellIndex: -1
    property int itemWidth: 1    // Width in cells
    property int itemHeight: 1   // Height in cells

    // Property for cell size
    property int cellSize: 80

    width: (cellSize + 2) * itemWidth - 2
    height: (cellSize + 2) * itemHeight - 2
    color: itemColor
    border.color: "#333333"
    border.width: 2
    radius: 4
    visible: false
    z: 1000
    opacity: 0.8

    Text {
        anchors.centerIn: parent
        text: draggedItem.itemText
        color: "white"
        font.bold: true
        font.pixelSize: 10
    }

    // Public functions
    function updateFromItemData(itemData) {
        itemType = itemData.itemType
        itemColor = itemData.itemColor
        itemText = itemData.itemText
        itemWidth = itemData.itemWidth
        itemHeight = itemData.itemHeight
        sourceType = itemData.sourceType
        sourceCellIndex = itemData.sourceCellIndex
    }

    function show() {
        visible = true
    }

    function hide() {
        visible = false
    }

    function updatePosition(globalX, globalY) {
        x = globalX - cellSize / 2
        y = globalY - cellSize
    }
}
