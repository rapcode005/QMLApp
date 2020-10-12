import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    ColumnLayout {
        id: ly
        width: parent.width

        Label {
            text: "Test Dynamic Loaded QML."
            height: 40
            Layout.alignment: Layout.Center
            color: "#50506e"
            font.pointSize: 22
        }

        Label {
            text: "Here we'll load the first screen of the installer."
            height: 40
            Layout.alignment: Layout.Center
            color: "#50506e"
            font.pointSize: 22
        }
    }
}
