import QtQuick 2.9

Item {
    id: root
    property string text;
    property color color: "black"
    property var light
    implicitHeight: item.height
    anchors {
        left: parent.left
        right: parent.right
        rightMargin: 10
        leftMargin: 10
    }
    MouseArea {
        id: dragArea
        property bool held: false

        anchors { left: parent.left; right: parent.right }
        height: item.height

        Rectangle {
            id: item
            width: root.width
            height: 40
            radius: 5
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            color: root.color

            Text {
                id: header
                text: name
                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 10
            }
        }
    }
}
