import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: dockRoot
    radius: 20
    color: dock.bgColor

    width: Screen.width * dock.widthPercent / 100
    height: Screen.height * dock.heightPercent / 100

    Flickable {
        anchors.fill: parent
        contentWidth: row.width
        interactive: contentWidth > width
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        flickDeceleration: 3000

        Row {
            id: row
            spacing: 10
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter

            Repeater {
                model: dock.iconPaths

                delegate: Image {
                    source: modelData
                    fillMode: Image.PreserveAspectFit
                    asynchronous: true
                    cache: true
                    width: parent.height * dock.iconWidthPercent / 100
                    height: parent.height * dock.iconHeightPercent / 100
                    anchors.verticalCenter: parent.verticalCenter
                    smooth: true
                }
            }
        }
    }
}

