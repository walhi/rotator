import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    GridLayout {
        id: grid
        columns: 1
        rows: 1
        anchors.fill: parent

        Compass {
            id: compass
        }

    }
}
