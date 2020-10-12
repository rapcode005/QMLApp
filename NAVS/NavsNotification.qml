import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id:notificationWindow
    visible: true
    width:1;
    height:1;
    title: qsTr("NAVS Notification")
    color: "transparent"
    flags: Qt.Widget | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    property variant notificationsArray : [];
    property variant notificationInX : Screen.width-300;
    property variant component : Qt.createComponent("NotificationItem.qml");

    function createNotification(dataObject, notificationDelay){
        notificationWindow.visible = true;
        notificationWindow.y = Screen.virtualY+50;

        notificationWindow.height = (notificationsArray.length+1)*170;
        notificationsArray.push(component.createObject(notificationWindow));
        notificationsArray[notificationsArray.length-1].slideIn(
                    (notificationsArray.length-1)*170,
                    dataObject,
                    notificationDelay
                    );
    }

    function removeNotification(notificationData){
        var fnd = -1;
        for (var i=0; i<notificationsArray.length; i++){
            if(notificationsArray[i].getNotificationId() === notificationData.notificationId){
                fnd = i;
                notificationsArray[i].destroy();
            }
        }
        if(fnd > -1){
            notificationsArray = notificationsArray.slice(0, fnd)
                .concat(notificationsArray
                .slice(fnd + 1, notificationsArray.length));
        }
        for (var p=0; p<notificationsArray.length; p++){
            //Move all of them up.
            notificationsArray[p].setNewY(p*170);
        }
        if(notificationsArray.length === 0){
            notificationWindow.width = 1;
            notificationWindow.height = 1;
        }
        //console.log("Call backend to remove notification mongo id = " + notificationData.notificationId);
        root.notificationDisplayed(notificationData);
    }
}
