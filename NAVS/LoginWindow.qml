import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: loginScreen
    width: 640
    height: 480
    visible:false

    property string sourceComponent: "";
    property string previousSessionKey : "";

    function cancelAction(){
        loginScreen.visible = false;
        if(sourceComponent == "navsFull"){
            navs.flags = Qt.Window;
            navs.restorenavsFullFromLogin();
        }else{
            startupContainer.visible = true;
        }
    }

    function resetValues(){
        loginUsernameInput.enabled = true;
        loginPasswordInput.enabled = true;
        passwordB.enabled = true;
        loginNextButton.enabled = true;
        loginCancelButton1.enabled = true;
        loginTextVerifError.visible = false;
        loginTextMissingError.visible = false;
        loginUsernameInput.focus = true;
        loginUsernameInput.text = "";
        loginPasswordInput.text = "";
        loginUsernameInput.focus = true;
        passwordB.visible = false;
    }

    function attemptLoginOption1(){
        console.log("Attempting Login");

        loginUsernameInput.enabled = false;
        loginPasswordInput.enabled = false;
        loginNextButton.enabled = false;
        loginCancelButton1.enabled = false;
        passwordB.enabled = false;

        if(loginUsernameInput.text.length > 0 && loginPasswordInput.text.length > 0){
            //Store the old session key if there is one to make sure we changed it.
            if(backend.loginData["session_key"]){
                previousSessionKey = backend.loginData["session_key"];
            }

            //Login using username and password
            backend.loginUsernameSecondary(loginUsernameInput.text, loginPasswordInput.text);

            //Check if successful or fail
            if(backend.loginData["session_key"] && backend.loginData["session_key"] !== previousSessionKey) {
                console.log("logged in");
                if(sourceComponent == "navsFull"){
                    navsFull.closeAllApps();
                    navsFull.clearAppCategories();
                }
                loginScreen.visible = false;
                navs.flags = Qt.Window;
                startupContainer.launchFullNavs();
            }else {
                console.log("Login failed");
                loginUsernameInput.enabled = true;
                loginPasswordInput.enabled = true;
                loginNextButton.enabled = true;
                loginCancelButton1.enabled = true;
                loginTextVerifError.visible = true;
                loginTextMissingError.visible = false;
                loginUsernameInput.focus = true;
                passwordB.enabled = true;
            }
        }else{
            loginUsernameInput.enabled = true;
            loginPasswordInput.enabled = true;
            loginNextButton.enabled = true;
            loginCancelButton1.enabled = true;
            loginTextMissingError.visible = true;
            loginTextVerifError.visible = false;
            loginUsernameInput.focus = true;
            passwordB.enabled = true;
        }
    }

    function setCancelEnabled(enbVal){
        loginCancelButton1.enabled = enbVal;
    }

    //Particle Background
    InstallerBackground{
        id:installerBackground
    }

    //Splash screen
    Rectangle {
        id:splashScreen
        x:100
        y:0
        width: 540;
        height: 480;
        color: "#ffffff"
        opacity: 0.9
        visible:true

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


        Rectangle {
            x: 114
            y: 234
            width: 249
            height: 42
            border.color: loginPasswordInput.activeFocus || passwordB.activeFocus ?  "#50506E" : "#C2C1CD"
            color: loginPasswordInput.enabled || passwordB.enabled ? "transparent" : "#C2C1CD"

            TextField {
                id: loginPasswordInput
                x: 0
                z: 2
                width: 199
                height: 38
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
                echoMode: TextInput.Password
                KeyNavigation.tab: loginNextButton
                Keys.onReturnPressed: {
                    attemptLoginOption1()
                }
                onTextChanged: {
                    if (loginPasswordInput.length > 0)
                        passwordB.visible = true;
                    else
                        passwordB.visible = false;
                }

                background: Rectangle {
                    color: loginPasswordInput.enabled ? "transparent" : "#C2C1CD"
                }
            }

            Button {
                id: passwordB
                z: 2
                x: 195
                y: 2
                width: 50
                height: 38
                font.pixelSize: 12
                font.family: montreuxCDmBd.name
                text: "Show"
                contentItem: Text {
                    text: passwordB.text
                    font: passwordB.font
                    opacity: enabled ? 1.0 : 0.3
                    color: passwordB.down ?  "#C2C1CD" : "Black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                onClicked: {
                    if (loginPasswordInput.echoMode === TextInput.Normal) {
                        loginPasswordInput.echoMode = TextInput.Password;
                        passwordB.text = "Show";
                        loginPasswordInput.focus = true;
                    }
                    else {
                        loginPasswordInput.echoMode = TextInput.Normal;
                        passwordB.text = "Hide";
                        loginPasswordInput.focus = true;
                    }
                }

                background: Rectangle {
                    color: passwordB.enabled ? "transparent" : "#C2C1CD"
                }

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
            text: qsTr("LOGIN")
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            onClicked: {
                attemptLoginOption1()
            }
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
            id: loginCancelButton1
            x: 215
            y: 411
            width: 111
            height: 42
            text: qsTr("CANCEL")
            onClicked: {
                cancelAction();
            }
            font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            KeyNavigation.tab: loginUsernameInput
            Keys.onReturnPressed: {
                cancelAction()
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
    }
}
