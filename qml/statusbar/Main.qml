import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: Screen.width
    height: Screen.height * (statusBar.heightPercent / 100)

    StatusBar {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
    }
}
