import QtQuick 2.0
import QtQuick.Controls 2.5

//Startup
Item {
    id: root
    width: 640
    height: 480
    visible:true

    signal goFullNavs

    property bool allowClose: false

    function verifyClose(){
        cancelInstallDialog.open();
    }

    function attemptLoginOption1(){
        console.log("Attempting Login");

        loginUsernameInput.enabled = false;
        loginPasswordInput.enabled = false;
        loginNextButton.enabled = false;
        loginBackButton.enabled = false;
        loginCancelButton1.enabled = false;

        if(loginUsernameInput.text.length > 0 && loginPasswordInput.text.length > 0){
            loginTextMissingError.visible = false;
            loginTextVerifError.visible = false;
            locationChoices.focus = true;
            organizationLocationNext.start();
        }else{
            loginUsernameInput.enabled = true;
            loginPasswordInput.enabled = true;
            loginNextButton.enabled = true;
            loginBackButton.enabled = true;
            loginCancelButton1.enabled = true;
            loginTextMissingError.visible = true;
            loginUsernameInput.focus = true;
        }
    }

    function assignLocation(){
        console.log("Assigning Organization Location");
        installingNext.start();
        finishUpBar.start();

        timer.setTimeout(function(){
            navs.flags = Qt.Window;
            root.goFullNavs()
        }, 2300);
    }

    function ckeckOrganization(){
        console.log("Checking organization");
        orgTextInput.enabled = false;
        orgNextButton.enabled = false;
        orgCancelButton.enabled = false;
        loginUsernameInput.focus = true;

        if(orgTextInput.text.length > 0){
            orgTextInputError.visible = false;
            loginUsernameInput.focus = true;
            loginNext.start();
        }else{
            orgTextInput.enabled = true;
            orgNextButton.enabled = true;
            orgCancelButton.enabled = true;
            orgTextInputError.visible = true;
            orgTextInput.focus = true;
        }
    }

    //Particle Background
    InstallerBackground{
        id:installerBackground
    }

    //Splash screen
    Rectangle {
        id:splashScreen
        x:100
        //            x:641
        y:0
        width: 540;
        height: 480;
        color: "#ffffff"
        opacity: 0.9
        visible:true


        Text {
            id: splashNavsLogo
            x: 60
            y: 95
            width: 408
            height: 114
            color: "#50506E"
            text: qsTr("NAVS")
            font { family: montreuxCDmBd.name; pixelSize: 100; capitalization: Font.Capitalize; weight:Font.DemiBold;}
            horizontalAlignment: Text.AlignHCenter
        }

        TextArea {
            id: splashNavsDisclosures
            x: 87
            y: 225
            width: 354
            color: "#82829f"
            height: 81
            text: qsTr("2019.1 Release\n\n@ 2019 CU Northwest and its licensors. All Rights Reserved. See the legal notices in the about box.")
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font { family: montreuxCRg.name; pixelSize: 13; weight:Font.Normal;}
        }

        Text {
            id: splashLoadingText
            x: 198
            y: 371
            width: 111
            height: 49
            color: "#e695a3"
            text: qsTr("Loading...")
            horizontalAlignment: Text.AlignHCenter
            font { family: montreuxCRg.name; pixelSize: 13; weight:Font.Normal;}
        }

        Component.onCompleted: function(){
            if(true){
                console.log("Not installed.");
                timer.setTimeout(function(){ splashOut.start(); }, 1500);
            }else{
                console.log("Is installed");
            }
        }


        SequentialAnimation {
            id: splashOut
            running: false
            PropertyAnimation { target: splashLoadingText; property: "text"; to: "Done"; duration: 1;}
            PauseAnimation { duration: 500 }
            PropertyAnimation { target: splashScreen; property: "x"; to: 641; duration: 300; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: organizationScreen; property: "x"; to: 100; duration: 300; easing.type: Easing.InOutQuad}
        }

    }

    //Organization screen
    Rectangle {
        id:organizationScreen
        x:641
        //            x:100
        y:0
        width: 540;
        height: 480;
        color: "#ffffff"
        opacity: 1
        visible:true

        Component.onCompleted: function(){
            //console.log("Running");
        }

        Text {
            id: orgText1
            x: 53
            y: 84
            width: 357
            height: 35
            color: "#50506E"
            text: qsTr("Sign in to your organization")
            font { family: montreuxCDmBd.name; pixelSize: 30; weight:Font.DemiBold;}
        }

        Text {
            id: orgText2
            x: 110
            y: 167
            color: "#50506E"
            text: qsTr("Enter your NAVS organization domain")
            font { family: montreuxCDmBd.name; pixelSize: 15; weight:Font.DemiBold;}
        }

        TextField {
            id: orgTextInput
            x: 60
            y: 200
            width: 249
            height: 42
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            focus: true
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignBottom
            selectionColor: "#CCCCCC"
            selectByMouse: true
            color: orgTextInput.enabled ? "#50506E" : "#82829F"
            KeyNavigation.tab: orgNextButton
            placeholderText: qsTr("mydomain")
            Keys.onReturnPressed: {
                ckeckOrganization();
            }

            background: Rectangle {
                color: orgTextInput.enabled ? "transparent" : "#C2C1CD"
                border.color: orgTextInput.activeFocus ?  "#50506E" : "#C2C1CD"
            }
        }

        Text{
            id: orgTextInputError
            x: 60
            y: 250
            text: qsTr("You must supply a NAVS organization domain")
            color:"#E96178"
            font { family: montreuxCRg.name; pixelSize: 14; weight:Font.Normal;}
            visible:false;
        }

        Text {
            id: orgDomainSuffixText
            x: 315
            y: 215
            color: "#50506E"
            text: qsTr(".navsnow.com")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
        }

        Button {
            id: orgNextButton
            x: 262
            y: 320
            width: 245
            height: 42
            text: qsTr("NEXT")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            onClicked: ckeckOrganization()
            KeyNavigation.tab: orgCancelButton
            Keys.onReturnPressed: {
                ckeckOrganization()
            }

            contentItem: Text {
                color: orgNextButton.down || ! orgNextButton.enabled ? "#82829F" : "#FFFFFF"
                text: orgNextButton.text
                font: orgNextButton.font
                opacity: enabled ? 1.0 : 0.3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                opacity: enabled ? 1 : 0.3
                color: orgNextButton.down ? "#50506E" : "#82829F"
                border.color: orgNextButton.activeFocus ? "#50506E" : "#82829F"
                border.width: 2
                radius: 2
            }
        }

        Button {
            id: orgCancelButton
            x: 104
            y: 320
            width: 152
            height: 42
            text: qsTr("CANCEL")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            onClicked: verifyClose()
            KeyNavigation.tab: orgTextInput
            Keys.onReturnPressed: {
                verifyClose()
            }

            contentItem: Text {
                color: orgCancelButton.down || ! orgCancelButton.enabled ? "#82829F" : "#FFFFFF"
                text: orgCancelButton.text
                font: orgCancelButton.font
                opacity: enabled ? 1.0 : 0.3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                opacity: enabled ? 1 : 0.3
                color: orgCancelButton.down ? "#50506E" : "#82829F"
                border.color: orgCancelButton.activeFocus ? "#50506E" : "#82829F"
                border.width: 2
                radius: 2
            }
        }

        SequentialAnimation {
            id: loginNext
            running: false
            PropertyAnimation { target: organizationScreen; property: "x"; to: 641; duration: 300; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: loginScreen; property: "x"; to: 100; duration: 300; easing.type: Easing.InOutQuad}
        }
    }

    //Login screen
    Rectangle {
        id:loginScreen
        x:641
        //            x:100
        y:0
        width: 540;
        height: 480;
        color: "#ffffff"
        opacity: 1
        visible:true

        Component.onCompleted: function(){
            //console.log("Running");
        }

        Text {
            id: loginTitle
            x: 140
            y: 59
            width: 261
            height: 35
            color: "#50506E"
            text: qsTr("Login to NAVS")
            font { family: montreuxCDmBd.name; pixelSize: 30; weight:Font.DemiBold;}
        }

        Text {
            id: loginPasswordText
            x: 106
            y: 210
            text: qsTr("Password")
            color: "#50506E"
            font { family: montreuxCDmBd.name; pixelSize: 15; weight:Font.DemiBold;}
        }

        Text {
            id: loginUsernameText
            x: 106
            y: 132
            text: qsTr("Username")
            color: "#50506E"
            font { family: montreuxCDmBd.name; pixelSize: 15; weight:Font.DemiBold;}
        }

        TextField {
            id: loginUsernameInput
            x: 114
            y: 157
            width: 249
            height: 42
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            focus: true
            color: loginUsernameInput.enabled ? "#50506E" : "#82829F"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignBottom
            selectionColor: "#CCCCCC"
            selectByMouse: true
            KeyNavigation.tab: loginPasswordInput
            placeholderText: qsTr("username")
            Keys.onReturnPressed: {
                attemptLoginOption1()
            }

            background: Rectangle {
                color: loginUsernameInput.enabled ? "transparent" : "#C2C1CD"
                border.color: loginUsernameInput.activeFocus ?  "#50506E" : "#C2C1CD"
            }
        }

        TextField {
            id: loginPasswordInput
            x: 114
            y: 234
            width: 249
            height: 42
            color: loginPasswordInput.enabled ? "#50506E" : "#82829F"
            focus: true
            verticalAlignment: Text.AlignBottom
            font.pixelSize: 20
            font.family: montreuxCDmBd.name
            font.weight: Font.Normal
            horizontalAlignment: Text.AlignLeft
            placeholderText: qsTr("password")
            passwordCharacter: "*"
            selectionColor: "#CCCCCC"
            selectByMouse: true
            passwordMaskDelay: 500
            echoMode: TextInput.Password
            KeyNavigation.tab: loginNextButton
            Keys.onReturnPressed: {
                attemptLoginOption1()
            }

            background: Rectangle {
                color: loginPasswordInput.enabled ? "transparent" : "#C2C1CD"
                border.color: loginPasswordInput.activeFocus ?  "#50506E" : "#C2C1CD"
            }
        }

        Text{
            id: loginTextMissingError
            x: 90
            y: 300
            text: qsTr("You must supply a username and password.")
            color:"#E96178"
            font { family: montreuxCRg.name; pixelSize: 14; weight:Font.Normal;}
            visible:false;
        }

        Text{
            id: loginTextVerifError
            x: 50
            y: 300
            text: qsTr("Failed to login with the provided organization, user, and password.")
            color:"#E96178"
            font { family: montreuxCRg.name; pixelSize: 14; weight:Font.Normal;}
            visible:false;
        }

        Button {
            id: loginNextButton
            x: 338
            y: 411
            width: 171
            height: 42
            text: qsTr("NEXT")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            onClicked: attemptLoginOption1()
            KeyNavigation.tab: loginBackButton
            Keys.onReturnPressed: {
                attemptLoginOption1()
            }

            contentItem: Text {
                color: loginNextButton.down || ! loginNextButton.enabled ? "#82829F" : "#FFFFFF"
                text: loginNextButton.text
                font: loginNextButton.font
                opacity: enabled ? 1.0 : 0.3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: loginNextButton.down ? "#50506E" : "#82829F"
                border.color: loginNextButton.activeFocus ? "#50506E" : "#82829F"
                border.width: 2
                radius: 2
            }
        }

        Button {
            id: loginBackButton
            x: 215
            y: 411
            width: 111
            height: 42
            text: qsTr("BACK")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            onClicked:{
                orgTextInput.enabled = true;
                orgNextButton.enabled = true;
                orgCancelButton.enabled = true;
                loginUsernameInput.focus = true;
                orgTextInput.focus = true;
                organizationBack.start()
            }
            KeyNavigation.tab: loginCancelButton1
            Keys.onReturnPressed: {
                orgTextInput.focus = true;
                organizationBack.start()
            }

            contentItem: Text {
                color: loginBackButton.down || ! loginBackButton.enabled ? "#82829F" : "#FFFFFF"
                text: loginBackButton.text
                font: loginBackButton.font
                opacity: enabled ? 1.0 : 0.3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: loginBackButton.down ? "#50506E" : "#82829F"
                border.color: loginBackButton.activeFocus ? "#50506E" : "#82829F"
                border.width: 2
                radius: 2
            }
        }

        Button {
            id: loginCancelButton1
            x: 92
            y: 411
            width: 111
            height: 42
            text: qsTr("CANCEL")
            onClicked: verifyClose()
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            KeyNavigation.tab: loginUsernameInput
            Keys.onReturnPressed: {
                verifyClose()
            }

            contentItem: Text {
                color: loginCancelButton1.down || ! loginCancelButton1.enabled ? "#82829F" : "#FFFFFF"
                text: loginCancelButton1.text
                verticalAlignment: Text.AlignVCenter
                font: loginCancelButton1.font
                opacity: enabled ? 1.0 : 0.3
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }

            background: Rectangle {
                color: loginCancelButton1.down ? "#50506E" : "#82829F"
                opacity: enabled ? 1 : 0.3
                implicitWidth: 100
                border.color: loginCancelButton1.activeFocus ? "#50506E" : "#82829F"
                implicitHeight: 40
                border.width: 2
                radius: 2
            }
        }

        SequentialAnimation {
            id: organizationBack
            running: false
            PropertyAnimation { target: loginScreen; property: "x"; to: 641; duration: 300; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: organizationScreen; property: "x"; to: 100; duration: 300; easing.type: Easing.InOutQuad}
        }

        SequentialAnimation {
            id: organizationLocationNext
            running: false
            PropertyAnimation { target: loginScreen; property: "x"; to: 641; duration: 300; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: organizationLocationScreen; property: "x"; to: 100; duration: 300; easing.type: Easing.InOutQuad}
        }

    }

    //Organization Location Screen
    Rectangle {
        id:organizationLocationScreen
        x:641
        //                        x:100
        y:0
        width: 540;
        height: 480;
        color: "#ffffff"
        opacity: 1
        visible:true

        Text {
            id: orgLocationTitle
            x: 111
            y: 113
            width: 288
            height: 35
            color: "#50506E"
            text: qsTr("Choose a Location")
            font { family: montreuxCDmBd.name; pixelSize: 30; weight:Font.DemiBold;}
        }


        Text {
            id: loginOrganizationText
            x: 106
            y: 268
            text: qsTr("Organization:")
            color: "#50506E"
            font { family: montreuxCDmBd.name; pixelSize: 15; weight:Font.DemiBold;}
        }

        Text {
            id: element3
            x: 106
            y: 301
            text: qsTr("Organization Name")
            color: "#CDCDDC"
            font { family: montreuxCDmBd.name; pixelSize: 15; weight:Font.DemiBold;}
        }

        Button {
            id: orgLocationNextButton
            x: 262
            y: 350
            width: 245
            height: 42
            text: qsTr("NEXT")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            onClicked: assignLocation()
            KeyNavigation.tab: orgLocationCancelButton
            Keys.onReturnPressed: {
                assignLocation()
            }

            contentItem: Text {
                color: orgLocationNextButton.down ? "#C2C1CD" : "#FFFFFF"
                text: orgLocationNextButton.text
                font: orgLocationNextButton.font
                opacity: enabled ? 1.0 : 0.3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: orgLocationNextButton.down ? "#50506E" : "#82829F"
                border.color: orgLocationNextButton.activeFocus ? "#50506E" : "#82829F"
                border.width: 1
                radius: 2
            }
        }

        Button {
            id: orgLocationCancelButton
            x: 104
            y: 350
            width: 152
            height: 42
            text: qsTr("CANCEL")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            onClicked: verifyClose()
            KeyNavigation.tab: locationChoices
            Keys.onReturnPressed: {
                verifyClose()
            }

            contentItem: Text {
                color: orgLocationCancelButton.down ? "#C2C1CD" : "#FFFFFF"
                text: orgLocationCancelButton.text
                font: orgLocationCancelButton.font
                opacity: enabled ? 1.0 : 0.3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: orgLocationCancelButton.down ? "#50506E" : "#82829F"
                border.color: orgLocationCancelButton.activeFocus ? "#50506E" : "#82829F"
                border.width: 2
                radius: 2
            }
        }

        ComboBox {
            id: locationChoices
            x: 104
            y: 196
            width: 403
            height: 40
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            KeyNavigation.tab: orgLocationNextButton
            Keys.onReturnPressed: {
                assignLocation()
            }

            currentIndex: 0
            model: ListModel {
                id: locationsModel
                ListElement { text: "Location 1" }
                ListElement { text: "Location 2" }
                ListElement { text: "Location 3" }
            }

            background: Rectangle {
                //color: orgTextInput.enabled ? "transparent" : "#50506E"
                border.color: locationChoices.activeFocus ?  "#50506E" : "#C2C1CD"
            }
        }

        SequentialAnimation {
            id: installingNext
            running: false
            PropertyAnimation { target: organizationLocationScreen; property: "x"; to: 641; duration: 300; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: charlie1InstallScreen; property: "x"; to: 100; duration: 300; easing.type: Easing.InOutQuad}
        }

        Component.onCompleted: function(){
            //console.log("Running");
        }

    }

    //Charlie1 install and loading screen.
    Rectangle {
        id:charlie1InstallScreen
        x:641
        //            x:100
        y:0
        width: 540;
        height: 480;
        color: "#ffffff"
        opacity: 1
        visible:true

        Text {
            id: finishUpTitle
            x: 160
            y: 122
            width: 194
            height: 35
            color: "#50506E"
            text: qsTr("Finishing Up")
            font { family: montreuxCDmBd.name; pixelSize: 30; weight:Font.DemiBold;}
        }

        ProgressBar {
            id: progressBar
            x: 59
            y: 284
            width: 415
            height: 23
            value: 0
        }

        Component.onCompleted: function(){

        }

    }

    //Cancel Dialog
    Dialog {
        width: 450
        height: 200
        x:(parent.width-450)/2
        y:(parent.height-200)/2
        opacity: 1
        id: cancelInstallDialog
        standardButtons: Dialog.Ok | Dialog.Cancel

        Rectangle {
            x:0
            y:0
            width: parent.width;
            height: parent.height;
            color: "#ffffff"
            opacity: 1

            Text {
                x: 0
                y: 0
                color: "#50506E"
                width: parent.width;
                height: parent.height;
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("Are you sure you want to cancel NAVS installation?")
                font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            }
        }

        onAccepted: function(){
            root.allowClose = true;
            navs.close();
        }
        modal:true
    }

    SequentialAnimation {
        id: finishUpBar
        running: false
        PauseAnimation { duration: 1000 }
        PropertyAnimation { target: progressBar; property: "value"; to: 1; duration: 1000; easing.type: Easing.InOutQuad}
        PropertyAnimation { target: root; property: "visible"; to: false; duration: 300; easing.type: Easing.InOutQuad}

        ParallelAnimation{
            PropertyAnimation { target: navs; properties: "x"; to: getFullStartupX(); duration: 500; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: navs; properties: "y"; to: getFullStartupY(); duration: 500; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: navs; properties: "width"; to: getFullStartupWidth(); duration: 500; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: navs; properties: "height"; to: getFullStartupHeight(); duration: 500; easing.type: Easing.InOutQuad}
        }
    }
}
