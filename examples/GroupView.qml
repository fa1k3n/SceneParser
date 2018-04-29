import QtQuick 2.9

Item {
    id: root
    property var model
    property string title
    property Component delegate
    property string icon: ""
    property color color: "gray"
    property color textColor: "white"
    property bool expanded: false

    implicitHeight: groupHeader.height + content.height * expanded
    Rectangle {
        id: groupHeader
        width: root.width
        height: 20
        radius: 5
        color: root.color


        MouseArea {
            anchors.fill: parent
            onPressed: root.expanded = !root.expanded

            Image {
                id: iconImg
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                source: root.icon
            }

            Text {
                anchors.left: iconImg.right
                leftPadding: 10
                anchors.verticalCenter: parent.verticalCenter
                text: root.title
                color: textColor
            }

            Image {
                id: arrowImg
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10
                source: root.expanded ? "resources/images/down.png" : "resources/images/left.png"
            }
        }

    }

    ListView {
        id: content
        model: root.model
        height: contentHeight + spacing
        visible: root.expanded
        anchors {
            left: root.left
            right:root.right
            top: groupHeader.bottom
        }

        spacing: 2
        interactive: false
        delegate: root.delegate
    }
}
