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

    property var bottomRowIcons: launcher.allIcons.slice(0, columns)

    property var gridIcons: launcher.allIcons.slice(columns)
    property var gridAppNames: launcher.allAppNames.slice(columns)

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
                model: gridIcons.length

                delegate: Item {
                    width: iconSize
                    height: iconSize + spacing + (iconSize * 0.25)

                    Column {
                        spacing: spacing * 0.5
                        anchors.centerIn: parent
                        width: parent.width

                        Image {
                            source: gridIcons[index]
                            width: iconSize
                            height: iconSize
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            cache: true
                        }

                        Text {
                            text: gridAppNames[index]
                            font.pixelSize: iconSize * 0.15
                            color: "white"
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            elide: Text.ElideRight
                            wrapMode: Text.WordWrap
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            launcher.launchApp(
                                gridIcons[index].split("/").pop().split("_")[0]
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
                model: bottomRowIcons.length

                delegate: Item {
                    width: iconSize
                    height: iconSize

                    Image {
                        source: bottomRowIcons[index]
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        cache: true
                        smooth: true
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            launcher.launchApp(bottomRowIcons[index].split("/").pop().split("_")[0])
                        }
                    }
                }
            }
        }
    }
}
