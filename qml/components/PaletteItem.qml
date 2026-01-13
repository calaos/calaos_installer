import QtQuick

Rectangle {
    id: paletteItem

    property string itemType: "RectangleSmall"
    property color itemColor: "#ff0000"
    property string itemText: "Item"
    property int itemWidth: 1    // Width in cells
    property int itemHeight: 1   // Height in cells

    // Signals
    signal dragStarted(var mouse, var itemData)
    signal dragMoved(var mouse, var itemData)
    signal dragEnded(var mouse, var itemData)

    // Visual size proportional to cell size
    width: Math.min(100, 20 + itemWidth * 15)
    height: Math.min(80, 20 + itemHeight * 15)
    color: itemColor
    border.color: "#333333"
    border.width: 1
    radius: 4

    Text {
        anchors.centerIn: parent
        text: itemText
        color: "white"
        font.bold: true
        font.pixelSize: 12
    }

    MouseArea {
        id: paletteMouseArea
        anchors.fill: parent

        property bool dragActive: false

        onPressed: function(mouse) {
            console.log("Pressed palette item:", paletteItem.itemText)
            dragActive = true

            var itemData = {
                itemType: paletteItem.itemType,
                itemColor: paletteItem.itemColor,
                itemText: paletteItem.itemText,
                itemWidth: paletteItem.itemWidth,
                itemHeight: paletteItem.itemHeight,
                sourceType: "palette",
                sourceCellIndex: -1
            }

            dragStarted(mouse, itemData)
        }

        onPositionChanged: function(mouse) {
            if (dragActive && pressed) {
                var itemData = {
                    itemType: paletteItem.itemType,
                    itemColor: paletteItem.itemColor,
                    itemText: paletteItem.itemText,
                    itemWidth: paletteItem.itemWidth,
                    itemHeight: paletteItem.itemHeight,
                    sourceType: "palette",
                    sourceCellIndex: -1
                }

                dragMoved(mouse, itemData)
            }
        }

        onReleased: function(mouse) {
            console.log("Released palette item")
            dragActive = false

            var itemData = {
                itemType: paletteItem.itemType,
                itemColor: paletteItem.itemColor,
                itemText: paletteItem.itemText,
                itemWidth: paletteItem.itemWidth,
                itemHeight: paletteItem.itemHeight,
                sourceType: "palette",
                sourceCellIndex: -1
            }

            dragEnded(mouse, itemData)
        }
    }
}