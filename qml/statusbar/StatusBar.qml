import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: statusBarRoot
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: statusBar.backgroundColor

        // Time text on left
        Text {
            id: timeText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: statusBarRoot.height * 0.4
            color: statusBar.textColor
            font.pixelSize: Math.floor(statusBarRoot.height * 0.6)
            text: Qt.formatTime(new Date(), statusBar.timeFormat24 ? "HH:mm" : "hh:mm AP")
            font.bold: true
        }

        Timer {
            interval: 1000; running: true; repeat: true
            onTriggered: timeText.text = Qt.formatTime(new Date(), statusBar.timeFormat24 ? "HH:mm" : "hh:mm AP")
        }

        // Battery container on right with icon + percentage text side by side
        Row {
            id: batteryRow
            spacing: statusBarRoot.height * 0.1
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: statusBarRoot.height * 0.4

            Image {
                id: batteryIcon
                source: statusBar.batteryIcon
                width: statusBarRoot.height
                height: statusBarRoot.height * 0.8
                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: batteryPercentText
                text: statusBar.batteryPercentage + "%"
                color: statusBar.textColor
                font.pixelSize: Math.floor(statusBarRoot.height * 0.6)
                verticalAlignment: Text.AlignVCenter
                font.bold: true
            }
        }
    }
}
