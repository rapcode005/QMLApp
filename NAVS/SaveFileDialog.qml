import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3

Item {
    //added 4.8.2020
    FileDialog {
        id: fileSaveDialog
        objectName: "saveFileDialog" //essential if your going to use QQmlContext
        title: "Save to a new location"
        selectFolder: true

        folder: shortcuts.home

        signal acceptClicked(msg: string)

        signal rejectedClicked();

        onAccepted: fileSaveDialog.acceptClicked(folder.toString().substring((8)));

        onRejected: fileSaveDialog.rejectedClicked();  //console.log("rejected");

        Component.onCompleted: visible = true

    }
}
