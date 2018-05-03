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
        id: basicCam
        Row {
            height: root.rowHeight
            PropertyLabel {
                title: "Field of view"
                value: fov
            }

            PropertyLabel {
                title: "Aspect ratio"
                value: aspectRatio
            }
        }
    }

    Component {
        id: advancedCam
        Row {
            height: root.rowHeight
            PropertyLabel {
                title: "Left"
                value: model.left
            }

            PropertyLabel {
                title: "Right"
                value: model.right
            }

            PropertyLabel {
                title: "Top"
                value: model.top
            }

            PropertyLabel {
                title: "Bottom"
                value: model.bottom
            }
        }
    }

    Component.onCompleted: {
        if(type == "basic") basicCam.createObject(col)
        else if(type == "advanced") advancedCam.createObject(col)
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
                    title: "Eye point"
                    value: eyePoint
                }

                PropertyLabel {
                    title: "Look point"
                    value: lookPoint
                }

                PropertyLabel {
                    title: "Up"
                    value: up
                }

                PropertyLabel {
                    title: "Distance image plane"
                    value: distanceImagePlane
                }
            }
        }
    }
}
