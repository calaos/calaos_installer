import QtQuick
import QtQuick.Controls

Rectangle {
    id: pageSelector

    property var pages: [{ name: "Page 1", type: "Default" }]
    property int currentPageIndex: 0

    signal pageAdded(int index)
    signal pageSelected(int index)
    signal pageRenamed(int index, string newName)
    signal pageDeleted(int index)
    signal pageDuplicated(int index)
    signal pageTypeChanged(int index, string newType)

    width: parent.width
    height: 40
    color: "#e0e0e0"
    border.color: "#ccc"
    border.width: 1
    radius: 4

    ScrollView {
        anchors.fill: parent
        anchors.margins: 5
        contentWidth: pageRow.width

        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        Row {
            id: pageRow
            spacing: 5
            height: parent.height

            Repeater {
                model: pages

                Rectangle {
                    id: pageButton
                    width: Math.max(80, pageText.width + 20)
                    height: 30
                    color: index === currentPageIndex ? "#4CAF50" : "#ffffff"
                    border.color: "#999"
                    border.width: 1
                    radius: 3

                    property int pageIndex: index

                    Text {
                        id: pageText
                        anchors.centerIn: parent
                        text: modelData.name
                        color: index === currentPageIndex ? "white" : "black"
                        font.pixelSize: 11
                    }

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton

                        onClicked: function(mouse) {
                            if (mouse.button === Qt.LeftButton) {
                                currentPageIndex = pageIndex
                                pageSelected(pageIndex)
                            } else if (mouse.button === Qt.RightButton) {
                                contextMenu.pageIndex = pageIndex
                                contextMenu.popup()
                            }
                        }
                    }
                }
            }

            // Add page button
            Button {
                width: 30
                height: 30
                text: "+"
                font.pixelSize: 16
                font.bold: true

                background: Rectangle {
                    color: parent.pressed ? "#e0e0e0" : parent.hovered ? "#f5f5f5" : "#f0f0f0"
                    border.color: "#999"
                    border.width: 1
                    radius: 3
                }

                onClicked: {
                    addPage()
                }
            }
        }
    }

    Menu {
        id: contextMenu
        property int pageIndex: 0

        MenuItem {
            text: "Rename"
            onTriggered: {
                renameDialog.pageIndex = contextMenu.pageIndex
                renameDialog.currentName = pages[contextMenu.pageIndex].name
                renameDialog.open()
            }
        }

        MenuSeparator {}

        MenuItem {
            text: "Duplicate"
            onTriggered: {
                duplicatePage(contextMenu.pageIndex)
            }
        }

        MenuItem {
            text: "Change Type"
            onTriggered: {
                typeDialog.pageIndex = contextMenu.pageIndex
                typeDialog.currentType = pages[contextMenu.pageIndex].type
                typeDialog.open()
            }
        }

        MenuSeparator {}

        MenuItem {
            text: "Delete"
            enabled: pages.length > 1
            onTriggered: {
                deleteDialog.pageIndex = contextMenu.pageIndex
                deleteDialog.pageName = pages[contextMenu.pageIndex].name
                deleteDialog.open()
            }
        }
    }

    Dialog {
        id: renameDialog
        title: "Rename Page"
        modal: true
        anchors.centerIn: parent

        property int pageIndex: 0
        property string currentName: ""

        TextField {
            id: nameField
            placeholderText: "Enter page name"
            text: renameDialog.currentName
            selectByMouse: true

            onAccepted: {
                if (text.trim() !== "") {
                    renamePage(renameDialog.pageIndex, text.trim())
                    renameDialog.close()
                }
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            if (nameField.text.trim() !== "") {
                renamePage(renameDialog.pageIndex, nameField.text.trim())
            }
        }

        onOpened: {
            nameField.selectAll()
            nameField.forceActiveFocus()
        }
    }

    Dialog {
        id: typeDialog
        title: "Change Page Type"
        modal: true
        anchors.centerIn: parent

        property int pageIndex: 0
        property string currentType: ""

        Column {
            spacing: 10

            ButtonGroup {
                id: typeGroup
            }

            RadioButton {
                text: "default"
                checked: true
                ButtonGroup.group: typeGroup
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            changePageType(typeDialog.pageIndex, "default")
        }
    }

    Dialog {
        id: deleteDialog
        title: "Delete Page"
        modal: true
        anchors.centerIn: parent

        property int pageIndex: 0
        property string pageName: ""

        Label {
            text: "Are you sure you want to delete the page \"" + deleteDialog.pageName + "\"?\n\nThis action cannot be undone."
            wrapMode: Text.WordWrap
            width: 300
        }

        standardButtons: Dialog.Yes | Dialog.No

        onAccepted: {
            deletePage(deleteDialog.pageIndex)
        }
    }

    function addPage() {
        // Just emit signal - the actual page creation is handled by the model in main.qml
        // The pages list will be updated from the model after creation
        pageAdded(pages.length)  // Pass the expected index of the new page
        console.log("Requested to add new page")
    }

    function renamePage(index, newName) {
        if (index >= 0 && index < pages.length) {
            // Just emit signal - the actual rename is handled by the model in main.qml
            pageRenamed(index, newName)
            console.log("Requested to rename page", index, "to:", newName)
        }
    }

    function deletePage(index) {
        if (index >= 0 && index < pages.length && pages.length > 1) {
            // Just emit signal - the actual deletion is handled by the model in main.qml
            pageDeleted(index)
            console.log("Requested to delete page at index:", index)
        }
    }

    function duplicatePage(index) {
        if (index >= 0 && index < pages.length) {
            // Just emit signal - the actual duplication is handled by the model in main.qml
            pageDuplicated(index)
            console.log("Requested to duplicate page:", pages[index].name)
        }
    }

    function changePageType(index, newType) {
        if (index >= 0 && index < pages.length) {
            // Just emit signal - the actual type change is handled by the model in main.qml
            pageTypeChanged(index, newType)
            console.log("Requested to change page", index, "type to:", newType)
        }
    }
}