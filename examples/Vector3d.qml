import QtQuick 2.9

Item {
    property string title
    property var value
    implicitWidth: theText.implicitWidth
    implicitHeight: theText.implicitHeight
    Text {
        id: theText
        text: title + " " + value
        leftPadding: 10
    }
}
