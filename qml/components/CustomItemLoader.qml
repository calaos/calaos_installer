import QtQuick

Loader {
    id: customItemLoader

    property string itemType: ""
    property int cellWidth: 1
    property int cellHeight: 1
    property bool isPaletteItem: false

    // Load custom component based on type
    source: {
        switch(itemType) {
            case "LightSmall": return "LightSmall.qml"
            case "LightMedium": return "LightMedium.qml"
            case "LightBig": return "LightBig.qml"
            default: return ""
        }
    }

    // Fill entire cell
    anchors.fill: parent

    // MouseArea to forward events to parent cell if not a palette item
    MouseArea {
        id: loaderMouseArea
        anchors.fill: parent
        enabled: !isPaletteItem
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: function(mouse) {
            // Forward event to parent
            mouse.accepted = false
        }

        onPositionChanged: function(mouse) {
            mouse.accepted = false
        }

        onReleased: function(mouse) {
            mouse.accepted = false
        }
    }

    onLoaded: {
        if (item) {
            // Configure custom item
            item.itemWidth = cellWidth
            item.itemHeight = cellHeight
            item.isPaletteItem = isPaletteItem

            // Disable MouseArea if not a palette item
            if (item.mouseArea && !isPaletteItem) {
                item.mouseArea.enabled = false
            }

            // Fill cell
            item.anchors.fill = customItemLoader
        }
    }
}