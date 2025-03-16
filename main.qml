import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.folderlistmodel

Window {
    id: root
    width: Screen.width * 0.8
    height: Screen.height * 0.8
    visible: true
    title: qsTr("Wisteria")
    color: "#1e1e2e"

    // Top Menu Bar
    Rectangle {
        id: menuBar
        width: parent.width
        height: parent.height * 0.03
        color: "#2d2d3a"
        Row {
            spacing: 15
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10

            Image {
                source: "qrc:/UI/Assets/icon.png"
                width: menuBar.height - 5
                height: menuBar.height - 5
            }

            // Menu items
            Button {
                text: "File"
                flat: true
                contentItem: Text {
                    text: parent.text
                    color: "#d9d9d9"
                    font.pixelSize: 13
                    font.family: "JetBrains Mono Nerd Font"
                }
                onClicked: fileMenu.open()

                Menu {
                    id: fileMenu

                    MenuItem {
                        text: "New File"
                        onTriggered: newFileDialog.open()
                    }

                    MenuItem {
                        text: "Open Folder"
                        onTriggered: folderDialog.open()
                    }

                    MenuItem {
                        text: "Save"
                        enabled: fileManager.activeFileIndex >= 0
                        onTriggered: fileManager.saveFile(fileManager.activeFileIndex)
                    }

                    MenuItem {
                        text: "Save As"
                        enabled: fileManager.activeFileIndex >= 0
                        onTriggered: saveAsDialog.open()
                    }

                    MenuItem {
                        text: "Close File"
                        enabled: fileManager.activeFileIndex >= 0
                        onTriggered: fileManager.closeFile(fileManager.activeFileIndex)
                    }
                }
            }

            Text {
                text: "Edit"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
            }
            Text {
                text: "Selection"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
            }
            Text {
                text: "View"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
            }
            Text {
                text: "Go"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
            }
            Text {
                text: "Run"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
            }
            Text {
                text: "Terminal"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
            }
            Text {
                text: "Help"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
            }
        }
    }

    // Content area (below menu bar)
    Item {
        id: contentArea
        anchors.top: menuBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // Left sidebar panel
        Rectangle {
            id: sidebarPanel
            color: "#2f2f3f"
            width: 50
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Column {
                id: sidebarPanelColumn
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 50
                spacing: 10
                topPadding: 2

                Button {
                    id: folderButton
                    width: sidebarPanelColumn.width - 10
                    height: sidebarPanelColumn.width - 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    flat: true
                    Image {
                        source: "qrc:/UI/Assets/folder.svg"
                        width: sidebarPanelColumn.width * 0.6
                        height: sidebarPanelColumn.width * 0.6
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    onClicked: {
                        fileManager.explorerVisible = !fileManager.explorerVisible
                    }
                }

                Button {
                    width: sidebarPanelColumn.width - 10
                    height: sidebarPanelColumn.width - 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    flat: true
                    Image {
                        source: "qrc:/UI/Assets/git.svg"
                        width: sidebarPanelColumn.width * 0.6
                        height: sidebarPanelColumn.width * 0.6
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Button {
                    width: sidebarPanelColumn.width - 10
                    height: sidebarPanelColumn.width - 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    flat: true
                    Image {
                        source: "qrc:/UI/Assets/search.svg"
                        width: sidebarPanelColumn.width * 0.6
                        height: sidebarPanelColumn.width * 0.6
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Button {
                    width: sidebarPanelColumn.width - 10
                    height: sidebarPanelColumn.width - 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    flat: true
                    Image {
                        source: "qrc:/UI/Assets/extensions.svg"
                        width: sidebarPanelColumn.width * 0.6
                        height: sidebarPanelColumn.width * 0.6
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Button {
                    id: settingsButton
                    width: sidebarPanelColumn.width - 10
                    height: sidebarPanelColumn.width - 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    flat: true
                    Image {
                        source: "qrc:/UI/Assets/settings.svg"
                        width: sidebarPanelColumn.width * 0.6
                        height: sidebarPanelColumn.width * 0.6
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }

        // Dialogs
        FolderDialog {
            id: folderDialog
            title: "Select a folder"
            onAccepted: {
                console.log("Selected folder: ", selectedFolder)
                fileManager.currentFolder = selectedFolder
            }
            onRejected: {
                console.log("Folder selection canceled.")
            }
        }

        FileDialog {
            id: saveAsDialog
            title: "Save As"
            fileMode: FileDialog.SaveFile
            onAccepted: {
                fileManager.saveFileAs(fileManager.activeFileIndex, selectedFile)
            }
        }

        Dialog {
            id: newFileDialog
            title: "New File"
            standardButtons: Dialog.Ok | Dialog.Cancel

            ColumnLayout {
                TextField {
                    id: newFileNameField
                    placeholderText: "Enter file name"
                    Layout.fillWidth: true
                }
            }

            onAccepted: {
                if (newFileNameField.text.trim() !== "") {
                    fileManager.createNewFile(newFileNameField.text)
                }
            }
        }

        // Main area (to the right of sidebar)
        SplitView {
            id: mainSplitView
            anchors.left: sidebarPanel.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            orientation: Qt.Horizontal

            // File Explorer Panel
            Rectangle {
                id: fileExplorerPanel
                SplitView.preferredWidth: fileManager.explorerVisible ? 250 : 0
                SplitView.minimumWidth: 0
                SplitView.maximumWidth: root.width * 0.3
                visible: fileManager.explorerVisible
                color: "#252535"

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    // Explorer header
                    Rectangle {
                        Layout.fillWidth: true
                        height: 30
                        color: "transparent"

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: "EXPLORER"
                            color: "#cccccc"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }

                    // Folder display or Open Folder button
                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        // Show when no folder is opened
                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            visible: fileManager.currentFolder === ""

                            Column {
                                anchors.centerIn: parent
                                spacing: 10

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "No folder opened"
                                    color: "#cccccc"
                                }

                                Button {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Open Folder"
                                    onClicked: {
                                        folderDialog.open()
                                    }
                                }
                            }
                        }

                        // Show when a folder is opened
                        ListView {
                            id: fileListView
                            anchors.fill: parent
                            visible: fileManager.currentFolder !== ""
                            clip: true

                            model: FolderListModel {
                                id: folderModel
                                folder: fileManager.currentFolder !== "" ? fileManager.currentFolder : ""
                                showDirs: true
                                showDotAndDotDot: false
                                sortField: FolderListModel.Name
                            }

                            delegate: Rectangle {
                                width: fileListView.width
                                height: 24
                                color: "transparent"

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 10
                                    spacing: 5

                                    Image {
                                        Layout.preferredWidth: 16
                                        Layout.preferredHeight: 16
                                        source: fileIsDir ? "qrc:/UI/Assets/folder.svg" : "qrc:/UI/Assets/file.svg"
                                    }

                                    Text {
                                        text: fileName
                                        color: "#ffffff"
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (fileIsDir) {
                                            // Navigate to subfolder
                                            fileManager.currentFolder = fileURL.toString().slice(7) // Remove "file://"
                                        } else {
                                            // Open File
                                            var fullPath = fileManager.currentFolder + "/" + fileName
                                            fileManager.openFile(fullPath.toString().slice(8))
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Main content area
            Rectangle {
                id: mainContentArea
                SplitView.fillWidth: true
                SplitView.minimumWidth: 100
                color: "#1e1e2e"

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Tab Bar for opened files
                    Rectangle {
                        id: tabBar
                        Layout.fillWidth: true
                        height: 36
                        color: "#252535"
                        visible: fileManager.openFiles.length > 0

                        ScrollView {
                            anchors.fill: parent
                            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                            ScrollBar.vertical.policy: ScrollBar.AlwaysOff
                            clip: true

                            Row {
                                id: tabsRow
                                height: parent.height
                                spacing: 0

                                // Generate tabs for each open file
                                Repeater {
                                    model: fileManager.openFiles

                                    Rectangle {
                                        id: tabRect
                                        width: tabText.width + 50
                                        height: tabBar.height
                                        color: index === fileManager.activeFileIndex ? "#1e1e2e" : "#252535"
                                        border.width: 0

                                        RowLayout {
                                            anchors.fill: parent
                                            anchors.leftMargin: 10
                                            anchors.rightMargin: 10
                                            spacing: 5

                                            Image {
                                                Layout.preferredWidth: 16
                                                Layout.preferredHeight: 16
                                                source: "qrc:/UI/Assets/file.svg"
                                            }

                                            Text {
                                                id: tabText
                                                text: modelData
                                                color: "#ffffff"
                                                elide: Text.ElideRight
                                                Layout.fillWidth: true
                                            }

                                            // Unsaved indicator
                                            Rectangle {
                                                visible: fileManager.isFileDirty(index)
                                                width: 8
                                                height: 8
                                                radius: 4
                                                color: "#55aaff"
                                            }

                                            Button {
                                                Layout.preferredWidth: 20
                                                Layout.preferredHeight: 20
                                                flat: true
                                                text: "×"
                                                font.pixelSize: 16
                                                onClicked: {
                                                    // Close this tab using FileManager
                                                    fileManager.closeFile(index)
                                                }
                                            }
                                        }

                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                fileManager.activeFileIndex = index
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // File content area
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#1e1e2e"

                        // Welcome screen when no files are open
                        Item {
                            anchors.fill: parent
                            visible: fileManager.openFiles.length === 0

                            Column {
                                anchors.centerIn: parent
                                spacing: 20

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Welcome to Wisteria"
                                    color: "#ffffff"
                                    font.pixelSize: 24
                                    font.bold: true
                                    visible: fileManager.currentFolder === ""
                                }

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Open a folder to get started"
                                    color: "#cccccc"
                                    font.pixelSize: 16
                                    visible: fileManager.currentFolder === ""
                                }

                                Button {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Open Folder"
                                    visible: fileManager.currentFolder === ""
                                    onClicked: {
                                        folderDialog.open()
                                    }
                                }
                            }
                        }

                        // Stack of editors for open files
                        StackLayout {
                            anchors.fill: parent
                            currentIndex: fileManager.activeFileIndex
                            visible: fileManager.openFiles.length > 0

                            Repeater {
                                model: fileManager.openFiles

                                TextArea {
                                    text: fileManager.getFileContent(index)
                                    color: "#ffffff"
                                    font.family: "JetBrains Mono Nerd Font"
                                    font.pixelSize: 14
                                    wrapMode: TextEdit.NoWrap
                                    background: Rectangle {
                                        color: "#1e1e2e"
                                    }

                                    // Line numbers
                                    property int lineNumbersWidth: 40
                                    leftPadding: lineNumbersWidth + 10

                                    // Line numbers background
                                    Rectangle {
                                        width: parent.lineNumbersWidth
                                        height: parent.height
                                        color: "#252535"
                                        anchors.left: parent.left
                                    }

                                    // Monitor content changes
                                    onTextChanged: {
                                        if (index === fileManager.activeFileIndex) {
                                            fileManager.setFileContent(index, text)
                                        }
                                    }

                                    // Keyboard shortcuts
                                    Keys.onPressed: function(event) {
                                        // Ctrl+S for save
                                        if ((event.modifiers & Qt.ControlModifier) && event.key === Qt.Key_S) {
                                            fileManager.saveFile(fileManager.activeFileIndex)
                                            event.accepted = true
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Error notification
    Connections {
        target: fileManager
        function onErrorOccurred(error) {
            errorDialog.text = error
            errorDialog.open()
        }
    }

    Dialog {
        id: errorDialog
        title: "Error"
        property string text: ""

        Label {
            text: errorDialog.text
            wrapMode: Text.Wrap
        }

        standardButtons: Dialog.Ok
    }
}
