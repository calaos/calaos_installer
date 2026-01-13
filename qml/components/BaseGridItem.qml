import QtQuick

Rectangle {
    id: baseItem

    // Configurable properties for each custom item
    property string itemName: "BaseItem"
    property string itemType: "RectangleSmall"
    property color itemColor: "#ff0000"
    property string itemText: "Item"
    property int itemWidth: 1    // Width in cells
    property int itemHeight: 1   // Height in cells

    // Customizable visual properties
    property color borderColor: "#333333"
    property int borderWidth: 2
    property int itemRadius: 4
    property color textColor: "white"
    property int textSize: 10
    property bool textBold: true

    // Internal properties (do not modify)
    property int cellSize: 80
    property bool isPaletteItem: false
    property alias mouseArea: mouseArea  // Expose MouseArea for external connections

    // Item size (auto-calculated or overridden)
    width: isPaletteItem ? Math.min(100, 20 + itemWidth * 15) : cellSize * itemWidth
    height: isPaletteItem ? Math.min(80, 20 + itemHeight * 15) : cellSize * itemHeight

    // Basic appearance
    color: itemColor
    border.color: borderColor
    border.width: borderWidth
    radius: itemRadius

    // Basic content - can be overridden in subclasses
    Item {
        id: contentArea
        anchors.fill: parent
        anchors.margins: borderWidth

        Text {
            id: defaultText
            anchors.centerIn: parent
            text: baseItem.itemText
            color: textColor
            font.bold: textBold
            font.pixelSize: textSize
        }
    }

    // MouseArea for drag & drop (above all custom content)
    MouseArea {
        id: mouseArea
        anchors.fill: parent
    z: 10  // Above all custom content

        property bool dragActive: false

    // Signals will be connected by parent (ItemPalette or GridContainer)
        signal dragPressed(var mouse)
        signal dragMoved(var mouse)
        signal dragReleased(var mouse)

        onPressed: function(mouse) {
            console.log("BaseGridItem MouseArea pressed - itemType:", baseItem.itemType)
            dragActive = true
            dragPressed(mouse)
        }

        onPositionChanged: function(mouse) {
            if (dragActive && pressed) {
                dragMoved(mouse)
            }
        }

        onReleased: function(mouse) {
            dragActive = false
            dragReleased(mouse)
        }
    }

    // Get item data
    function getItemData() {
        return {
            itemName: itemName,
            itemType: itemType,
            itemColor: itemColor,
            itemText: itemText,
            itemWidth: itemWidth,
            itemHeight: itemHeight
        }
    }

    // Component.onCompleted is now handled in each subclass
}