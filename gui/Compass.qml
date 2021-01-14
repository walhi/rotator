import QtQuick 2.9
import QtQuick.Window 2.2
import QtCharts 2.0

Rectangle {
    anchors.fill: parent

    property var upperValue: 190
    property var lowerValue: 170

    PolarChartView {
        id: chart
        legend.visible: false

        CategoryAxis {
            id: yAxis
            min: 0
            max: 1
            tickCount: 0
            labelsVisible: false
        }
        ValueAxis {
            id: xAxis
            min: 0
            max: 360
            labelsVisible: false
        }

        margins.top: 0
        margins.bottom: 0
        margins.left: 0
        margins.right: 0

        //title: "Компас"
        anchors.fill: parent

        LineSeries {
            id: mouse_position
            width: 2
            color: "lightgray"
            axisRadial: yAxis
            axisAngular: xAxis
        }
        LineSeries {
            id: lowerLine
            axisRadial: yAxis
            axisAngular: xAxis
            XYPoint { x: lowerValue; y: 0 }
            XYPoint { x: lowerValue; y: 1 }
        }
        LineSeries {
            id: upperLine
            axisRadial: yAxis
            axisAngular: xAxis
            XYPoint { x: upperValue; y: 0 }
            XYPoint { x: upperValue; y: 1 }
        }
        AreaSeries {
            axisRadial: yAxis
            axisAngular: ValueAxis {
                min: 0
                max: 360
                labelsVisible: false
            }
            lowerSeries: lowerLine
            upperSeries: upperLine
            color: "red"
        }
        LineSeries {
            id: ant_position
            width: 5
            axisRadial: yAxis
            axisAngular: ValueAxis {
                min: 0
                max: 360
                tickCount: 9
                labelFormat: "%d"
                labelsVisible: false
            }

            XYPoint { x: 0; y: 0 }
            XYPoint { x: 0; y: 1 }

        }

        antialiasing: true
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            propagateComposedEvents: true
            hoverEnabled: true
            onEntered:{
                text_value.visible = true;
            }
            onExited:{
                text_value.visible = false;
                mouse_position.clear()
            }
            onPositionChanged:{
                var point = chart.mapToValue(Qt.point(mouse.x,mouse.y));

                mouse_position.clear()
                mouse_position.append(0, 0)
                mouse_position.append(point.x, 1)

                text_value.text = point.x.toFixed(0);
            }

            onClicked: {
                var point = chart.mapToValue(Qt.point(mouse.x,mouse.y));

                if (point.x < upperValue && point.x > lowerValue)
                {
                    if (point.x < (lowerValue + (upperValue - lowerValue)/2))
                        point.x = lowerValue;
                    else
                        point.x = upperValue;
                }

                ant_position.clear()
                ant_position.append(0, 0)
                ant_position.append(point.x, 1)

                rotator.go(point.x)
            }
        }
    }

    Rectangle {
        id: stopButton
        x: 526
        y: 409
        width: 64
        height: 64
        color: "#66ff0000"
        radius: 15
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.rightMargin: 0

        MouseArea {
            anchors.fill: parent
            onClicked: {
                rotator.stop()
            }
        }
    }

    Text {
        id: text_value
        text: "EVal"
        font.pointSize: 16
        font.bold: true
        visible: false
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
