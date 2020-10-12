import QtQuick 2.0
import QtQuick.Particles 2.0
import QtQuick.Window 2.13

//Drawer
Rectangle {
    id:root
    anchors.fill: parent
    opacity: 1
    visible:false

    gradient: Gradient {
        GradientStop {position: 0.0; color: "#ffffff"}
        GradientStop{position: 1.0; color: "#82829F"}
    }

    ParticleSystem {
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

        Text {
            id: drawerText
            x: 7
            y: 92
            color: "#50506E"
            text: qsTr("NAVS")
            font { family: montreuxCDmBd.name; pixelSize: 30; capitalization: Font.Capitalize; weight: Font.DemiBold }

            transform: Rotation {
                angle: -90
            }
        }

        Rectangle {
            id: drawerLight
            x: 2
            y: 2
            width: 15
            height: 15
            color: "#ff0000"
            radius: 6
            border.color: "black"
            border.width: 1
            visible:false
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onPressed:{
                navs.x = Screen.width-150
                navs.y = (Screen.height/2)-170
                navs.width = 150
                navs.height = 340
                drawerText.y = 192
                drawerLight.y = 102
            }
            //              onEntered:{

            //              }
            onExited: {
                var isOutside = false;

//                    console.log(mouseX);
//                    console.log(root.x);
//                    console.log(mouseY);
//                    console.log(root.y);
//                    console.log("------");

                if(mouseX >= 3 && mouseY <= 8){
//                        console.log("above");
                    isOutside = true;
                }else if(mouseX >= 3 && mouseY >= root.height-8){
//                        console.log("below");
                    isOutside = true;
                }else if(mouseX <= 10){
//                        console.log("left");
                    isOutside = true;
                }else{
                    //console.log("Inside");
                }

                if(isOutside){
                    navs.x = Screen.width-50
                    navs.y = (Screen.height/2)-50
                    navs.width = 50
                    navs.height = 100
                    drawerText.y = 92
                    drawerLight.y = 2
                }
            }
        }

        Column {
            id: wcolumn
            width: 60
            x:60

            Rectangle {
                id: wapp1
                width: 60
                height: 60
                color: "#82829F"
                radius: 10
                anchors.top: wcolumn.top
                anchors.topMargin: 10

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed:{
                        drawerText.y = 92;
                        navsFull.goBig(1,0)
                    }
                    onEntered:{
//                                            console.log("Entered App 1");
                    }
                    onExited: {
//                                            console.log("Left App 1");
                    }
                    cursorShape: Qt.PointingHandCursor
                }

                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter;
                        top: wapp1.bottom
                    }
                    color: "#50506E"
                    text: qsTr("Test App 1")
                    font { family: montreuxCDmBd.name; pixelSize: 11; capitalization: Font.Capitalize; weight: Font.DemiBold }
                }
            }

            Rectangle {
                id: wapp2
                width: 60
                height: 60
                color: "#82829F"
                radius: 10
                anchors.top: wapp1.bottom
                anchors.topMargin: 20

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed:{
                        drawerText.y = 92;
                        navsFull.goBig(1,1)
                    }
                    onEntered:{
//                                            console.log("Entered App 2");
                    }
                    onExited: {
//                                            console.log("Left App 2");
                    }
                    cursorShape: Qt.PointingHandCursor
                }

                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter;
                        top: wapp2.bottom
                    }
                    color: "#50506E"
                    text: qsTr("Test App 2")
                    font { family: montreuxCDmBd.name; pixelSize: 11; capitalization: Font.Capitalize; weight: Font.DemiBold }
                }
            }

            Rectangle {
                id: wapp3
                width: 60
                height: 60
                color: "#82829F"
                radius: 10
                anchors.top: wapp2.bottom
                anchors.topMargin: 20

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed:{
                        drawerText.y = 92;
                        navsFull.goBig(1,2)
                    }
                    onEntered:{
//                                            console.log("Entered App 3");
                    }
                    onExited: {
//                                            console.log("Left App 3");
                    }
                    cursorShape: Qt.PointingHandCursor
                }

                Text {
                    anchors {
                        horizontalCenter: parent.horizontalCenter;
                        top: wapp3.bottom
                    }
                    color: "#50506E"
                    text: qsTr("Test App 3")
                    font { family: montreuxCDmBd.name; pixelSize: 11; capitalization: Font.Capitalize; weight: Font.DemiBold }
                }
            }
        }

        Rectangle {
            width: 90
            height: 2
            color: "#FFFFFF"
            x:45
            y:parent.height-85
        }

        Rectangle {
            id: wAllApps
            width: 60
            height: 60
            color: "#82829F"
            radius: 10
            x:60
            y:parent.height-75

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onPressed:{
                    drawerText.y = 92;
                    navsFull.goBig(2,0)
                }
                onEntered:{
//                                            console.log("Entered All Apps");
                }
                onExited: {
//                                            console.log("Left All Apps");
                }
                cursorShape: Qt.PointingHandCursor
            }

            Text {
                anchors {
                    horizontalCenter: parent.horizontalCenter;
                    top: wAllApps.bottom
                }
                color: "#50506E"
                text: qsTr("Welcome")
                font { family: montreuxCDmBd.name; pixelSize: 11; capitalization: Font.Capitalize; weight: Font.DemiBold }
            }
        }
    }
}
