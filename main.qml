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
    title: qsTr("Hello World")
    color: "#1e1e2e"

    // State properties for file explorer
    property bool explorerVisible: false
    property string currentFolder: ""

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
            Text {
                text: "File"
                color: "#d9d9d9"
                font.pixelSize: 13
                font.family: "JetBrains Mono Nerd Font"
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
                        root.explorerVisible = !root.explorerVisible
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

        FolderDialog {
            id: folderDialog
            title: "Select a folder"
            onAccepted: {
                console.log("Selected folder: ", selectedFolder)
                root.currentFolder = selectedFolder
            }
            onRejected: {
                console.log("Folder selection canceled.")
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
                SplitView.preferredWidth: root.explorerVisible ? 250 : 0
                SplitView.minimumWidth: 0
                SplitView.maximumWidth: root.width * 0.3
                visible: root.explorerVisible
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
                            visible: root.currentFolder === ""

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
                            visible: root.currentFolder !== ""
                            clip: true

                            model: FolderListModel {
                                id: folderModel
                                folder: root.currentFolder !== "" ? root.currentFolder : ""
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
                                            root.currentFolder = fileURL.toString().slice(7) // Remove "file://"
                                        } else {
                                            // TODO: Open file (would implement file opening logic here)
                                            console.log("Opening file:", fileName)
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

                Text {
                    anchors.centerIn: parent
                    text: "Main Content Area"
                    color: "#ffffff"
                    font.pixelSize: 16
                }
            }
        }
    }
}
