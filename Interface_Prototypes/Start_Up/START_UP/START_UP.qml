import QtQuick 2.13
import QtQuick.Window 2.13
import "installWindowLoader.js" as WindowLoader

Window {
    id: primaryWindow
    visible: true
    width: 480
    height: 640
    title: qsTr("NAVS")
    flags: Qt.FramelessWindowHint

    Component.onCompleted: WindowLoader.startInstallProcess();
}
