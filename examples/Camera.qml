import QtQuick 2.9

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
        height: header.height + secondRow.height
        radius: 5
        color: root.color

        property bool expanded: false

        MouseArea {
            anchors.fill: item
            onPressed: item.expanded = !item.expanded
        }

        Text {
            id: header
            text: name
            anchors.topMargin: 5
            leftPadding: 10
        }

        Column {
            id: secondRow
            visible: item.expanded
            height: header.height * item.expanded
            //height: 40
            anchors.left: header.right

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
        }
    }
}
