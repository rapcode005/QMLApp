import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Window {
    id:notificationWindow
    x:Screen.width
    y:200
    visible: true
    width: 300
    height: 150
    title: qsTr("Hello World")
    color: "transparent"
    flags: Qt.Widget | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    Rectangle{
        x:0
        y:0
        width:300
        height:150
        color:"#333333"
        opacity: 0.95
    }

    Text {
        id: notificationTitle
        x: 48
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
        x: 48
        y: 35
        wrapMode: Text.WordWrap
        width: 231
        height: 100
        color: "#EEEEEE"
        text: qsTr("This is the body of the notification. It should be able to handle quite a bit of text.")
        font.pixelSize: 12
    }

    Image{
        source: "qrc:/app.png"
        width:30
        height:30
        x:10
        y:10
    }

    Button {
        id:engageButton
        x:10
        y:100
        width:120
        height:40
        onClicked: console.log("go")

        contentItem: Text {
            text: "View Weather"
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
            notificationIn.stop();
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

    SequentialAnimation {
        id: notificationIn
        running: true
        PropertyAnimation { target: notificationWindow; property: "x"; to: Screen.width - notificationWindow.width; duration: 200;}
        PauseAnimation { duration: 10000 }
        PropertyAnimation { target: notificationWindow; property: "x"; to: Screen.width; duration: 200;}
    }

    SequentialAnimation {
        id: notificationDismiss
        running: false
        PropertyAnimation { target: notificationWindow; property: "x"; to: Screen.width; duration: 200;}
    }
}
