import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: Screen.width
    height: Screen.height

    Image {
        id: bgImage
        anchors.fill: parent
        source: "file:///home/deepak/.config/hypr/wallhaven-p2j763_1920x1080.png"
        fillMode: Image.PreserveAspectCrop
        smooth: true
        cache: true
    }

    Launcher {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
    }
}
