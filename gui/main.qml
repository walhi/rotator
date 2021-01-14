import QtQuick 2.9
import QtQuick.Window 2.2
import QtPositioning 5.5
import QtLocation 5.6
import QtCharts 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


    Component.onCompleted: {
        console.log("load")
    }

    GridLayout {
        id: grid
        columns: 2
        rows: 2
        anchors.fill: parent

        Rectangle {
            id: map_rectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.rowSpan: 1
            Layout.row: 1
            Layout.column: 0
            //width: 200
            //height: 200
            color: "#ffffff"

            //! [Initialize Plugin]
            Plugin {
                id: myPlugin
                name: "osm"
            }
            //! [Places MapItemView]
            Map {
                id: map
                anchors.fill: parent
                plugin: myPlugin;
                zoomLevel: 1
                //activeMapType: MapType.CustomMap
                center {
                    // The Qt Company in Oslo
                    latitude: 56.722339873731876
                    longitude: 38.79044010242691
                }
                Component.onCompleted: {
                    //map.clearData()
                }
                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                    onDoubleClicked: {
                        //
                    }
                }

            }
            //! [Places MapItemView]


        }

        Rectangle {
            id: compass_rectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.row: 0
            Layout.column: 1
            color: "#ffffff"
            PolarChartView {
                id: chart
                legend.visible: false
                //title: "Компас"
                anchors.fill: parent
                LineSeries {
                    id: compass_series
                    width: 5
                    axisRadial: CategoryAxis {
                        min: 0
                        max: 1
                    }
                    axisAngular: ValueAxis {
                        min: 0
                        max: 360
                        tickCount: 9
                        labelFormat: "%d"
                    }

                    XYPoint { x: 0; y: 0 }
                    XYPoint { x: 0; y: 1 }

                }
                antialiasing: true

                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                    onDoubleClicked: {
                        console.log(mouse.x)
                        console.log(mouse.y)
                        var point = chart.mapToValue(Qt.point(mouse.x,mouse.y));
                        console.log(point)
                    }
                }
            }
        }



    }
}
