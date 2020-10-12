import QtQuick 2.12
import backend 1.5
import QtWebEngine 1.10
import QtWebChannel 1.0
import QtQuick.Dialogs 1.3

WebEngineView {
    id: webEngineNavs
    height:  parent.height
    width: parent.width
    x:0
    y:0

    //Unique value
    property int ids

    property var uploadedFilePath
    property var uploadedFileFolderLoc: ""

    onFileDialogRequested: {

        request.accepted = true; // inhibit default dialog

        dialog.request = request;
        dialog.folder =  webEngineNavs.uploadedFileFolderLoc;
        dialog.open();
    }

    FileDialog {
        id: dialog
        property var request

        onRejected: request.dialogReject()

        onAccepted: {
            webEngineNavs.uploadedFilePath = fileUrl.toString().substring(8);
            webEngineNavs.uploadedFileFolderLoc = folder;
            request.dialogAccept(fileUrl.toString().substring(8));
            webEngineNavs.runJavaScript('navs.api.loadMD5("' + webEngineNavs.uploadedFilePath + '");');
            //webEngineNavs.runJavaScript('navs.api.downloadISO(301, "view");');
        }
    }

    webChannel: WebChannel {
        id: channel
    }

    //This way the scripts get injected before the document is rendered.
    userScripts: [

        WebEngineScript{
            id: navsJSInjection
            injectionPoint: WebEngineScript.DocumentCreation
            worldId: WebEngineScript.MainWorld

            Component.onCompleted: {
                channel.registerObject("navsObj", backend.navsObject);
                navsJSInjection.setSourceCode(
                            backend.getJsCode("webchannel.txt")+
                            backend.getJsCode("NAVSAPI.txt")+
                            ";window.navsappTabId="+ids+";"
                );
            }
        }

   ]

}
