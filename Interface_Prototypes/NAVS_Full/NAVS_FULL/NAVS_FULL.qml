import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Window {
    visible: true
    width: 800
    height: 500
    color: "#e9e9e9"
    title: qsTr("Collapsable panel")

    Rectangle {
        anchors.fill: parent
        color: "#ffffff"
    }

    GridLayout {
        id : grid
        rowSpacing: 0
        columnSpacing: 0
        anchors.fill: parent
        rows    : 1
        columns : 2

        Rectangle {
            id: colsx
            color: "#ffffff"
            border.width: 1
            border.color: "#c2c1cc"
            Layout.rowSpan   : 1
            Layout.columnSpan: 1
            Layout.preferredWidth  : 200
            Layout.fillHeight: true

            Behavior on Layout.preferredWidth  {
                NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
            }

            Rectangle {
                id: colsxInner
                x: 0
                y: 0
                width: 200
                Layout.preferredWidth: 200
                height: 41
                color: "#ffffff"
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                border.width: 1
                border.color: "#c2c1cc"

                Text {
                    id: element
                    x: 8
                    y: 15
                    color: "#cdcddc"
                    text: qsTr("NAVS")
                    font.pixelSize: 12
                }

                Button {
                    id: button
                    x: 80
                    y: 111
                    width: 112
                    height: 38
                    text: "hello"
                    onClicked:  {
                        colsx.Layout.preferredWidth  = 50
                        colsxInner.width  = 50
                    }
                }
            }
        }

        Rectangle {
            id: coldx

            color: "#ffffff"
            border.color: "#ffffff"
            Layout.rowSpan   : 1
            Layout.columnSpan: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                Rectangle {
                    id: tabbr
                    Layout.fillWidth: true
                    height: 40
                    color: "#ffffff"
                    border.color: "#c2c1cc"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                    TabBar {
                        id: bar
                        //width: parent.width
                        topPadding: 9
                        TabButton {
                            width: 67
                            text: qsTr("App 1")
                        }
                        TabButton {
                            width: 65
                            text: qsTr("App 2")
                        }
                        TabButton {
                            width: 65
                            text: qsTr("App 3")
                        }
                    }
                }

                Rectangle {
                    id: webarea
                    Layout.fillWidth: true
                    color: "#E9E9E9"
                    border.width: 0
                    border.color: "#c2c1cc"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.fillHeight: true

                    Button {
                        x: 481
                        y: -46
                        text: "Expand"
                        onClicked:  {
                            colsx.Layout.preferredWidth  = 200
                            colsxInner.width  = 200
                        }
                    }

                    StackLayout {
                        width: parent.width
                        currentIndex: bar.currentIndex
                        Item {
                            id: app1
                            Label {
                                text: "App 1"
                                height: 40
                                Layout.alignment: Layout.Center
                                color: "#50506e"
                                font.pointSize: 22
                            }
                        }
                        Item {
                            id: app2
                            Label {
                                text: "App 2"
                                height: 40
                                Layout.alignment: Layout.Center
                                color: "#50506e"
                                font.pointSize: 22
                            }
                        }
                        Item {
                            id: app3
                            Label {
                                text: "App 3"
                                height: 40
                                Layout.alignment: Layout.Center
                                color: "#50506e"
                                font.pointSize: 22
                            }
                        }
                    }
                }


            }
        }

    }
}









































/*##^## Designer {
    D{i:10;anchors_height:40}
}
 ##^##*/
