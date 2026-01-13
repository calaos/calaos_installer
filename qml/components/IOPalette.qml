import QtQuick

Rectangle {
    id: itemPalette

    property string title: "Palette"
    property int titleSize: 16
    property int spacing: 15
    property var paletteItems: [
        // Custom rectangle items
        { type: "RectangleSmall", width: 1, height: 1 },
        { type: "RectangleWide", width: 2, height: 1 },
        { type: "RectangleFull", width: 4, height: 1 },
        { type: "RectangleSquare", width: 2, height: 2 },
        { type: "RectangleLarge", width: 4, height: 2 },
        // Custom light items
        { type: "LightSmall", width: 1, height: 1 },
        { type: "LightMedium", width: 4, height: 1 },
        { type: "LightBig", width: 4, height: 2 }
    ]
    signal itemDragStarted(var mouse, var sourceItem, var itemData)
    signal itemDragMoved(var mouse, var sourceItem, var itemData)
    signal itemDragEnded(var mouse, var sourceItem, var itemData)

    width: 120
    color: "#f0f0f0"
    border.color: "#cccccc"
    border.width: 1

    Text {
        id: titleText
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        text: title
        font.bold: true
        font.pixelSize: titleSize
    }

    Column {
        anchors.centerIn: parent
        spacing: itemPalette.spacing

        Repeater {
            model: paletteItems

            Loader {
                id: paletteItemLoader

                property var modelData: paletteItems[index]

                // Load custom items
                sourceComponent: customItemComponent

                onLoaded: {
                    // Configurer l'item chargé
                    if (item) {
                        item.isPaletteItem = true

                        // Connecter les signaux de drag EN PREMIER
                        if (item.dragStarted) {
                            item.dragStarted.connect(function(mouse, itemData) {
                                console.log("Main Loader dragStarted received")
                                itemPalette.itemDragStarted(mouse, item, itemData)
                            })
                        }
                        if (item.dragMoved) {
                            item.dragMoved.connect(function(mouse, itemData) {
                                itemPalette.itemDragMoved(mouse, item, itemData)
                            })
                        }
                        if (item.dragEnded) {
                            item.dragEnded.connect(function(mouse, itemData) {
                                itemPalette.itemDragEnded(mouse, item, itemData)
                            })
                        }

                        // Configurer les propriétés
                        item.cellWidth = modelData.width
                        item.cellHeight = modelData.height
                        item.itemType = modelData.type
                        // itemName sera défini dans le loader interne
                    }
                }
            }
        }
    }

    // Fonction pour ajouter un nouvel item à la palette
    function addPaletteItem(itemType, itemColor, itemText, itemWidth, itemHeight) {
        var newItems = paletteItems.slice() // Copie du tableau
        newItems.push({
            type: itemType,
            color: itemColor,
            text: itemText,
            width: itemWidth,
            height: itemHeight
        })
        paletteItems = newItems
    }

    // Fonction pour supprimer un item de la palette
    function removePaletteItem(index) {
        if (index >= 0 && index < paletteItems.length) {
            var newItems = paletteItems.slice() // Copie du tableau
            newItems.splice(index, 1)
            paletteItems = newItems
        }
    }

    // Fonction pour vider la palette
    function clearPalette() {
        paletteItems = []
    }

    // Composant pour items personnalisés
    Component {
        id: customItemComponent

        Loader {
            property bool isPaletteItem: false
            property int cellWidth: 1
            property int cellHeight: 1
            property string itemType: ""

            // Signaux pour le drag & drop
            signal dragStarted(var mouse, var itemData)
            signal dragMoved(var mouse, var itemData)
            signal dragEnded(var mouse, var itemData)

            // Charger le composant personnalisé basé sur le type
            source: {
                switch(itemType) {
                    case "RectangleSmall": return "../widgets/RectangleSmall.qml"
                    case "RectangleWide": return "../widgets/RectangleWide.qml"
                    case "RectangleFull": return "../widgets/RectangleFull.qml"
                    case "RectangleSquare": return "../widgets/RectangleSquare.qml"
                    case "RectangleLarge": return "../widgets/RectangleLarge.qml"
                    case "LightSmall": return "../widgets/LightSmall.qml"
                    case "LightMedium": return "../widgets/LightMedium.qml"
                    case "LightBig": return "../widgets/LightBig.qml"
                    default: return ""
                }
            }

            // Taille adaptée pour la palette
            width: Math.min(100, 20 + cellWidth * 15)
            height: Math.min(80, 20 + cellHeight * 15)

            onLoaded: {
                if (item) {
                    console.log("CustomItemComponent onLoaded - item type:", item.itemType, "mouseArea exists:", !!item.mouseArea)

                    // Configurer l'item personnalisé
                    item.itemWidth = cellWidth
                    item.itemHeight = cellHeight
                    item.isPaletteItem = true

                    // Connecter les signaux de la MouseArea du BaseGridItem aux signaux du Loader
                    if (item.mouseArea) {
                        console.log("Connecting mouseArea.dragPressed signal")
                        item.mouseArea.dragPressed.connect(function(mouse) {
                            console.log("CustomItemComponent dragPressed received")
                            var itemData = item.getItemData()
                            itemData.sourceType = "palette"
                            itemData.sourceCellIndex = -1
                            dragStarted(mouse, itemData)
                        })

                        item.mouseArea.dragMoved.connect(function(mouse) {
                            var itemData = item.getItemData()
                            itemData.sourceType = "palette"
                            itemData.sourceCellIndex = -1
                            dragMoved(mouse, itemData)
                        })

                        item.mouseArea.dragReleased.connect(function(mouse) {
                            var itemData = item.getItemData()
                            itemData.sourceType = "palette"
                            itemData.sourceCellIndex = -1
                            dragEnded(mouse, itemData)
                        })
                    }
                }
            }
        }
    }

}
