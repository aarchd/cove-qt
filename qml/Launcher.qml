import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: launcherRoot
    anchors.fill: parent

    property int iconSize: launcher.iconSize
    property int spacing: launcher.spacing

    property int columns: Math.floor(width / (iconSize + spacing))
    property int rows: Math.floor((height - bottomRow.height - spacing) / (iconSize + spacing))

    Flickable {
        id: flickable
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomRow.top
        clip: true
        interactive: true
        contentWidth: width
        contentHeight: gridContent.implicitHeight
        flickableDirection: Flickable.VerticalFlick

        Grid {
            id: gridContent
            width: flickable.width
            columns: launcherRoot.columns
            spacing: launcherRoot.spacing

            Repeater {
                model: launcher.gridIcons.length

                delegate: Item {
                    width: iconSize
                    height: iconSize

                    Image {
                        source: launcher.gridIcons[index]
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        cache: true
                    }

                    Text {
                        text: launcher.gridAppNames[index]
                        font.pixelSize: 12
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        elide: Text.ElideRight
                        width: iconSize
                    }


                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            launcher.launchApp(
                                launcher.gridIcons[index].split("/").pop().split("_")[0]
                            )
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: bottomRow
        width: parent.width
        height: iconSize + 20
        anchors.bottom: parent.bottom
        color: "#22000000"
        radius: 12

        Row {
            id: bottomRowIcons
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Repeater {
                model: launcher.bottomRowIcons.length

                delegate: Item {
                    width: iconSize
                    height: iconSize

                    Image {
                        source: launcher.bottomRowIcons[index]
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        cache: true
                        smooth: true
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            launcher.launchApp(launcher.bottomRowIcons[index].split("/").pop().split("_")[0])
                        }
                    }
                }
            }
        }
    }
}
