import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    id: root
    width: 640
    height: 480
    color: "#414244"

    function colorFromType(reg) {
        if (reg === "camera") return "lightgreen"
        else if (reg === "light") return "lightblue"
        else if (reg === "material") return "lightcoral"
        return "black"
    }

    Column {
        spacing: 4

        GroupView {
            id: cameras
            model: sceneModel.cameras
            width: root.width
            title: "Cameras"
            icon: "resources/images/camera.png"
            delegate: Camera {
                color: colorFromType("camera")
            }
        }

        GroupView {
            id: lights
            model: sceneModel.lights
            width: root.width
            title: "Lights"
            icon: "resources/images/light.png"
            delegate: Light {
                color: colorFromType("light")
            }
        }

        GroupView {
            id: materials
            model: sceneModel.materials
            width: root.width
            title: "Materials"
            icon: "resources/images/material.png"
            delegate: Material {
                color: colorFromType("material")
            }
        }
    }
}
