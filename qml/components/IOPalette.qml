import QtQuick
import QtQuick.Layouts

Rectangle {
    id: itemPalette

    property string title: "Palette"
    property int titleSize: 16

    // Template sizes for the palette
    property var templateSizes: [
        { w: 1, h: 1 },
        { w: 2, h: 1 },
        { w: 1, h: 2 },
        { w: 2, h: 2 },
        { w: 3, h: 1 },
        { w: 3, h: 2 },
    ]

    signal itemDragStarted(var mouse, var sourceItem, var itemData)
    signal itemDragMoved(var mouse, var sourceItem, var itemData)
    signal itemDragEnded(var mouse, var sourceItem, var itemData)

    color: "#f0f0f0"
    border.color: "#cccccc"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            id: titleText
            Layout.alignment: Qt.AlignHCenter
            text: title
            font.bold: true
            font.pixelSize: titleSize
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#ccc"
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Widget Templates"
            font.pixelSize: 11
            color: "#666"
        }

        // Grid of template widgets
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            rowSpacing: 8
            columnSpacing: 8

            Repeater {
                model: templateSizes

                RectWidget {
                    id: templateWidget

                    property var sizeData: templateSizes[index]

                    itemWidth: sizeData.w
                    itemHeight: sizeData.h
                    itemName: sizeData.w + "×" + sizeData.h
                    itemType: "RectWidget"
                    isPaletteItem: true

                    Layout.preferredWidth: 60 + sizeData.w * 10
                    Layout.preferredHeight: 40 + sizeData.h * 10
                    Layout.alignment: Qt.AlignHCenter

                    // Connect drag signals from mouseArea
                    Connections {
                        target: templateWidget.mouseArea

                        function onDragPressed(mouse) {
                            var itemData = templateWidget.getItemData()
                            itemData.sourceType = "palette"
                            itemData.sourceCellIndex = -1
                            itemPalette.itemDragStarted(mouse, templateWidget, itemData)
                        }

                        function onDragMoved(mouse) {
                            var itemData = templateWidget.getItemData()
                            itemData.sourceType = "palette"
                            itemData.sourceCellIndex = -1
                            itemPalette.itemDragMoved(mouse, templateWidget, itemData)
                        }

                        function onDragReleased(mouse) {
                            var itemData = templateWidget.getItemData()
                            itemData.sourceType = "palette"
                            itemData.sourceCellIndex = -1
                            itemPalette.itemDragEnded(mouse, templateWidget, itemData)
                        }
                    }
                }
            }
        }

        // Spacer
        Item {
            Layout.fillHeight: true
        }
    }

    // Helper function to add custom template
    function addTemplate(width, height) {
        var newTemplates = templateSizes.slice()
        newTemplates.push({ w: width, h: height })
        templateSizes = newTemplates
    }

    // Helper function to clear templates
    function clearTemplates() {
        templateSizes = []
    }
}
