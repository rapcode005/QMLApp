import QtQuick 2.14
import QtQuick.Particles 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import FontAwesome 1.0
import QtQml.Models 2.14
import QtGraphicalEffects 1.14

//NavsDrawer
Rectangle {
    id:root
    anchors.fill: parent
    width:parent.width
    height:parent.height
    opacity: 1
    visible:false

    property int stateX : 0;
    property int stateY : 0;

    MouseArea{
        id: navsMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            if(root.visible && !navsFull.visible && navs.width < 160){
                navs.width = 150
                navs.height = 440
                navs.y = stateY - 170;
                navs.x = stateX - 80;
            }
        }
        z:10
        propagateComposedEvents: true
        onExited: {
            if(root.visible && !navsFull.visible && navs.width < 160){
                navs.width = 70
                navs.height = 70
                navs.y = stateY;
                navs.x = stateX;
            }
        }
    }

    property alias quickApllList: quickAppRepeater.model;
    property alias allAppList: allAppRepeater.model;

    function launchDrawerApp(appId){
       root.visible = false;
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

    function setStates(xVal, yVal){
        stateX = xVal;
        stateY = yVal;
    }

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
            visible: (root.height > 200)? true : false;
        }

        Image{
            id:navsLogoN
            source:"images/nRed.png"
            fillMode: Image.PreserveAspectFit
            x: 5;
            y: 5;
            width:60
            height:60
            visible: (root.height > 200)? false : true;
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
            visible: (root.height > 200)? true : false;
        }

        ScrollView{
            id: allAppScrollView
            width:parent.width-35
            height:parent.height-115
            clip: true
            x:35
            y:55
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            visible: (root.height > 200)? true : false;
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

                DrawerRepeater{
                    id:allAppRepeater
                }

            }
        }


        Rectangle {
            id:allAppsBarBottom
            width: 125
            height: 2
            color: "#FFFFFF"
            x:16
            y:navs.height-50
            visible: (root.height > 200)? true : false;
        }

        Rectangle{
            id:allAppsText
            x: 8
            y: navs.height-40
            width:100
            height:40
            color: "transparent"
            visible: (root.height > 200)? true : false;

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
                    root.visible = false;
                    navsFull.goBig(2,0);
                }
            }
        }


        Rectangle {
            id:allAppsButton
            x: 108
            y: navs.height-40
            color: "transparent"
            width:30
            height:30
            visible: (root.height > 200)? true : false;

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
                    root.visible = false;
                    navsFull.goBig(2,0);
                }
            }
        }
    }
}
