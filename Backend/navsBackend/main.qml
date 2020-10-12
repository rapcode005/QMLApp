import QtQuick 2.13
import QtQuick.Controls 2.13
import backend 1.2
import QtQml 2.3

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true

    BackEnd {
        id: backend
    }

    //Run tests here.------
    Component.onCompleted: function(){
        console.log("Running");
        //backend.loadOrganizationName("quaerat");
        //backend.loadOrganizationLocation("quaerat");
        //The backend can be referenced with: backend
    }


    ScrollView {
        id: scrollView
        x: 0
        y: 0
        width: 330
        height: 500
        anchors.fill: parent

        TextArea {
            function getOrganizationData(){
                 return backend.registerStatus;
            }
            id: txt1
            width: 211
            height: 78
            wrapMode: Text.WordWrap
        }

        ListView {
            anchors.fill: parent
            x: 0
            y: 50
            id: listview
            delegate: Item {
                   width: ListView.view.width
                   height: 40

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                        }
                    }

                   Text {
                          //anchors.centerIn: parent
                          //font.bold: true
                          // NOTE: This is where the roleName comes into play
                          // There is a magic object called "dataObject" that references the
                          // MusicTrack object for this particular index
                          text: "Name:" + dataObject.name + "\nID:" + dataObject.organizationID +
                          "\nLocation:" + dataObject.location + "\nDns key:" + dataObject.dnsKey
                    }
                }
             }

    }

        Button{
            id: btn
            x: 0
            y: 70
            text: "Test Loading the User"
            onClicked: function(){
                //"quaerat"
                //backend.checkOrganizationNameStatus("laboriosam");
                //backend.loadOrganization("cunw");
                //listview.model = backend.organizationData;
                //backend.loadInstallationData();
                //backend.createLog("Descriptive", "Hardware", "high");
                //listview.model = backend.organizationData;
                //txt1.text = backend.organizationNameStatus;
                //backend.loadInstallationData();
                //backend.checkRegisterStatus();
                //backend.checkRegisterStatus();
                //txt1.text = backend.registerStatus;
                //backend.loginOrganization("administrator", "admin1234");
                //console.log(backend.loginOrganizationData["organization_user"]["user_type"]["name"]);
                //console.log(backend.loginOrganizationData["session_key"]);
                //txt1.text = BackEnd.loginOrganizationData["organization"]["user_type"]["name"];
                backend.createISOFile();
            }
        }
    }
