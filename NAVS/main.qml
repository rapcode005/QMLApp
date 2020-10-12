import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import FontAwesome 1.0
import backend 1.5
import QtQml 2.12
import notificationsmodel 1.0
import appfilterproxy 1.0

ApplicationWindow {
    id: navs
    visible: true
    width: 640
    height: 480
    flags: Qt.FramelessWindowHint | Qt.CustomizeWindowHint | Qt.WindowMinMaxButtonsHint | Qt.WindowTitleHint
    title: qsTr("NAVS")

    property int prvX: 0

    onScreenChanged: {
        timer.setTimeout(function(){
            processMonitorChange();
        }, 3000);

        console.log("Screen Changed.");
    }

    BackEnd {
        id: backend
    }

    onClosing:  {
        if(startupContainer.allowClose){
            close.accepted = true;
        }else if(!backend.isCUAdmin()){
            close.accepted = false;
            //windowStateChange();
            navs.showMinimized();
        }

        if(!startupContainer.visible){
            if(backend.isCUAdmin()){
                close.accepted = true;
            }
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

    Timer {
        id: charlieTimer
        interval: 10000; running: true; repeat: true
        onTriggered: {
            backend.checkCharlieProcess();
        }
    }

    Timer {
        id: updateTimer
        interval: 1000; running: true; repeat: true
        onTriggered: {
            checkForMidnightUpdate();
        }
    }

    function getprvX(){
        return navs.prvX;
    }

    function processMonitorChange(){
        //console.log("Processing Monitor change");
        handle.goSmall();
        handle.y = Screen.virtualY + (Screen.height/2 -  35)
        handle.x = Screen.virtualX + Screen.width - 70

        if(navsFull.width > Screen.width){
            navsFull.width = Screen.width - 50;
        }
        if(navsFull.height > Screen.height){
            navsFull.height = Screen.height - 50;
        }
    }

    function checkForMidnightUpdate(){
        var dt = new Date();
        var secs = dt.getSeconds() + (60 * dt.getMinutes()) + (60 * 60 * dt.getHours());
        if(secs === 86399){
            backend.getAvailableVersionNumber();
        }
    }

    function windowStateChange(){
        try{
            if(!backend.isCUAdmin() && backend.loginData["session_key"]){
                if(!navs.active){
                    //Was just minimized

                    timer.setTimeout(function(){
                        raise();
                        showNormal();
                        navs.visible = false;
                    }, 500);
                }
            }
        }catch(e){
            console.log(e);
        }
    }

    function getFullStartupWidth(){
        return Screen.width/2;
    }

    function getFullStartupHeight(){
        var screens = Qt.application.screens;
        var maxHeight = 0;
        for(var i=0; i<screens.length; i++){
            if(maxHeight === 0){
                maxHeight = screens[i].height;
            }
            if(screens[i].height < maxHeight){
                maxHeight = screens[i].height;
            }
        }
        return maxHeight*0.8;
    }

    function getFullStartupX(){
        return Screen.virtualX + getFullStartupWidth()/2;
    }

    function getFullStartupY(){
        return Screen.virtualY + ((Screen.height - getFullStartupHeight()) /2);
    }

    function showSecondaryLogin(srcComponent, showCancel){
        navsFull.prepMinimized();
        navs.flags = Qt.FramelessWindowHint;
        navs.width = 640;
        navs.height = 480;
        handle.visible = false;
        if(srcComponent === "navsFull"){
            navs.y = Screen.virtualY + (Screen.height/2 -  navs.height/2);
            navs.x = Screen.virtualX + Screen.width/2 - navs.width/2;
        }
        secondaryLogin.sourceComponent = srcComponent;
        navsFull.visible = false;
        startupContainer.visible = false;
        secondaryLogin.resetValues();
        secondaryLogin.setCancelEnabled(showCancel);
        secondaryLogin.visible = true;
    }

    function restorenavsFullFromLogin(){
        navsFull.visible = true;
        handle.visible = true;
        navsFull.restoreWindow();
    }

    function isSecondaryLoginVisible(){
        return secondaryLogin.visible;
    }

    function showHandle(){
        handle.visible = true;
    }

    //Start-up Screens
    StartupScreens{
        id:startupContainer
        onGoFullNavs: {
            navsFull.startUp();
        }
    }

    //Handle
    NavsHandle{
        id:handle
    }

    //Alpha1 full
    NavsFull{
        id:navsFull
    }

    //Login Screen
    LoginWindow{
        id:secondaryLogin
    }

}
