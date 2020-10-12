import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12

//Startup
Item {
    id: root
    width: 640
    height: 480
    visible:true

    property string currentVersion : "1.0.29";
    property bool checkUpdate : true;

    signal goFullNavs

    property bool allowClose: false

    function verifyClose(){
        cancelInstallDialog.open();
    }

    //It would be nice to pop a window for logs if in the test env.
    function writeLog(logItem){ //Used only in testing.
        //orgTextInput.text = logItem;
    }

    function attemptLoginOption1(){
        //console.log("Attempting Login");

        loginUsernameInput.enabled = false;
        loginPasswordInput.enabled = false;
        loginNextButton.enabled = false;
        loginBackButton.enabled = false;
        loginCancelButton1.enabled = false;
        passwordB.enabled = false;

        if(loginUsernameInput.text.length > 0 && loginPasswordInput.text.length > 0){

            //Login using username and password
            backend.loginUsername(loginUsernameInput.text, loginPasswordInput.text);

            //Check if successful or fail
            if(backend.loginData["session_key"]) {
                if(!backend.isCUAdmin()){
                    loginUsernameInput.enabled = true;
                    loginPasswordInput.enabled = true;
                    passwordB.enabled = true;
                    loginNextButton.enabled = true;
                    loginBackButton.enabled = true;
                    loginCancelButton1.enabled = true;
                    loginTextVerifError.visible = false;
                    loginTextMissingError.visible = false;
                    loginNonAdminError.visible = true;
                    loginUsernameInput.focus = true;
                }else{
                    //Check if organization location more than one
                    if (backend.organizationData.count === 1){
                        locationChoices.model = backend.organizationData;
                        assignLocation();
                    }else{
                        loginTextMissingError.visible = false;
                        loginTextVerifError.visible = false;
                        organizationLocationNext.start();
                        locationChoices.focus = true;
                        locationChoices.model = backend.organizationData;
                        //console.log(locationChoices.model.get(0).location);
                        element3.text = backend.getOrgName();
                    }
                }
            }else{
                loginUsernameInput.enabled = true;
                loginPasswordInput.enabled = true;
                loginNextButton.enabled = true;
                loginBackButton.enabled = true;
                loginCancelButton1.enabled = true;
                loginTextVerifError.visible = true;
                loginTextMissingError.visible = false;
                loginNonAdminError.visible = false;
                loginUsernameInput.focus = true;
                passwordB.enabled = true;
            }
        }else{
            loginUsernameInput.enabled = true;
            loginPasswordInput.enabled = true;
            loginNextButton.enabled = true;
            loginBackButton.enabled = true;
            loginCancelButton1.enabled = true;
            loginTextMissingError.visible = true;
            loginNonAdminError.visible = false;
            loginTextVerifError.visible = false;
            loginUsernameInput.focus = true;
            passwordB.enabled = true;
        }
    }

    function assignLocation(){

        if(locationChoices.currentText == "Select a Location"){
            return;
        }
        //console.log("Assigning Organization Location");
        installingNext.start();
        backend.sendInstallationDataToBeta1(locationChoices.currentText, locationChoices.currentIndex-1);
        backend.installCharlie2();
        finishUpBar.start();
        timer.setTimeout(function(){
            launchFullNavs();
        }, 2300);

    }

    function organizationDNSKeyLoaded(){
        //console.log("Called");
        if(organizationScreen.visible == true && !backend.loginData["session_key"]){
            //console.log("visible");
            //Check for dns key
            if (backend.organizationDnsKeyStatus) {
                orgTextInputError.visible = false;
                orgTextValueError.visible = false;
                loginUsernameInput.focus = true;
                loginNext.start();
            }
            else {
                orgTextInput.enabled = true;
                orgNextButton.enabled = true;
                orgCancelButton.enabled = true;
                orgTextInputError.visible = false;
                orgTextValueError.visible = true;
                orgTextInput.focus = true;
            }
        }
    }

    function ckeckOrganization(){
        orgTextInput.enabled = false;
        orgNextButton.enabled = false;
        orgCancelButton.enabled = false;
        loginUsernameInput.focus = true;

        if(orgTextInput.text.length > 0){
            //Check if exist then load organization
            backend.checkOrganizationDnsKeyStatus(orgTextInput.text);
            //console.log(backend.organizationDataCode);
        }else{
            orgTextInput.enabled = true;
            orgNextButton.enabled = true;
            orgCancelButton.enabled = true;
            orgTextValueError.visible = false;
            orgTextInputError.visible = true;
            orgTextInput.focus = true;
        }
    }

    function goVersionCheck(versionAvailable){
        if(versionAvailable !== ""){
            //Check the versions.
            var isNewVersion = versionCompare(versionAvailable, currentVersion);
            if(isNewVersion === 1){
                if(backend.loginData["session_key"]){
                    navsFull.prepMinimized();
                    navs.flags = Qt.FramelessWindowHint;
                    navs.width = 640;
                    navs.height = 480;
                    handle.visible = false;
                    navs.x = Screen.width / 2 - 320;
                    navs.y = Screen.height / 2 - 240;
                    navsFull.visible = false;
                    startupContainer.visible = true;
                }
                availableVersion.text = versionAvailable;
                versionWindowOverlay.visible = true;
                launchUpdate.enabled = true;
                launchUpdateLater.enabled = true;
                versionAnimation.start();
            }else{
                if(!backend.loginData["session_key"]) {
                    continueStartup();
                }
            }
        }else{
            if(!backend.loginData["session_key"]) {
                continueStartup();
            }
        }
    }

    function updateCompleted(){
        root.allowClose = true;
        navs.close();
    }

    function continueStartup(){
        if(backend.registerStatus){
            //console.log("NAVS is already installed");

            //We'll show a message that tells the user we're logging in. (Installation ID, Windows Username, Organization ID)
            //splashLoadingText.text = "Logging in to NAVS...";
            backend.loginWindowsUsername();

            if (backend.loginData["session_key"]){
                //Http Status Code
                //console.log("backend.loginWindowsUsernameDataCode = " + backend.loginWindowsUsernameDataCode);
                launchFullNavs();
            }
            else {//If the login Fails. Show an error message.
                splashLoadingText.text = "Automatic login failed.";
                //For logs
                //backend.createLog();
                loginSecondary.visible = true;
            }
        }else{
            //console.log("NAVS is not installed");
            splashOut.start();
        }
    }

    function registerCheckCompleted(){
        //Check most recent available version number
        if(checkUpdate){
            backend.getAvailableVersionNumber();
        }else{
            continueStartup();
        }
    }

    function versionCompare(v1, v2, options) {
        var lexicographical = options && options.lexicographical,
            zeroExtend = options && options.zeroExtend,
            v1parts = v1.split('.'),
            v2parts = v2.split('.');

        function isValidPart(x) {
            return (lexicographical ? /^\d+[A-Za-z]*$/ : /^\d+$/).test(x);
        }

        if (!v1parts.every(isValidPart) || !v2parts.every(isValidPart)) {
            return NaN;
        }

        if (zeroExtend) {
            while (v1parts.length < v2parts.length) v1parts.push("0");
            while (v2parts.length < v1parts.length) v2parts.push("0");
        }

        if (!lexicographical) {
            v1parts = v1parts.map(Number);
            v2parts = v2parts.map(Number);
        }

        for (var i = 0; i < v1parts.length; ++i) {
            if (v2parts.length === i) {
                return 1;
            }

            if (v1parts[i] === v2parts[i]) {
                continue;
            }
            else if (v1parts[i] > v2parts[i]) {
                return 1;
            }
            else {
                return -1;
            }
        }

        if (v1parts.length !== v2parts.length) {
            return -1;
        }

        return 0;
    }

    function launchFullNavs(){

        //Load Profile
        backend.loadProfile();
        //Load Welcome HTML screen
        backend.loadWelcome();

        //Get the me data
        backend.loadMeData();

        //Fill the list of All Applications in the NavsFull.qml appList Rectangle.
        if(!backend.isCUAdmin()) {
            //Load Applications and Quick App
            backend.loadApps();
            /*Hi Jon, I don't think it neccessary following codes to use,
            because of code #383 and #388, appDataChanged and appQuickDataChanged, right?
            navsFull.gridAppList = backend.appData;
            navsFull.gridAppQuickList = backend.appQuickData;
            */
            appBadgeTimer.running = true;
            checkAllAppBadges();

            //Send Session key to server
            //backend.sendSessionKeyServer();
            backend.startInitialSocketConnection();
        }

        navsFull.openWelcomeScreen();

        //Hide the whatever loading screen is visible
        root.visible = false;

        //Start the Full Navs Animation
        navs.flags = Qt.Window;
        fullNavsAnimation.start();

        root.goFullNavs();

        navs.showHandle();
    }

    Timer {
        id: appBadgeTimer
        interval: 30000; running: false; repeat: true
        onTriggered: {
           checkAllAppBadges();
        }
    }

    function checkAllAppBadges(){
        if(backend.loginData["session_key"]) {
            //console.log("Checking all app badges.");
            backend.updateAllAppBadges();
        }
    }

    function showErrorPop(errId){
        if(!errorDialog.visible){
            if(errId === 1011){
                errorTxt.text = "It looks like you're not connected to the internet.  Please check your connection and try again.";
                errorDialog.visible = true;
            }else if(errId === 1012){
//                errorTxt.text = "We're having trouble connecting to the NAVS service. We're very sorry for the inconvenience. Please login to attempt to re-connect.";
//                navsFull.showSecondaryLogin(false);
//                errorDialog.visible = true;
            }else if(errId === 1013){
                if(!isSecondaryLoginVisible){
                    errorTxt.text = "Your NAVS session is no longer valid.  Please log in again to continue.";
                    navsFull.showSecondaryLogin(false);
                    errorDialog.visible = true;
                }
            }
        }
    }

    //This is how we connect backend signals to this QML object
    Connections {
        target: backend
        onRegisterStatusChanged: {
            registerCheckCompleted();
        }
        onNotificationsDataChanged: {
            navsFull.notificationsLoaded();
        }
        onMeDataChanged: {
            navsFull.loadMeData();
        }
        onOrganizationDataChanged:{
            navsFull.loadOrganizationData();
        }
        onNewNotificationConveyer:{
            navsFull.caughtNotification(new_notification);
        }
        onVersionInfoFound:{
            goVersionCheck(versionInfo);
        }
        onUpdateCompleted:{
            updateCompleted();
        }
        onOrganizationDnsKeyStatusChanged:{
            organizationDNSKeyLoaded();
        }
        onWriteQMLLogItem:{
            writeLog(logItem);
        }
        onTotalBadgeCountChanged:{
            handle.updateTotalBadgeCount(totalBadgeCount);
        }
        onErrorMessageChanged:{
            showErrorPop(errorId);
        }
        onAppDataChanged: {
            if(!backend.isCUAdmin()) {
                navsFull.setAppModels(backend.getAppCategories());
            }
        }
        onAppQuickDataChanged: {
            if(!backend.isCUAdmin()) {
                navsFull.gridAppQuickList = backend.appQuickData;
                handle.quickApllList = backend.appQuickData;
            }
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

        Image{
            source:"images/logoRed.png"
            width:408
            height:217
            fillMode: Image.PreserveAspectFit
            x: 30
            y: 45
        }

        TextArea {
            id: splashNavsDisclosures
            x: 87
            y: 275
            width: 354
            color: "#82829f"
            height: 81
            text: qsTr(currentVersion+ " Beta Release\n\n@ 2020 CU*NorthWest and its licensors. All Rights Reserved.")
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font { family: montreuxCRg.name; pixelSize: 13; weight:Font.Normal;}
        }

        Text {
            id: splashLoadingText
            x: 198
            y: 435
            width: 111
            height: 49
            color: "#e695a3"
            text: qsTr("Loading...")
            horizontalAlignment: Text.AlignHCenter
            font { family: montreuxCRg.name; pixelSize: 13; weight:Font.Normal;}
        }

        Button{
            id: loginSecondary
            y:375
            x:160
            text: qsTr("Login with username and password")
            onClicked: {
               navs.showSecondaryLogin("startup", false);
            }
            visible: false;
        }

        Component.onCompleted: function(){
          backend.checkRegisterStatus();
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

    Rectangle{
        id:versionWindowOverlay
        x:0
        y:0
        width: 640;
        height: 480;
        color:  "#999999";
        opacity: 0.9;
        visible: false;
    }

    Rectangle {
        id: versionWindow
        x:(parent.width/2)-210
        y:parent.height
        width: 420;
        height: 300;
        border.color:  "#50506E";
        border.width: 2;
        color: "#50506e"

        Button {
            id: launchUpdate
            x: 273
            y: 242
            text: qsTr("Update Now")
            onClicked: {
                launchUpdate.enabled = false;
                launchUpdateLater.enabled = false;
                backend.launchUpdate();
            }
        }

        Button {
            id: launchUpdateLater
            x: 143
            y: 242
            text: qsTr("Update Later")
            onClicked: {
                launchUpdate.enabled = false;
                launchUpdateLater.enabled = false;
                backend.startUpdateClock();
                versionWindow.x = versionWindow.parent.height;
                versionWindow.visible = false;
                versionWindowOverlay.visible = false;
                continueStartup();
            }
        }

        Text {
            id: installedVersionText
            x: 94
            y: 119
            width: 124
            height: 27
            color: "#ffffff"
            text: qsTr("Installed Version: ")
            horizontalAlignment: Text.AlignLeft
            font.bold: false
            font.pixelSize: 16
        }

        Text {
            id: availableVersionText
            x: 94
            y: 146
            width: 133
            height: 27
            color: "#ffffff"
            text: qsTr("Available Version:")
            horizontalAlignment: Text.AlignLeft
            font.bold: false
            font.pixelSize: 16
        }

        Text {
            id: versionWarning
            x: 27
            y: 210
            width: 380
            height: 39
            color: "#ffffff"
            text: qsTr("NAVS will update automatically in less than 24hrs. Or you can choose to update now.")
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }

        Text {
            id: installedVersion
            x: 230
            y: 119
            width: 124
            height: 27
            color: "#ffffff"
            text: qsTr(currentVersion)
            horizontalAlignment: Text.AlignLeft
            font.bold: false
            font.pixelSize: 16
        }

        Text {
            id: availableVersion
            x: 230
            y: 146
            width: 124
            height: 27
            color: "#ffffff"
            text: qsTr("1.0.0")
            horizontalAlignment: Text.AlignLeft
            font.bold: false
            font.pixelSize: 16
        }

        Rectangle {
            id: updateMessageBg
            x: 3
            y: 3
            width: 413
            height: 79
            color: "#ffffff"

            Text {
                id: updateMessage
                x: 0
                y: 23
                width: 412
                height: 53
                color: "#50506e"
                text: qsTr("A new version of NAVS Desktop is available.")
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 17
            }
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

        Text{
            id: orgTextValueError
            x: 60
            y: 250
            text: qsTr("Invalid NAVS organization domain")
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
                visible: false;
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

        Text{
            id: loginNonAdminError
            x: 70
            y: 300
            text: qsTr("Only CU*NorthWest technicians can install NAVS.")
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
            //text: organizationName()
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
            background: Rectangle {
                border.color: locationChoices.activeFocus ?  "#50506E" : "#C2C1CD"
            }

            textRole: "dataObject.location"

            delegate: ItemDelegate {
                text: dataObject.location
            }
        }

        SequentialAnimation {
            id: installingNext
            running: false
            PropertyAnimation { target: organizationLocationScreen; property: "x"; to: 641; duration: 300; easing.type: Easing.InOutQuad}
            PropertyAnimation { target: charlie1InstallScreen; property: "x"; to: 100; duration: 300; easing.type: Easing.InOutQuad}
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

    //Cancel Dialog
    Dialog {
        width: 450
        height: 200
        x:(parent.width-450)/2
        y:(parent.height-200)/2
        opacity: 1
        id: errorDialog
        standardButtons: Dialog.Ok

        Rectangle {
            x:0
            y:0
            width: parent.width;
            height: parent.height;
            color: "#ffffff"
            opacity: 1

            Text {
                id:errorTxt
                x: 0
                y: 0
                color: "#50506E"
                width: parent.width;
                height: parent.height;
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font { family: montreuxCRg.name; pixelSize: 20; weight:Font.Normal;}
            }
        }

        onAccepted: function(){
            //
        }
        modal:true
    }

    SequentialAnimation {
        id: finishUpBar
        running: false
        PauseAnimation { duration: 1000 }
        PropertyAnimation { target: progressBar; property: "value"; to: 1; duration: 1000; easing.type: Easing.InOutQuad}
    }

    ParallelAnimation{
        id: fullNavsAnimation
        running: false
        PropertyAnimation { target: navs; properties: "x"; to: getFullStartupX(); duration: 150; easing.type: Easing.InOutQuad}
        PropertyAnimation { target: navs; properties: "y"; to: getFullStartupY(); duration: 150; easing.type: Easing.InOutQuad}
        PropertyAnimation { target: navs; properties: "width"; to: getFullStartupWidth(); duration: 150; easing.type: Easing.InOutQuad}
        PropertyAnimation { target: navs; properties: "height"; to: getFullStartupHeight(); duration: 150; easing.type: Easing.InOutQuad}
        onFinished: {
            navsFull.isStartingUp = false;
        }
    }

    ParallelAnimation{
        id: versionAnimation
        running: false
        PropertyAnimation { target: versionWindow; properties: "y"; to: (versionWindow.parent.height/2)-150; duration: 250; easing.type: Easing.InOutQuad}
    }


}
