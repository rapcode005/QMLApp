import QtQuick 2.14
import QtQuick.Window 2.12
import QtQuick.Particles 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import FontAwesome 1.0
import QtQml.Models 2.12
import QtWebEngine 1.10
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.14

GridLayout {
    id : root
    rowSpacing: 0
    columnSpacing: 0
    anchors.fill: parent
    rows    : 1
    columns : 2
    visible: false;

    property var webEngs: [];
    property alias gridAppQuickList: appQuickRepeater.model;
    property alias gridTabsRoot: tabsRoot;
    property alias titleList: visualModel.model;
    property alias colorModel: colorModel;
    property var indexCurDrag;
    property var appID;
    property bool dragStatus: false
    property var removeAppQuick;
    property var dragGridview: "AppQuick";
    property var deleteOrNot: false;
    property var activeText: "";
    property var webCont;
    property var newList: false;
    property var folderLoc: "";
    property variant notif;
    property variant notifComponent : Qt.createComponent("NavsNotification.qml");
    property variant notifQue : [];
    property bool notifsReady : false;
    property bool isStartingUp : true;
    property var devObj: [];
    property variant webDev;
    property variant appCategories : [];
    property variant component

    Component.onCompleted: {
        notif = notifComponent.createObject(root);
        notif.x = Screen.virtualX + Screen.width - notif.width;
        notifsReady = true;
        if(notifQue.length > 0){
            for(var i=notifQue.length-1; i>=0; i--){
                notif.createNotification(notifQue[i], i);
            }
        }
    }

    function establishAppHeightBasedOnTextLength(len){
        if(len <= 12){
            return 100;
        }else if(len <= 18){
            return 110;
        }else{
            return 125;
        }
    }

    function notificationDisplayed(notifObject){
        notifQue.pop();
        backend.updateNotificationDeliveryDate(notifObject);
    }

    function caughtNotification(notifObject){
        notifQue.push(notifObject);
        if(notifsReady){
            if(isStartingUp){
                var t = notifQue.length;
                timer.setTimeout(function(){
                    prepNotifWindow();
                    notif.createNotification(notifObject, t);
                },5000);
            }else{
                prepNotifWindow();
                notif.createNotification(notifObject, notifQue.length);
            }
        }
    }

    function prepNotifWindow(){
        notif.y = Screen.virtualY + 50;
        notif.width = 300;
        notif.height = 170;
        notif.x = Screen.virtualX + Screen.width - notif.width;
    }

    function notificationsLoaded(){
        notificationsList.model = backend.notificationsData;
        notificationsList.forceLayout();
    }

    function restoreWindow(){
        navs.flags = Qt.Window;
        root.visible = true;
        navs.x = getFullStartupX();
        navs.y = getFullStartupY();
        navs.height = getFullStartupHeight();
        navs.width = getFullStartupWidth();
        notif.x = Screen.virtualX + Screen.width - notif.width;
    }

    function goBig(aType, appId){
        navs.visible = true;
        //restoreWindow();

        if(aType === 1){
             openApp(appId);
        }else if(aType === 2){ //All Apps
            if(!appList.opened){
                timer.setTimeout(function(){
                    appList.y = appList.parent.height;
                    appList.visible = true;
                    showAppListAnimation.start();
                    appList.opened = true;
                }, 250);
            }
        }
    }

    function startUp(){
        navs.flags = Qt.Window;
        root.visible = true;
        if(backend.isTestEnv()){
            navs.title = "NAVS - Development Environment";
        }else{
            navs.title = "NAVS - Production Environment";
        }
    }

    function switchTabs(tabIndex){
        for(var i=0; i<webEngs.length; i++){
            if(i===tabIndex){
                webEngs[i].visible = true;
                //Set DevToolsview
                if (devToolsNavsCont.visible == true) // I forgot to add this code.
                    openDevTools(webEngs[i]);
                topColorBar.color = colorModel.get(tabIndex).color;
            }else{
                webEngs[i].visible = false;
            }
        }
    }

    /*
    property QtObject defaultProfile: WebEngineProfile {
        storageName: "Profile"
        offTheRecord: false
        useForGlobalCertificateVerification: true
    }
    */

    function checkApp(appID) {
        for(var i = 0; i < colorModel.count; i++) {
            if(colorModel.get(i).appId === appID) {
                return i;
            }
        }

        return -1;
    }

    function showSecondaryLogin(showCancel){
        userDrawer.close();
        closeAppListFast();
        navs.showSecondaryLogin("navsFull", showCancel);
    }

    function openAdministration(adminUrl){
        //var component;

        //console.log(adminUrl);

        //Name of the profile is combination of Title plus appId
        //defaultProfile.storageName = "admin-1"; // without sessionKey needed

        component = Qt.createComponent("WebContainer.qml");
        webCont = component.createObject(engineContainer, {ids: -1,
                                             url:adminUrl//,
                                             //profile: defaultProfile
                                         });

        if(component.status !== Component.Ready )
        {
            if( component.status === Component.Error )
                console.debug("Error:"+ component.errorString() );
            return; // or maybe throw
        }
        else {

            //Append the instance to the webEngs List
            var newLen = webEngs.length;
            webEngs.push(webCont);
            webEngs[newLen].visible = true;

            //Append the color model
            colorModel.append({"text":"Administration",
                                        "color":"#50506E",
                                        "offColor":"#CDCDDC",
                                         "appId": -1 });

            //Hide the applications screen
            tabsContainer.activeTab = newLen;
            switchTabs(newLen);
            if(appList.opened){
                appList.visible = false;
                appList.y = appList.parent.y;
                appList.opened = false;
            }

            //Hide all other web engines
            for(var ih=0; ih<newLen; ih++){
                    webEngs[ih].visible = false;
            }
        }
    }

    function getTempWelcomeURL(){
        var profileTypeId = backend.getProfileTypeId();
        if(profileTypeId === 1){
            return "https://cunorthwest.com/placeholder-one/";
        }else if(profileTypeId === 2){
            return "https://cunorthwest.com/placeholder-two/";
        }else{
            return "https://cunorthwest.com/placeholder-three/";
        }
    }

    function openWelcomeScreen(){
        //var component;

        //Name of the profile is combination of Title plus appId
        //defaultProfile.storageName = "welcome-2";// without sessionKey needed
        backend.profileData.storageName = "welcome-2";
        component = Qt.createComponent("WebContainer.qml");
        // Assign App ID can be done here
        webCont = component.createObject(engineContainer, {ids: -2,
                                            profile: backend.profileData,
                                            //url:backend.welcomeData //---Get from Beta1
                                             url:getTempWelcomeURL()
                                          });

        if(component.status !== Component.Ready ) {
            if( component.status === Component.Error )
                console.debug("Error:"+ component.errorString() );
            return; // or maybe throw
        }
        else {
            //Append the instance to the webEngs list
            webEngs.push(webCont);

            //Append the color model
            navsFull.colorModel.append({"text":"Welcome", "color":"#50506E",
                                                  "offColor":"#CDCDDC", "appId": -2  });
        }
    }

    function closeAppListFast(){
        appList.visible = false;
        appList.y = appList.parent.y;
        appList.opened = false;
    }

    function prepMinimized(){
        root.visible = false;
        userDrawer.close();
    }

    function lauchAppFromNotification(appId, targetUrl){
        if(root.visible == false){
            navsFull.visible = true;
            //restoreWindow();
        }

        var url = targetUrl;
        var result = checkApp(appId);

        //Check if not exist
        if(result === -1) {
            openNewAppByAppIdandURL(appId, url);
        }
        else {
            tabsContainer.activeTab = result;
            switchTabs(result);
            webEngs[result].url = url;
        }

        userDrawer.close();
        closeAppListFast();
    }

    function openApp(appId){

        //console.log(appId);

        var result = checkApp(appId);
        if(result === -1) {
            var url = backend.getAppURL(appId);
            openNewAppByAppIdandURL(appId, url);
        }else{
            tabsContainer.activeTab = result;
            switchTabs(result);
        }
        closeAppListFast();
    }

    function openNewAppByAppIdandURL(appId, url){
        //var component;
        var newTitle = backend.getAppTitle(parseInt(appId));

        //Name of the profile is combination of title plus appId
        backend.profileData.storageName = newTitle + appId;

        component = Qt.createComponent("WebContainer.qml");
        webCont = component.createObject(engineContainer, {ids: appId,
                                        url: url,
                                        profile: backend.profileData});

        if(component.status !== Component.Ready ) {
            if( component.status === Component.Error )
                console.debug("Error:"+ component.errorString() );
            return; // or maybe throw
        }
        else {
            if (webCont !== null) {
                 webEngs.push(webCont);
                 for(var i=0; i<webEngs.length; i++){
                     webEngs[i].visible = true;
                 }
             }

             colorModel.append({"text":newTitle,
                                         "color":"#50506E",
                                         "offColor":"#CDCDDC",
                                          "appId": appId });

            tabsContainer.activeTab = webEngs.length-1;
            switchTabs(webEngs.length-1);
        }
    }

    //completely destroy everything related to this webengine and the tab
    function deleteApp(index) {
        var cnt = colorModel.count - 1;
        //I used this when exchanging tab and it seems colorModel is not updating value right awa. This is strange.
        var indexColor = checkApp(webEngs[index].ids);
        webEngs[index].destroy();
        if(cnt !== index) {
            webEngs.splice(index, 1);
            colorModel.remove(indexColor);
        }
        else if(webEngs.length <= 1) {
            webEngs.pop();
            colorModel.remove(indexColor);
            devToolsNavsCont.visible = false;
            if (devObj.length > 0) {
                devObj[0].destroy();
                devObj.pop();
            }
        }
        else {
            webEngs.pop();
            colorModel.remove(indexColor);
        }
    }

    function openDevTools(item) {

        var devComponent = Qt.createComponent("DevTools.qml");

        webDev = devComponent.createObject(devToolsNavsCont);

        if(devComponent.status !== Component.Ready ) {
            if( devComponent.status === Component.Error )
                console.debug("Error:"+ devComponent.errorString() );
            return; // or maybe throw
        }
        else {
            webDev.inspectedView = item;
            if (devObj.length == 0)
                devObj.push(webDev);
            else {
                devObj[0].destroy();
                devObj.pop();
                devObj.push(webDev);
            }

        }
    }

    function closeAllApps(){
        for(var i=webEngs.length-1; i >= 0; i--){
            deleteApp(i);
        }
    }

    function clearAppCategories(){
        appcategoriesModel.clear();
        backend.deleteAllCategoryProxyModels();
    }

    function loadMeData(){
        //backend.meData has the following data properties.
        //[created_at,deleted_at,email,email_verified_at,full_name,id,isSuperAdmin,organizationUser,phone,status,timezone,updated_at,user_type,user_type_id,username]
        if(backend.meData.user_type && backend.meData.user_type.name){
            profileTypeName.text = backend.meData.user_type.name.replace("_"," ");
        }else if(backend.meData.isSuperAdmin === 1){
             profileTypeName.text = "CU Northwest Admin";
        }
        if(backend.meData.organizationUser){
            var profilePhoto =  backend.meData.organizationUser.profile_photo;
            if(profilePhoto && profilePhoto.length > 0){
                userImage.source = profilePhoto;
                userImage.visible = true;
                replacementUserIcon.visible = false;
            }else{
                userImage.visible = false;
                replacementUserIcon.visible = true;
            }
            organizationUserDisplayNameValue.text = clipLongDisplayNames(backend.meData.organizationUser.display_name);
        }else{
            userImage.visible = false;
            replacementUserIcon.visible = true;
            organizationUserDisplayNameValue.text = clipLongDisplayNames(backend.meData.full_name);
        }
    }

    function clipLongDisplayNames(displayName){
        if(displayName.length > 30){
            return displayName.substring(0,30) + '...';
        }
        return displayName;
    }

    function loadOrganizationData(){
        organizationNameValue.text = backend.getOrgName();
        var orgIcon = backend.getOrgIcon();
        if(orgIcon.length > 0){
            organizationImage.source = orgIcon;
            organizationImage.visible = true;
            replacementOrganizationIcon.visible = false;
        }else{
            organizationImage.visible = false;
            replacementOrganizationIcon.visible = true;
        }
    }

    function setAppModels(tms){
        for(var i=0; i<tms.length; i++){
            appcategoriesModel.append({"category":tms[i]});
        }
        root.appCategories = tms;
    }

    ListModel {
        id: appcategoriesModel
//        ListElement { catehory: "Name Here" }
    }

    DelegateModel { id: appVisualModel; model: appcategoriesModel; delegate:  AppDelegate {} }

    Drawer {
        id: userDrawer
        width: 400
        height: root.height

        Rectangle{
            color:"#E9E9E9"
            x:1
            y:1

            width:parent.width-2
            height:parent.height-2

            Text {
                id: organizationUserDisplayNameValue
                x: 81
                y: 12
                width: 307
                height: 58
                text: qsTr("Organization User Display Name")
                font.pixelSize: 18

                Button {
                    id: switchLoginButton
                    x: 147
                    y: 65
                    width: 149
                    height: 29
                    text: qsTr("Login as a different user")
                    onClicked: {
                        showSecondaryLogin(true);
                    }
                }
            }

            Image {
                id: userImage
                x: 11
                y: 12
                width: 64
                height: 58
                fillMode: Image.PreserveAspectFit
                visible:false

                property bool rounded: true
                property bool adapt: false

                layer.enabled: rounded
                layer.effect: OpacityMask {
                    maskSource: Item {
                        width: userImage.width
                        height: userImage.height
                        Rectangle {
                            anchors.centerIn: parent
                            width: userImage.adapt ? userImage.width : Math.min(userImage.width, userImage.height)
                            height: userImage.adapt ? userImage.height : width
                            radius: Math.min(width, height)
                        }
                    }
                }
            }

            Text {
                id:replacementUserIcon
                x: 11
                y: 12
                width:64
                height:58
                text: FontAwesome.user
                font.family: FontAwesome.fontFamily
                font.pixelSize: 72
            }


            Frame {
                id: frame
                x: 22
                y: 115
                width: 355
                height: 80

                Image {
                    id: organizationImage
                    x: -1
                    y: 7
                    width: 47
                    height: 41
                    fillMode: Image.PreserveAspectFit
                    visible:false;

                    property bool rounded: true
                    property bool adapt: true

                    layer.enabled: rounded
                    layer.effect: OpacityMask {
                        maskSource: Item {
                            width: organizationImage.width
                            height: organizationImage.height
                            Rectangle {
                                anchors.centerIn: parent
                                width: organizationImage.adapt ? organizationImage.width : Math.min(organizationImage.width, organizationImage.height)
                                height: organizationImage.adapt ? organizationImage.height : width
                                radius: 16
                            }
                        }
                    }
                }

                Text {
                    id:replacementOrganizationIcon
                    x: -1
                    y: 7
                    width:47
                    height:41
                    text: FontAwesome.industry
                    font.family: FontAwesome.fontFamily
                    font.pixelSize: 36
                }

                Text {
                    id: organizationNameValue
                    x: 52
                    y: 15
                    width: 273
                    height: 29
                    text: qsTr("This user is not part of an organization.")
                    font.pixelSize: 14
                }

            }

            Text {
                id: organizationTextHeader
                x: 22
                y: 96
                width: 355
                height: 17
                text: qsTr("Organization")
                font.bold: true
                font.pixelSize: 12
            }

            Text {
                id: profileTypeName
                x: 151
                y: 41
                width: 192
                height: 14
                text: qsTr("Profile Type Name")
                font.pixelSize: 12
            }

            Text {
                id: userTypeHeader
                x: 81
                y: 41
                width: 64
                height: 14
                text: qsTr("User Type:")
                font.bold: true
                font.pixelSize: 12
            }

            Text {
                id: notificationsHeader
                x: 22
                y: 228
                width: 352
                height: 28
                text: qsTr("Notifications")
                font.bold: true
                font.pixelSize: 12
            }

            ColumnLayout{
                x: 23
                y: 250
                width:parent.width-20
                height:parent.height-250

                ListView {
                    id: notificationsList

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.vertical: ScrollBar {}
                    clip:true

                    delegate: Item {

                        width: 350;
                        height: 170

                        Item {
                            id: notificationItemRow

                            property string targetURL : "";
                            property int appId : -1;
                            property string mongoid : "";
                            property string delivered_date : "";
                            property int notificationId : -1;
                            property int notificationIndex: index

                            Rectangle {
                                id: notificationRoot
                                anchors.left: parent.left
                                anchors.top: parent.top
                                width: 350
                                height: 150

                                Rectangle{
                                    x:0
                                    y:0
                                    width:350
                                    height:150
                                    color:"#333333"
                                    opacity: 0.95
                                }

                                Text {
                                    id:notReplaceIcon
                                    x: 8
                                    y: 13
                                    width:35
                                    height:35
                                    text: FontAwesome.thList
                                    font.family: FontAwesome.fontFamily
                                    font.pixelSize: 30
                                    color: "#ffffff"
                                }

                                Image {
                                    id: notifIcon
                                    x: 8
                                    y: 13
                                    width: 40
                                    height: 40
                                    fillMode: Image.PreserveAspectFit
                                    visible:false

                                    property bool rounded: true
                                    property bool adapt: true

                                    layer.enabled: rounded
                                    layer.effect: OpacityMask {
                                        maskSource: Item {
                                            width: notifIcon.width
                                            height: notifIcon.height
                                            Rectangle {
                                                anchors.centerIn: parent
                                                width: notifIcon.adapt ? notifIcon.width : Math.min(notifIcon.width, notifIcon.height)
                                                height: notifIcon.adapt ? notifIcon.height : width
                                                radius: 9
                                            }
                                        }
                                    }
                                }

                                Text {
                                    id: notificationTitle
                                    x: 58
                                    y: 13
                                    width: 231
                                    height: 23
                                    color: "#ffffff"
                                    text: "message"
                                    font.bold: true
                                    font.pixelSize: 13
                                }

                                Text {
                                    id: notificationText
                                    x: 58
                                    y: 35
                                    wrapMode: Text.WordWrap
                                    width: 231
                                    height: 100
                                    color: "#EEEEEE"
                                    text: qsTr("Body.")
                                    font.pixelSize: 12
                                }

                                Button {
                                    id:engageButton
                                    x:10
                                    y:100
                                    width:120
                                    height:40
                                    onClicked: {
                                        lauchAppFromNotification(notificationItemRow.appId, notificationItemRow.targetURL);
                                    }

                                    contentItem: Text {
                                        id:notification_action
                                        text: "Go"
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
                                        backend.deleteNotification(notificationItemRow.notificationId, notificationItemRow.notificationIndex);
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
                            }
                        }

                        Component.onCompleted: {
                            //console.log(Object.getOwnPropertyNames(model.dataObject));
                            notificationTitle.text = model.dataObject.message;
                            notificationText.text = model.dataObject.data.message_body;
                            notification_action.text = model.dataObject.action_name;
                            notificationItemRow.targetURL = model.dataObject.data.target_url;
                            notificationItemRow.mongoid = model.dataObject.id;
                            notificationItemRow.delivered_date = model.dataObject.delivered_date;
                            notificationItemRow.notificationId = model.dataObject.notification_id_int;
                            notificationItemRow.appId = model.dataObject.app_id_int;

                            if(model.dataObject.app_icon.length > 0){
                                notifIcon.source = model.dataObject.app_icon;
                                notifIcon.visible = true;
                                notReplaceIcon.visible = false;
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle{
        id:navsLeft
        Layout.rowSpan   : 1
        Layout.columnSpan: 1
        Layout.maximumWidth: 100
        Layout.fillWidth: true
        Layout.fillHeight: true

        ColumnLayout {
            id: columnLayout2
            spacing: 0
            anchors.fill: parent

                CornerNavsButton{
                    id:cornerNavsButton

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            userDrawer.open();
                        }
                    }
                }

                Rectangle {
                    id: quickAppsCont
                    y: 50
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "#FFFFFF"
                    border.width: 1
                    border.color: "#c2c1cc"

                    DropArea {
                        anchors {
                            fill: parent;
                        }

                        onDropped:  {
                            //console.log("Dropped. Move App index = " +  drag.source.idx + " to quick apps.");
                            //Add
                            backend.addAppQuick(drag.source.idx);
                            quickAppsCont.border.width = 0;
                            quickAppsCont.border.color = "#000000";
                        }

                        onEntered: {
                            if(typeof(drag.source.caught) != "undefined"){
                                drag.source.caught = true;
                                quickAppsCont.border.width = 1;
                                quickAppsCont.border.color = "#000000";
                            }
                        }

                        onExited: {
                            if(typeof(drag.source.caught) != "undefined"){
                                drag.source.caught = false;
                                quickAppsCont.border.width = 0;
                                quickAppsCont.border.color = "#000000";
                            }
                        }
                    }
                    Rectangle {
                        id:appcont
                        width:89
                        height:parent.height-60
                        x:9
                        y:20
                        color:"#ffffff"

                        ScrollView {
                            id: quickAppScrollView
                            anchors.fill: parent
                            anchors.margins: 0
                            clip: true
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                            bottomPadding: 20

                            Flow {
                                layoutDirection: Qt.LeftToRight
                                spacing: 20;
                                width:parent.width
                                move: Transition {
                                    NumberAnimation {
                                        properties: "x"
                                    }
                                }

                                Repeater {
                                    id: appQuickRepeater
                                    Loader {
                                        id : appQuiclLoaderEm
                                        property int value: index

                                        sourceComponent: Component {
                                            Rectangle {
                                                 id: rectQuick
                                                 width: 90
                                                 height: establishAppHeightBasedOnTextLength(dataObject.nameLength)
                                                 x:0
                                                 y:0
                                                 color: "transparent"
                                                 //z: dragAreaQuick.drag.active ||  dragAreaQuick.pressed ? 4 : 1

                                                 property int idx : index
                                                 property point beginDrag
                                                 property bool quickcaught: false

                                                 Drag.active: dragAreaQuick.drag.active //dragStatus //dragAreaQuick.drag.active

                                                 //Drag.dragType: Drag.Automatic

                                                 states: State {
                                                     when: dragAreaQuick.drag.active
                                                     ParentChange { target: rectQuick; parent: dragContainer }
                                                     //AnchorChanges { target: rectQuick; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
                                                 }

                                                 MouseArea {
                                                     id: dragAreaQuick
                                                     anchors.fill: parent
                                                     drag.target: parent

                                                     onPressed: {
                                                         rectQuick.beginDrag = Qt.point(rectQuick.x, rectQuick.y);
                                                     }

                                                     onReleased: {
                                                         //rectQuick.parent = rectQuick.Drag.target !== null ? rectQuick.Drag.target : positionerDelegate

                                                         if(!rectQuick.quickcaught) {
                                                             backAnimQuickX.from = rectQuick.x;
                                                             backAnimQuickX.to = beginDrag.x;
                                                             backAnimQuickY.from = rectQuick.y;
                                                             backAnimQuickY.to = beginDrag.y;
                                                             backAnimQuick.start();
                                                         }else{
                                                             rectQuick.Drag.drop();
                                                         }
                                                     }

                                                     ParallelAnimation {
                                                         id: backAnimQuick
                                                         SpringAnimation { id: backAnimQuickX; target: rectQuick; property: "x"; duration: 10; spring: 2; damping: 0.2 }
                                                         SpringAnimation { id: backAnimQuickY; target: rectQuick; property: "y"; duration: 10; spring: 2; damping: 0.2 }
                                                     }

                                                     onDoubleClicked: {
                                                        openApp(dataObject.appId);
                                                     }

                                                     Rectangle {
                                                         id: tileQuick
                                                         width: 80
                                                         height: establishAppHeightBasedOnTextLength(dataObject.nameLength)
                                                         color: "transparent"

                                                         ToolTip.visible: dragAreaQuick.containsMouse
                                                         ToolTip.text: "Click to drag/drop. Double click to open."
                                                         ToolTip.delay: 500
                                                         ToolTip.timeout: 5000

                                                        Column {
                                                            y:7
                                                            Image {
                                                                id:quickIcon
                                                                height: 80; width: 80
                                                                source: qsTr(dataObject.icon)
                                                                fillMode: Image.PreserveAspectFit

                                                                property bool rounded: true
                                                                property bool adapt: true

                                                                layer.enabled: rounded
                                                                layer.effect: OpacityMask {
                                                                    maskSource: Item {
                                                                        width: quickIcon.width
                                                                        height: quickIcon.height
                                                                        Rectangle {
                                                                            anchors.centerIn: parent
                                                                            width: quickIcon.adapt ? quickIcon.width : Math.min(quickIcon.width, quickIcon.height)
                                                                            height: quickIcon.adapt ? quickIcon.height : width
                                                                            radius: 16
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            Text {
                                                                text: dataObject.name
                                                                horizontalAlignment: Text.AlignHCenter
                                                                width: parent.width
                                                                wrapMode: Label.WordWrap
                                                            }
                                                          }

                                                         Rectangle {
                                                             id:redBadge
                                                             x: 65
                                                             y: 0
                                                             width: 20
                                                             height: 20
                                                             color: "red"
                                                             radius: width*0.5
                                                             visible: (dataObject.badgeCount > 0)? true: false;
                                                             Text {
                                                                 id:redBadgeNum
                                                                 anchors.centerIn: parent;
                                                                 color: "#ffffff";
                                                                 text: dataObject.badgeCount;
                                                             }
                                                         }
                                                     }

                                                     hoverEnabled: true

                                                     cursorShape: Qt.PointingHandCursor
                                                 }

                                             }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle{
                    id:appsButtonCont
                    width:100
                    height:40
                    color:"#ffffff"
                    x:0
                    y:parent.height-40

                    Text {
                        x: 10
                        y: 12
                        text: FontAwesome.bars
                        font.family: FontAwesome.fontFamily
                        font.pixelSize: 12
                    }

                    Text {
                        x: 25
                        y: 12
                        color: "#50506E"
                        text: qsTr("Apps")
                        font { family: montreuxCDmBd.name; pixelSize: 12; capitalization: Font.Capitalize; weight: Font.DemiBold }
                    }

                    MouseArea{
                       /*x: 10
                        y: 12
                        width:50
                        height:20*/
                        anchors.fill: parent
                        hoverEnabled: true

                        onPressed:{
                            //console.log("Clciked Apps");
                            if(appList.opened){
                                appList.y = appList.parent.height*0.5;
                                hideAppListAnimation.start();
                                //console.log("Started hide");
                                appList.opened = false;
                            }else{
                                appList.y = appList.parent.height;
                                appList.visible = true;
                                showAppListAnimation.start();
                                //console.log("Started show");
                                appList.opened = true;
                            }
                        }

                        cursorShape: Qt.PointingHandCursor
                    }

                    Text {
                        id: editPencil
                        x: 80
                        y: 12
                        color: "#50506E"
                        text: FontAwesome.pencil
                        font.family: FontAwesome.fontFamily
                        font.pixelSize: 12

                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true

                            onPressed:{
                                //console.log("Press Pencil");
                                //Disable/enable dragging
                                if (dragStatus === true)
                                    dragStatus = false;
                                else
                                    dragStatus = true;
                                //console.log("drag Staus:" + dragStatus);
                            }


                            onEntered:{
                                    //console.log("Entered Pencil");
                            }
                            onExited: {
                                    //console.log("Left Pencil");
                            }
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
               }

        }

    }

    Rectangle{
        id:navsRight
        Layout.rowSpan   : 1
        Layout.columnSpan: 1
        Layout.fillWidth: true
        Layout.fillHeight: true

        ColumnLayout {
            id: tabsBar
            spacing: 0
            anchors.fill: parent

            Rectangle {
                id: tabsContainer
                y: 0
                Layout.fillWidth: true
                height: 41
                color: "#ffffff"
                border.width: 1
                border.color: "#c2c1cc"

                property int activeTab:0

                GridView {
                    id: tabsRoot
                    width: parent.width-50;
                    height: 40
                    cellWidth: 140 //Tab width
                    cellHeight: 40
                    y:12
                    x:10

                    displaced: Transition {
                        NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
                    }

                    model: DelegateModel {

                        id: visualModel
                        model: ListModel {
                            id: colorModel
                        }

                        delegate: DropArea {
                            id: delegateRoot

                            width: 139; //Tab width -1
                            height: 30

                            onEntered: {
                                //Exchange tab
                                var urlV, colorText, colorId;

                                visualModel.items.move(drag.source.visualIndex, icon.visualIndex)

                                urlV = webEngs[drag.source.visualIndex];
                                webEngs[drag.source.visualIndex] = webEngs[icon.visualIndex];
                                webEngs[icon.visualIndex] = urlV;

                                //Retain previous tab highlight
                                if(tabsContainer.activeTab != icon.visualIndex){
                                    tabsContainer.activeTab = icon.visualIndex;
                                    switchTabs(icon.visualIndex);
                                    if(appList.opened){
                                        closeAppListFast();
                                    }
                                }

                            }

                            property bool held: false
                            property int visualIndex: DelegateModel.itemsIndex

                            Binding { target: icon; property: "visualIndex"; value: visualIndex }

                            Rectangle {
                                id: icon

                                property int visualIndex: 0


                                width: 139; //Tab width -1
                                height: 30
                                anchors {
                                    horizontalCenter: parent.horizontalCenter;
                                    verticalCenter: parent.verticalCenter
                                }
                                radius: 3

                                color: (model.color === undefined) ? "" : (visualIndex == tabsContainer.activeTab) ? model.color : model.offColor

                                Text {
                                    anchors.centerIn: parent
                                    color: "white"

                                    //text: model.text
                                    text: {
                                        var str = model.text;
                                        if(str.length > 15){
                                            return str.substring(0, 15) + "...";
                                        }
                                        return str;
                                    }

                                    MouseArea { //May have to redraw after drag to get the click bindings back.
                                            id:msArea
                                            anchors.fill: parent
                                            onPressed: {

                                                if(tabsContainer.activeTab != visualIndex){
                                                    tabsContainer.activeTab = visualIndex;
                                                    switchTabs(visualIndex);
                                                    closeAppListFast();
                                                }
                                            }

                                            onPressAndHold: held = true
                                            onReleased: held = false
                                      }
                                 }


                                Text {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    color: "white"
                                    text: "X"
                                    anchors.rightMargin: 6
                                    anchors.topMargin: 6

                                    ToolTip.visible: tabMouseArea.containsMouse
                                    ToolTip.text: "Click to close the app."
                                    ToolTip.delay: 500
                                    ToolTip.timeout: 5000

                                    MouseArea{
                                        id:tabMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onPressed:{
                                            //console.log("Clicked Close");
                                            //console.log(tabsRoot.model.items.get(index).model.id);
                                            //console.log(visualIndex);
                                            var preIndex = visualIndex;
                                            deleteApp(preIndex);
                                            //Highlight Previous
                                           if (preIndex >= 1) {
                                                tabsContainer.activeTab = preIndex - 1;
                                                switchTabs(preIndex - 1);
                                                //appList.visible = false;
                                            }
                                            //If the first index remove then get the next one
                                            else if(preIndex === 0 && colorModel.count === 1){
                                                tabsContainer.activeTab = preIndex;
                                                switchTabs(preIndex);
                                            }
                                        }

                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }

                                // parent.visualIndex

                                DragHandler {
                                    id: dragHandler
                                    yAxis.enabled: false
                                    xAxis.minimum: 0

                                }

                               /*MouseArea { //May have to redraw after drag to get the click bindings back.
                                    id:msArea
                                    anchors.fill: parent
                                    onPressed: {
                                        if(tabsContainer.activeTab != visualIndex){
                                            tabsContainer.activeTab = visualIndex;
                                            switchTabs(visualIndex);
                                            appList.visible = false;
                                        }
                                    }
                                }*/

                                Drag.active: dragHandler.active
                                Drag.source: held ? icon : undefined
                                Drag.hotSpot.x: 36
                                Drag.hotSpot.y: 10

                                states: [
                                    State {
                                        when: icon.Drag.active
                                        ParentChange {
                                            target: icon
                                            parent: tabsRoot
                                        }

                                        AnchorChanges {
                                            target: icon
                                            anchors.horizontalCenter: undefined
                                            anchors.verticalCenter: undefined
                                        }
                                    }
                                ]
                            }
                        }
                //! [1]
                    }

                }

                Rectangle{
                    id:adminAppCont
                    x:parent.width-30
                    y:10
                    width:20
                    height:20

                    Text {
                        id: gearText
                        x: 0
                        y: 0
                        color: "#50506E"
                        text: FontAwesome.cog
                        font.family: FontAwesome.fontFamily
                        font.pixelSize: 20
                    }

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onPressed:{
                            //console.log("Clicked Gear");
                            var result = checkApp(-1);
                            if (result === -1) {
                                var sessionKey = backend.getSessionKey;
                                openAdministration(backend.betaUrl + "?token=" + sessionKey);
                            }
                            else {
                                tabsContainer.activeTab = result;
                                switchTabs(result);
                                closeAppListFast();
                            }

                        }
                        onEntered:{
                                //console.log("Entered Gear");
                        }
                        onExited: {
                                //console.log("Left Gear");
                        }
                        cursorShape: Qt.PointingHandCursor
                    }
                }


            }

            Rectangle {
                id: rightSideCont
                y: 0
                x: 0
                border.width: 1
                border.color: "#c2c1cc"
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#e9e9e9"

                Rectangle{
                    id:topColorBar
                    y: 0
                    x: 1
                    width:parent.width
                    height: 5
                    color: "#50506E"
                }

                Rectangle{ //Web Engines will be placed in this Rectangle
                    id: engineContainer
                    y: 6
                    x: 1
                    width: (devToolsNavsCont.visible)? parent.width-401: parent.width-1
                    height: parent.height-6
                    z: 1
                    color: "#82829f"
                    clip: true

                    Image {
                        id:navsBGImage
                        source: "images/navsFullBG.png"
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        width: 683
                        height: 335
                    }
                }

                //Shortcut Key
                Action{
                    shortcut: "F10"
                    onTriggered: {
                        if(devToolsNavsCont.visible) {
                            //Set to null if devtool is invisible.
                            devToolsNavsCont.visible = false;
                            if(devObj.length > 0) {
                                devObj[0].destroy();
                                devObj.pop();
                            }
                        }
                        else {
                            if (webEngs.length >= 1) {
                                devToolsNavsCont.visible = true;
                                if (appID >= 0) {
                                    var i = checkApp(appId);
                                    openDevTools(webEngs[i]);
                                }
                                else {
                                    openDevTools(webEngs[webEngs.length - 1]);
                                }
                            }
                            else {
                                //Invisible if there is no app open.
                                devToolsNavsCont.visible = false;
                                if(devObj.length > 0) {
                                    devObj[0].destroy();
                                    devObj.pop();
                                }
                            }
                        }
                    }
                }

                //Echo2 File Dialog
                FileDialog {
                    id: dialogEcho2
                }

                //DevTools
                Rectangle{
                    id: devToolsNavsCont
                    border.width: 1
                    border.color: "#c2c1cc"
                    visible: false;
                    height: parent.height - 6
                    width: 400
                    z: 2
                    //Right Alignment
                    LayoutMirroring.enabled: true
                    anchors.left: parent.left
                }

                //App Drawer
                Rectangle{
                    id: appList
                    z:2
                    x:0
                    y:parent.height
                    height:parent.height * 0.5
                    width:parent.width
                    color:"#E9E9E9"
                    visible: false;
                    border.width: 1
                    border.color : "#50506e"

                    property bool opened: false;

                    Text {
                        y: 20
                        z:3
                        color: "#50506E"
                        text: FontAwesome.windowClose
                        font.family: FontAwesome.fontFamily
                        font.pixelSize: 30
                        LayoutMirroring.enabled: true
                        anchors.left: parent.left

                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true

                            onPressed:{
                                appList.y = appList.parent.height*0.5;
                                hideAppListAnimation.start();
                                appList.opened = false;
                            }
                            cursorShape: Qt.PointingHandCursor
                        }
                    }

                    DropArea {
                        anchors {
                            fill: parent;
                        }

                        onDropped:  {
                            //console.log("Dropped. Move Quick App index = " +  drag.source.idx + " from quick apps to apps.");
                            backend.deleteAppQuick(drag.source.idx);
                            appList.border.width = 1;
                        }

                        onEntered: {
                            if(typeof(drag.source.quickcaught) != "undefined"){
                                drag.source.quickcaught = true;
                                appList.border.width = 2;
                            }
                        }

                        onExited: {
                            if(typeof(drag.source.quickcaught) != "undefined"){
                                drag.source.quickcaught = false;
                                appList.border.width = 1;
                            }
                        }
                    }

                    ScrollView {
                        id: appScrollView
                        anchors.fill: parent
                        anchors.margins: 1
                        clip: true
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                        contentHeight: aplicationsHeader.height+appsListView.height+10

                        Text {
                            id:aplicationsHeader
                            x:10
                            y:10
                            color: "#50506E"
                            text: qsTr("Applications")
                            font { family: montreuxCDmBd.name; pixelSize: 30; weight:Font.DemiBold;}
                        }


                        Flow {
                            id:appsListView
                            x:20
                            y:80
                            layoutDirection: Qt.LeftToRight
                            spacing: 10;
                            width:appScrollView.width-100
                            move: Transition {
                                NumberAnimation {
                                    properties: "x"
                                }
                            }
                            Repeater {
                                id:appCategoryRepeater
                                model:appVisualModel
                            }

                        }
                    }
                }
            }
        }
    }
    Item {
        id: dragContainer
    }

    ParallelAnimation{
        id: showAppListAnimation
        running: false
        PropertyAnimation { target: appList; properties: "y"; to: appList.parent.height*0.5; duration: 250; easing.type: Easing.InOutQuad}
    }
    SequentialAnimation{
        id: hideAppListAnimation
        running: false
        PropertyAnimation { target: appList; properties: "y"; to: appList.parent.height-100; duration: 250; easing.type: Easing.InOutQuad}
        PropertyAnimation { target: appList; properties: "visible"; to: false; duration: 1;}
    }

}

