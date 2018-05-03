import QtQuick 2.9
import QtQml 2.2

Item {
    id: root
    property string text;
    property color color: "black"
    height: item.height
    anchors {
        left: parent.left
        right: parent.right
        rightMargin: 10
        leftMargin: 10
    }

    Rectangle {
        id: item
        width: root.width
        height: 60
        radius: 5
        color: root.color

        Column {
            id: col
            anchors.fill: parent
            Row {
                height: 15
                anchors {
                    left: col.left
                    right: col.right
                }
                Text {
                    text: name
                    leftPadding: 10
                }
                Text {
                    text: type
                    leftPadding: 10
                }
            }

            Row {
                height: 15
                anchors {
                    left: col.left
                    right: col.right
                }

                Vector3d {
                    title: "Eye point"
                    value: eyePoint
                }

                Vector3d {
                    title: "Look point"
                    value: lookPoint
                }

                Vector3d {
                    title: "Up"
                    value: up
                }

                Vector3d {
                    title: "Distance image plane"
                    value: distanceImagePlane
                }
            }

            Row {
                height: 15
                anchors {
                    left: col.left
                    right: col.right
                }
                Vector3d {
                                        title: "Field of view"
                                        value: fov
                                    }
                Instantiator {
                    model: 1

                    delegate: Vector3d {
                        title: "Field of view"
                        value: fov
                    }
                }
            }
        }
    }
}
