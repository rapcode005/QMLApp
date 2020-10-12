import QtQuick 2.13
import QtQuick.Window 2.13
import FontAwesome 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Text {
     // The font will be loaded once the singleton object, FontAwesome, is referred in the application.
     text: FontAwesome.chevronLeft
     font.family: FontAwesome.fontFamily
    }
}
