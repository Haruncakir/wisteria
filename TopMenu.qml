import QtQuick

// Top Menu Bar
Item {
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
}
