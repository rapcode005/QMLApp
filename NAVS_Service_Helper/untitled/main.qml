import QtQuick 2.14
import QtQuick.Window 2.14
import tester 1.0
import QtQuick.Controls 2.12

ApplicationWindow {
    id:charlie1
    visible: false
    width: 640
    height: 480
    title: qsTr("NAVS Service Helper")
    flags: Qt.FramelessWindowHint

    Tester{
        id:tester
    }

    onClosing:  {
       close.accepted = false;
    }

    Timer {
        interval: 5000; running: true; repeat: true
        onTriggered: {
            if(tester.isUpdating()){
                charlie1.visible = true;
            }else{
                charlie1.visible = false;
                if(!tester.runCheck()){
                    tester.startNAVS();
                }
            }
        }
    }

    Text {
        id: element
        x: 120
        y: 218
        width: 388
        height: 60
        text: qsTr("NAVS is currently updating. Please wait.")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
    }
}
