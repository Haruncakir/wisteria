import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Dialog {
    id: themeSettingsDialog
    title: "Theme Settings"
    width: 600
    height: 500
    modal: true

    // Center in parent
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    // Theme styling
    background: Rectangle {
        color: theme.backgroundColor
        border.color: theme.sidebarColor
        border.width: 1
    }

    // Main content
    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Theme selection
        GroupBox {
            title: "Theme Selection"
            Layout.fillWidth: true

            background: Rectangle {
                color: "transparent"
                border.color: theme.sidebarColor
                border.width: 1
            }

            label: Label {
                text: parent.title
                color: theme.textColor
                font.bold: true
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "Current Theme:"
                        color: theme.textColor
                    }

                    ComboBox {
                        id: themeSelector
                        Layout.fillWidth: true
                        model: theme.availableThemes
                        currentIndex: model.indexOf(theme.themeName)

                        contentItem: Text {
                            text: themeSelector.displayText
                            color: theme.textColor
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 10
                        }

                        background: Rectangle {
                            color: themeSelector.pressed ? theme.sidebarColor : theme.explorerColor
                            border.color: theme.sidebarColor
                            border.width: 1
                            radius: 2
                        }

                        popup.background: Rectangle {
                            color: theme.backgroundColor
                            border.color: theme.sidebarColor
                            border.width: 1
                        }

                        delegate: ItemDelegate {
                            width: themeSelector.width
                            contentItem: Text {
                                text: modelData
                                color: theme.textColor
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            highlighted: themeSelector.highlightedIndex === index
                            background: Rectangle {
                                color: highlighted ? theme.sidebarColor : "transparent"
                            }
                        }

                        onActivated: {
                            theme.loadTheme(model[currentIndex])
                        }
                    }

                    Button {
                        text: "New"
                        contentItem: Text {
                            text: parent.text
                            color: theme.textColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                            border.width: 1
                            border.color: theme.sidebarColor
                            radius: 4
                        }
                        onClicked: newThemeDialog.open()
                    }

                    Button {
                        text: "Delete"
                        enabled: theme.themeName !== "Default" && theme.themeName !== "Light" && theme.themeName !== "Dracula"
                        contentItem: Text {
                            text: parent.text
                            color: parent.enabled ? theme.textColor : theme.lineNumberColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: parent.enabled && parent.hovered ? theme.sidebarColor : theme.explorerColor
                            border.width: 1
                            border.color: theme.sidebarColor
                            radius: 4
                        }
                        onClicked: deleteThemeConfirmation.open()
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    Button {
                        text: "Import Theme"
                        contentItem: Text {
                            text: parent.text
                            color: theme.textColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                            border.width: 1
                            border.color: theme.sidebarColor
                            radius: 4
                        }
                        onClicked: importThemeDialog.open()
                    }

                    Button {
                        text: "Export Theme"
                        contentItem: Text {
                            text: parent.text
                            color: theme.textColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                            border.width: 1
                            border.color: theme.sidebarColor
                            radius: 4
                        }
                        onClicked: exportThemeDialog.open()
                    }

                    Button {
                        text: "Reset to Default"
                        contentItem: Text {
                            text: parent.text
                            color: theme.textColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                            border.width: 1
                            border.color: theme.sidebarColor
                            radius: 4
                        }
                        onClicked: theme.resetToDefault()
                    }
                }
            }
        }

        // Theme Preview
        GroupBox {
            title: "Theme Preview"
            Layout.fillWidth: true
            Layout.fillHeight: true

            background: Rectangle {
                color: "transparent"
                border.color: theme.sidebarColor
                border.width: 1
            }

            label: Label {
                text: parent.title
                color: theme.textColor
                font.bold: true
            }

            Rectangle {
                id: previewArea
                anchors.fill: parent
                color: theme.backgroundColor

                Rectangle {
                    id: previewMenuBar
                    height: 30
                    width: parent.width
                    color: theme.menuBarColor

                    Row {
                        spacing: 10
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 10

                        Text {
                            text: "File"
                            color: theme.menuTextColor
                        }

                        Text {
                            text: "Edit"
                            color: theme.menuTextColor
                        }

                        Text {
                            text: "View"
                            color: theme.menuTextColor
                        }
                    }
                }

                Rectangle {
                    id: previewSidebar
                    width: 40
                    anchors.top: previewMenuBar.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    color: theme.sidebarColor
                }

                Rectangle {
                    id: previewExplorer
                    width: 150
                    anchors.top: previewMenuBar.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: previewSidebar.right
                    color: theme.explorerColor

                    Text {
                        text: "EXPLORER"
                        color: theme.menuTextColor
                        font.pixelSize: 10
                        font.bold: true
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.margins: 8
                    }
                }

                Rectangle {
                    id: previewEditor
                    anchors.top: previewMenuBar.bottom
                    anchors.left: previewExplorer.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    color: theme.backgroundColor

                    Rectangle {
                        id: previewTabBar
                        height: 30
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        color: theme.tabBarColor

                        Row {
                            anchors.left: parent.left
                            height: parent.height

                            Rectangle {
                                width: 100
                                height: parent.height
                                color: theme.activeTabColor

                                Text {
                                    text: "main.qml"
                                    color: theme.textColor
                                    anchors.centerIn: parent
                                }
                            }

                            Rectangle {
                                width: 100
                                height: parent.height
                                color: theme.tabBarColor

                                Text {
                                    text: "app.js"
                                    color: theme.textColor
                                    anchors.centerIn: parent
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: previewContent
                        anchors.top: previewTabBar.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        color: theme.backgroundColor

                        Rectangle {
                            width: 40
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            color: theme.explorerColor

                            Column {
                                anchors.fill: parent
                                anchors.topMargin: 5

                                Repeater {
                                    model: 10

                                    Text {
                                        text: index + 1
                                        width: parent.width
                                        horizontalAlignment: Text.AlignRight
                                        rightPadding: 5
                                        color: theme.lineNumberColor
                                    }
                                }
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "Sample code content"
                            color: theme.textColor
                        }
                    }
                }
            }
        }

        // Color Pickers
        TabBar {
            id: colorTabBar
            Layout.fillWidth: true

            background: Rectangle {
                color: theme.tabBarColor
            }

            TabButton {
                text: "UI Colors"
                contentItem: Text {
                    text: parent.text
                    color: parent.checked ? theme.textColor : theme.menuTextColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.checked ? theme.activeTabColor : theme.tabBarColor
                }
            }

            TabButton {
                text: "Text Colors"
                contentItem: Text {
                    text: parent.text
                    color: parent.checked ? theme.textColor : theme.menuTextColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.checked ? theme.activeTabColor : theme.tabBarColor
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            currentIndex: colorTabBar.currentIndex

            // UI Colors Tab
            GridLayout {
                columns: 3
                rowSpacing: 10
                columnSpacing: 10

                Label { text: "Background:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.backgroundColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "backgroundColor"
                            colorDialog.color = theme.backgroundColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "backgroundColor"
                        colorDialog.color = theme.backgroundColor
                        colorDialog.open()
                    }
                }

                Label { text: "Sidebar:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.sidebarColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "sidebarColor"
                            colorDialog.color = theme.sidebarColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "sidebarColor"
                        colorDialog.color = theme.sidebarColor
                        colorDialog.open()
                    }
                }

                Label { text: "Explorer:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.explorerColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "explorerColor"
                            colorDialog.color = theme.explorerColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "explorerColor"
                        colorDialog.color = theme.explorerColor
                        colorDialog.open()
                    }
                }

                Label { text: "Tab Bar:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.tabBarColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "tabBarColor"
                            colorDialog.color = theme.tabBarColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "tabBarColor"
                        colorDialog.color = theme.tabBarColor
                        colorDialog.open()
                    }
                }

                Label { text: "Menu Bar:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.menuBarColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "menuBarColor"
                            colorDialog.color = theme.menuBarColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "menuBarColor"
                        colorDialog.color = theme.menuBarColor
                        colorDialog.open()
                    }
                }

                Label { text: "Active Tab:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.activeTabColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "activeTabColor"
                            colorDialog.color = theme.activeTabColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "activeTabColor"
                        colorDialog.color = theme.activeTabColor
                        colorDialog.open()
                    }
                }
            }

            // Text Colors Tab
            GridLayout {
                columns: 3
                rowSpacing: 10
                columnSpacing: 10

                Label { text: "Text:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.textColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "textColor"
                            colorDialog.color = theme.textColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "textColor"
                        colorDialog.color = theme.textColor
                        colorDialog.open()
                    }
                }

                Label { text: "Menu Text:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.menuTextColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "menuTextColor"
                            colorDialog.color = theme.menuTextColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "menuTextColor"
                        colorDialog.color = theme.menuTextColor
                        colorDialog.open()
                    }
                }

                Label { text: "Line Numbers:"; color: theme.textColor }
                Rectangle {
                    width: 100
                    height: 20
                    color: theme.lineNumberColor
                    border.width: 1
                    border.color: theme.sidebarColor

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.targetProperty = "lineNumberColor"
                            colorDialog.color = theme.lineNumberColor
                            colorDialog.open()
                        }
                    }
                }
                Button {
                    text: "Pick"
                    contentItem: Text {
                        text: parent.text
                        color: theme.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                        border.width: 1
                        border.color: theme.sidebarColor
                        radius: 4
                    }
                    onClicked: {
                        colorDialog.targetProperty = "lineNumberColor"
                        colorDialog.color = theme.lineNumberColor
                        colorDialog.open()
                    }
                }
            }
        }
    }

    // Dialog buttons
    footer: DialogButtonBox {
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

        background: Rectangle {
            color: theme.backgroundColor
        }

        onAccepted: {
            // Save the current theme with its current name
            if (theme.themeName !== "Default" && theme.themeName !== "Light" && theme.themeName !== "Dracula") {
                theme.saveCurrentTheme(theme.themeName)
            }
            themeSettingsDialog.accept()
        }
        onRejected: themeSettingsDialog.reject()

        delegate: Button {
            contentItem: Text {
                text: parent.text
                color: theme.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                implicitWidth: 80
                implicitHeight: 30
                color: parent.hovered ? theme.sidebarColor : theme.explorerColor
                border.width: 1
                border.color: theme.sidebarColor
                radius: 4
            }
        }
    }
}
