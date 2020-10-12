import QtQuick 2.14
import QtQuick.Particles 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import FontAwesome 1.0
import QtQml.Models 2.14
import QtGraphicalEffects 1.14

Window {
    id:handleWindow
    visible: false
    width:70;
    height:70;
    y:Screen.virtualY + (Screen.height/2 -  35)
    x:Screen.virtualX + Screen.width - 70
    title: qsTr("NAVS Handle")
    flags: Qt.Widget | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    property bool isSmall: true

    property alias quickApllList: quickAppRepeater.model;

    function launchDrawerApp(appId){
       goSmall();
       navsFull.goBig(1, appId);
    }

    function updateTotalBadgeCount(cnt){
        redBadgeNum.text = cnt;
        if(cnt > 0){
            redBadge.visible = true;
        }else{
            redBadge.visible = false;
        }
    }

    MouseArea{
        id: navsMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            goBig();
        }
        z:10
        propagateComposedEvents: true
        onExited: {
           goSmall();
        }
    }

    Rectangle{
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {position: 0.0; color: "#cfcfd6"}
            GradientStop{position: 1.0; color: "#9495a4"}
        }

        ParticleSystem {
            id: particle
            anchors.fill: parent

            ImageParticle {
                source: "qrc:/images/glowdot3.png"
                opacity: 1
            }

            Emitter {
                width: parent.width
                height: parent.height
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                lifeSpan: 15000
                startTime: 15000
                emitRate: 20
                size: 2
                sizeVariation: 16
                acceleration: PointDirection{y: -22; xVariation: 2; yVariation: 2}
            }

            Wander {
                xVariance: 25
                pace: 25
            }

            Image{
                id:navsLogo
                source:"images/logoRed.png"
                fillMode: Image.PreserveAspectFit
                x: 20;
                y: 0;
                width:100
                height:60
                visible: (!handleWindow.isSmall)? true : false;
            }

            Image{
                id:navsLogoN
                source:"images/nRed.png"
                fillMode: Image.PreserveAspectFit
                x: 5;
                y: 5;
                width:60
                height:60
                visible: (handleWindow.isSmall)? true : false;
            }

            Rectangle {
                id:redBadge
                x: 5
                y: 5
                width: 20
                height: 20
                color: "red"
                radius: width*0.5
                visible: false
                Text {
                    id:redBadgeNum
                    anchors.centerIn: parent;
                    color: "#ffffff";
                    text: "0";
                }
            }

            Rectangle {
                id:allAppsBarTop
                width: 125
                height: 2
                color: "#FFFFFF"
                x:16
                y:53
                visible: (!handleWindow.isSmall)? true : false;
            }

            ScrollView{
                id: allAppScrollView
                width:parent.width-35
                height:parent.height-115
                clip: true
                x:25
                y:55
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                visible: (!handleWindow.isSmall)? true : false;
                bottomPadding: 20

                Flow {
                    layoutDirection: Qt.LeftToRight
                    spacing: 20;
                    width:parent.width
                    bottomPadding: 20
                    move: Transition {
                        NumberAnimation {
                            properties: "x"
                        }
                    }

                    DrawerRepeater{
                        id:quickAppRepeater
                    }

                }
            }


            Rectangle {
                id:allAppsBarBottom
                width: 125
                height: 2
                color: "#FFFFFF"
                x:16
                y:handleWindow.height-50
                visible: (!handleWindow.isSmall)? true : false;
            }

            Rectangle{
                id:allAppsText
                x: 8
                y: handleWindow.height-40
                width:100
                height:40
                color: "transparent"
                visible: (!handleWindow.isSmall)? true : false;

                Text {
                  text: "All Apps"
                  horizontalAlignment: Text.AlignHCenter
                  width: parent.width
                  color: "#ffffff"

                  font {
                      family: montreuxCDmBd.name;
                      pixelSize: 20;
                      capitalization: Font.Capitalize;
                      weight: Font.DemiBold
                  }
                }

                MouseArea {
                    id: mouseAllApps
                    anchors.fill: parent

                    onClicked: {
                        goSmall();
                        navsFull.goBig(2,0);
                    }
                }
            }


            Rectangle {
                id:allAppsButton
                x: 108
                y: handleWindow.height-40
                color: "transparent"
                width:30
                height:30
                visible: (!handleWindow.isSmall)? true : false;

                Text {
                  text: FontAwesome.externalLink
                  horizontalAlignment: Text.AlignHCenter
                  width: parent.width
                  color: "#ffffff"

                  font {
                      family: FontAwesome.fontFamily;
                      pixelSize: 25;
                  }
                }

                MouseArea {
                    id: mouseFont
                    anchors.fill: parent
                    onClicked: {
                        goSmall();
                        navsFull.goBig(2,0);
                    }
                }
            }

        }
    }

    function goSmall(){
        handleWindow.y = Screen.virtualY + (Screen.height/2 -  35);
        handleWindow.x = Screen.virtualX + Screen.width - 70;
        handleWindow.width = 70;
        handleWindow.height = 70;
        handleWindow.isSmall = true;
    }

    function goBig(){
        handleWindow.width = 150;
        handleWindow.height = 440;
        handleWindow.y = handleWindow.y - 185;
        handleWindow.x = handleWindow.x - 80;
        handleWindow.isSmall = false;
    }
}
