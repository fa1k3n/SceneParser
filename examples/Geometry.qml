import QtQuick 2.9
import QtQuick.Controls 2.1
import QtQml 2.2
import "."

Item {
    id: root
    property string text;
    property color color: "black"
    property int rowHeight: 17
    height: 4*rowHeight
    anchors {
        left: parent.left
        right: parent.right
        rightMargin: 10
        leftMargin: 10
    }

    Component {
        id: meshGeometry

        Row {
            id: theRow
            height: root.rowHeight + verticeList.height

            Text {
                id: title
                text: "Vertices"
                //anchors.left: theRow.left
            }

            ListView {
                id: verticeList
                model: vertices
                height: 3*rowHeight
                contentHeight: rowHeight * count
                anchors.left: title.right
                leftMargin: 10
              //  clip: true
                ScrollBar.vertical: ScrollBar {}
                delegate:  Text {
                    text: modelData
                }
            }



            PropertyLabel {
                //anchors.top: title.bottom
                //anchors.left: theRow.left
                title: "Tri"
                value: tri
            }
        }
    }

    Component.onCompleted: {
        if(type == "mesh") {
            var obj = meshGeometry.createObject(col)
            root.height = col.children.length*rowHeight + obj.height
        }
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
        }
    }
}
