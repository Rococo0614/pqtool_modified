import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Item {
    id: switchItem

    Column {
        Text {
            text: qsTr("Auto write")
        }
        Switch {
            objectName: "autoWriteSwitch"
            visible: true
            signal valueChanged(int position)

            onPositionChanged: {
                valueChanged(position)
            }
        }
    }
}
