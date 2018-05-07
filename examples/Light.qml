import QtQuick 2.9
import QtQml 2.2
import "."

Item {
    id: root
    property string text;
    property color color: "black"
    property int rowHeight: 17
    height: col.children.length*rowHeight
    anchors {
        left: parent.left
        right: parent.right
        rightMargin: 10
        leftMargin: 10
    }

    Component {
        id: pointLight
        Row {
            height: root.rowHeight
            PropertyLabel {
                title: "Position"
                value: position
            }

            PropertyLabel {
                title: "Attenuation coeffs"
                value: attenuationCoeffs
            }
        }
    }

    Component {
        id: directionalLight
        Row {
            height: root.rowHeight
            PropertyLabel {
                title: "Direction"
                value: direction
            }
        }
    }

    Component.onCompleted: {
        if(type == "directional") directionalLight.createObject(col)
        else if(type == "point") pointLight.createObject(col)
    }

    Rectangle {
        id: item
        anchors.fill: parent
        radius: 5
        color: root.color

        Column {
            id: col
            anchors.fill: parent
            Row {
                height: root.rowHeight
                anchors {
                    left: col.left
                    right: col.right
                }
                PropertyLabel {
                    title: "Name"
                    value: name
                }
                PropertyLabel {
                    title: "Type"
                    value: type
                }
            }

            Row {
                height: root.rowHeight
                anchors {
                    left: col.left
                    right: col.right
                }

                PropertyLabel {
                    title: "Ambient"
                    value: ambient
                }

                PropertyLabel {
                    title: "Diffuse"
                    value: diffuse
                }

                PropertyLabel {
                    title: "Specular"
                    value: specular
                }
            }
        }
    }
}
