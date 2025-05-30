import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: statusBarRoot
    anchors.fill: parent
    Rectangle {
        anchors.fill: parent
        color: statusBar.backgroundColor

        Text {
            id: timeText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            color: statusBar.textColor
            font.pixelSize: Math.floor(statusBarRoot.height * 0.6)
            text: Qt.formatTime(new Date(), statusBar.timeFormat24 ? "HH:mm" : "hh:mm AP")
        }

        Timer {
            interval: 1000; running: true; repeat: true
            onTriggered: timeText.text = Qt.formatTime(new Date(), statusBar.timeFormat24 ? "HH:mm" : "hh:mm AP")
        }
    }
}
