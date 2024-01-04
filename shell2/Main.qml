import QtQuick
import QtQuick.Controls

Rectangle {
    id: hack
    width: 30
    height: 600
    //color: "#00000000"
    color: "#000000"

    Rectangle {
        //x: hack.width-30
        x: 0
        id: rect
        width: 30
        height: 600
        visible: true
        color: "teal"
        //anchors.left: parent.left

        Button {
            text: "ok"
            width: 30
            height: 30
            anchors.verticalCenter: parent.verticalCenter

            ToolTip.visible: hovered
            ToolTip.delay: 500
            ToolTip.text: qsTr("This is what you see")

            MouseArea {
                id: mousearea
                anchors.fill: parent
                hoverEnabled: true
            }

            states: State {
                name: "extend"; when: mousearea.containsMouse === true
                PropertyChanges {
                    hack {
                        width: 300
                    }
                }
            }

            transitions: Transition {
                from: ""; to: "extend"; reversible: true

                NumberAnimation {
                    property: "width"
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
