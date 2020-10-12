import QtQuick 2.0
import QtGraphicalEffects 1.14

Repeater {
    id: appRepeater

    function getAppHeight(len){
        if(len <= 12){
            return 105;
        }else if(len <= 18){
            return 112;
        }else{
            return 125;
        }
    }

    Loader {
        id : allAppLoaderEm
        property int value: index

        sourceComponent: Component {
            Rectangle {
                 id: rectApp
                 width: 90
                 height: getAppHeight(dataObject.namelength)
                 x:0
                 y:0
                 color: "transparent"

                 MouseArea {
                     id: dragAreaQuick
                     anchors.fill: parent

                     onDoubleClicked: {
                         handleWindow.launchDrawerApp(dataObject.appId);
                     }

                     Rectangle {
                         id: tileQuick
                         width: 100
                         height: getAppHeight(dataObject.namelength)
                         color: "transparent"

                        Column {
                            y:7
                            width:100

                            Image {
                                id:appIcon
                                height: 80
                                x:10
                                width: 80
                                source: dataObject.icon
                                fillMode: Image.PreserveAspectFit

                                property bool rounded: true
                                property bool adapt: true

                                layer.enabled: rounded
                                layer.effect: OpacityMask {
                                    maskSource: Item {
                                        width: appIcon.width
                                        height: appIcon.height
                                        Rectangle {
                                            anchors.centerIn: parent
                                            width: appIcon.adapt ? appIcon.width : Math.min(appIcon.width, appIcon.height)
                                            height: appIcon.adapt ? appIcon.height : width
                                            radius: 16
                                        }
                                    }
                                }
                            }

                            Text {
                                text: dataObject.name
                                horizontalAlignment: Text.AlignHCenter
                                width: 100
                            }
                          }

                         Rectangle {
                             id:redBadge
                             x: 65
                             y: 0
                             width: 20
                             height: 20
                             color: "red"
                             radius: width*0.5
                             visible: (dataObject.badgeCount > 0)? true: false;
                             Text {
                                 id:redBadgeNum
                                 anchors.centerIn: parent;
                                 color: "#ffffff";
                                 text: dataObject.badgeCount;
                             }
                         }
                     }

                     hoverEnabled: true

                     cursorShape: Qt.PointingHandCursor
                 }

             }

        }
    }
}
