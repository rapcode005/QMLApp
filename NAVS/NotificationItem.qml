import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import FontAwesome 1.0
import QtGraphicalEffects 1.14

Item {
    id: notificationRoot
    x:300
    //x:0
    y:0
    visible: true
    width: 300
    height: 150

    property variant notificationData;
    property int myDelay: 0;
    property int myDelayOut : 0;

    function slideIn(y, dataObject, notificationDelay){
        myDelay = notificationDelay * 500;
        myDelayOut = 4000 + (notificationDelay * 3000);
        notificationData = dataObject;
        notificationRoot.y = y;
        notificationTitle.text = dataObject.messageTitle;
        notificationText.text = dataObject.messageBody;
        engageButton.text = dataObject.callToAction;
        if(dataObject.appIcon.length > 0){
            notReplaceIcon.visible = false;
            notifIcon.source = dataObject.appIcon;
            notifIcon.visible = true;
        }

        if(dataObject.notificationType === 1){
            notificationInTemp.start();
        }else{
            notificationInPerm.start();
        }
    }

    function goAction(){
        root.lauchAppFromNotification(notificationData.appId, notificationData.actionUrl);
        notificationDismiss.start();
    }

    function getNotificationId(){
        return notificationData.notificationId;
    }

    function setNewY(yVal){
       upYAnimation.to = yVal;
       notificationMoveY.start();
    }

    Rectangle{
        x:0
        y:0
        width:300
        height:150
        color:"#333333"
        opacity: 0.95
    }

    Text {
        id:notReplaceIcon
        x: 8
        y: 13
        width:35
        height:35
        text: FontAwesome.thList
        font.family: FontAwesome.fontFamily
        font.pixelSize: 30
        color: "#ffffff"
    }

    Image {
        id: notifIcon
        x: 8
        y: 13
        width: 40
        height: 40
        fillMode: Image.PreserveAspectFit
        visible:false

        property bool rounded: true
        property bool adapt: true

        layer.enabled: rounded
        layer.effect: OpacityMask {
            maskSource: Item {
                width: notifIcon.width
                height: notifIcon.height
                Rectangle {
                    anchors.centerIn: parent
                    width: notifIcon.adapt ? notifIcon.width : Math.min(notifIcon.width, notifIcon.height)
                    height: notifIcon.adapt ? notifIcon.height : width
                    radius: 9
                }
            }
        }
    }

    Text {
        id: notificationTitle
        x: 58
        y: 13
        width: 231
        height: 23
        color: "#ffffff"
        text: qsTr("Notification Text Here!")
        font.bold: true
        font.pixelSize: 13
    }

    Text {
        id: notificationText
        x: 58
        y: 35
        wrapMode: Text.WordWrap
        width: 231
        height: 100
        color: "#EEEEEE"
        text: qsTr("Body.")
        font.pixelSize: 12
    }

    Button {
        x:10
        y:100
        width:120
        height:40
        onClicked: {
            goAction();
        }

        contentItem: Text {
            id:engageButton
            text: "Go"
            font: engageButton.font
            opacity: enabled ? 1.0 : 0.3
            color: engageButton.down ? "#000000" : "#333333"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 120
            implicitHeight: 40
            color: engageButton.down ? "#666666" : "#CCCCCC"
            border.color: "#FFFFFF"
            border.width: 1
            radius: 1
        }
    }

    Button {
        id:dismissButton
        x:150
        y:100
        width:100
        height:40
        onClicked: {
            notificationInTemp.stop();
            notificationInPerm.stop();
            notificationDismiss.start();
        }

        contentItem: Text {
            text: "Dismiss"
            font: dismissButton.font
            opacity: enabled ? 1.0 : 0.3
            color: dismissButton.down ? "#000000" : "#333333"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            color: dismissButton.down ? "#666666" : "#CCCCCC"
            border.color: "#FFFFFF"
            border.width: 1
            radius: 1
        }
    }

    //Notification that stays for 5 seconds
    SequentialAnimation {
        id: notificationInTemp
        running: false

        onFinished: {
            notificationWindow.removeNotification(notificationData);
        }
        PauseAnimation { duration: myDelay }
        PropertyAnimation {
            target: notificationRoot;
            property: "x";
            to: 0;
            duration: 200;
        }
        PauseAnimation { duration: myDelayOut }
        PropertyAnimation {
            id:tempFinish
            target: notificationRoot;
            property: "x";
            to: 300;
            duration: 200;
        }
    }

    //Notification that sticks
    SequentialAnimation {
        id: notificationInPerm
        running: false
        PauseAnimation { duration: myDelay }
        PropertyAnimation {
            target: notificationRoot;
            property: "x";
            to: 0;
            duration: 200;}
    }

    SequentialAnimation {
        id: notificationDismiss
        running: false

        onFinished: {
            notificationWindow.removeNotification(notificationData);
        }

        PropertyAnimation {
            target: notificationRoot;
            property: "x";
            to: 300;
            duration: 200;
            onFinished: {
                console.log("remove finished");
                notificationMoveY.stop();
                notificationWindow.removeNotification(notificationData);
            }
        }
    }

    SequentialAnimation {
        id: notificationMoveY
        running: false
        PropertyAnimation {
            id: upYAnimation
            target: notificationRoot;
            property: "y";
            to: 0;
            duration: 100;
        }
    }
}
