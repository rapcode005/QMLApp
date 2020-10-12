import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import FontAwesome 1.0
import QtQml.Models 2.13


ApplicationWindow {
    id: navs
    visible: true
    width: 640
    height: 480
    flags: Qt.FramelessWindowHint
    title: qsTr("NAVS")

    onClosing: {
        if(startupContainer.allowClose){
            close.accepted = true;
        }else{
            close.accepted = false;
        }
    }

    FontLoader { id:montreuxCDmBd;  source: "qrc:/fonts/windows/MontreuxC-DmBd.otf" }
    FontLoader { id:montreuxCRg;  source: "qrc:/fonts/windows/MontreuxC-Rg.otf" }

    Timer {
        id: timer
        function setTimeout(cb, delayTime) {
            timer.interval = delayTime;
            timer.repeat = false;
            timer.triggered.connect(cb);
            timer.triggered.connect(function release () {
                timer.triggered.disconnect(cb); // This is important
                timer.triggered.disconnect(release); // This is important as well
            });
            timer.start();
        }
    }

    property int restoreX: 0
    property int restoreY: 0
    property int restoreWidth: 0
    property int restoreHeight: 0

    function windowStateChange(){
        if(!navs.active){
            //Switch to the drawer view
            navsFull.visible = false;
            navs.flags = Qt.FramelessWindowHint;
            navs.flags += Qt.WindowStaysOnTopHint;
            navs.x = Screen.width-50;
            navs.y = (Screen.height/2)-50
            navs.width = 50;
            navs.height = 100;

            drawer.visible = true;
            timer.setTimeout(function(){
                showNormal();
            }, 500);
        }
    }

    function getFullStartupWidth(){
        if(Screen.width>1200){
            restoreWidth = 1200;
            return 1200;
        }
        restoreWidth = Screen.width;
        return Screen.width;
    }

    function getFullStartupHeight(){
        if(Screen.width>1600){
            restoreHeight = 1600;
            return 1600;
        }
        restoreHeight = Screen.height;
        return Screen.height;
    }

    function getFullStartupX(){
        var w = getFullStartupWidth();
        if(w !== Screen.width){
            restoreX = (Screen.width/2) - (getFullStartupWidth()/2);
            return restoreX;
        }

        restoreX = 0;
        return 0;
    }

    function getFullStartupY(){
        var h = getFullStartupHeight();
        if(h !== Screen.height){
            restoreY = (Screen.height/2) - (getFullStartupHeight()/2);
            return restoreY;
        }

        restoreY = 0;
        return 0;
    }

    //Start-up Screens
    StartupScreens{
        id:startupContainer
        onGoFullNavs: {
            navsFull.startUp();
        }
    }

    //Drawer
    Drawer{
        id:drawer
    }

    //Alpha1 full
    NavsFull{
        id:navsFull
    }
}


