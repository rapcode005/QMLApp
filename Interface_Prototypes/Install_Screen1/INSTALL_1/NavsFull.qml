import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Particles 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import FontAwesome 1.0
import QtQml.Models 2.12
import QtWebEngine 1.7

GridLayout {
    id : root
    rowSpacing: 0
    columnSpacing: 0
    anchors.fill: parent
    rows    : 1
    columns : 2
    visible: false;

//    property var webEngs: [webView0,webView1,webView2,webView3]
    property var webEngs: []

    function goBig(aType, idx){
        navs.flags = Qt.Window;
        drawer.visible = false;
        root.visible = true;
        navs.x = navs.restoreX;
        navs.y = navs.restoreY;
        navs.height = navs.restoreHeight;
        navs.width = navs.restoreWidth;

        if(aType === 1){ //App open in a tab
            tabsContainer.activeTab = idx;
            switchTabs(idx);
            appList.visible = false;

        }else if(aType === 2){ //All Apps
            appList.visible = true;
        }
    }

    function startUp(){
//        webEngs[0].url = "https://cunorthwest.com/";
//        webEngs[1].url = "https://coffieldweb.com/";
//        webEngs[2].url = "https://google.com/";
//        webEngs[3].url = "https://forum.qt.io/";
        root.visible = true;
    }

    function switchTabs(tabIndex){
        for(var i=0; i<webEngs.length; i++){
            if(i===tabIndex){
                webEngs[i].visible = true;
                topColorBar.color = colorModel.get(tabIndex).color;
            }else{
                webEngs[i].visible = false;
            }
        }
    }

    Rectangle{
        id:navsLeft
        Layout.rowSpan   : 1
        Layout.columnSpan: 1
        Layout.preferredWidth  : 100
        Layout.fillHeight: true

        ColumnLayout {
            id: columnLayout2
            spacing: 0
            anchors.fill: parent

            CornerNavsButton{
                id:cornerNavsButton
            }

            Rectangle {
                id: rectangle22
                y: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#ffffff"
                border.width: 1
                border.color: "#c2c1cc"

                    Rectangle{
                        id:appcont
                        width:100
                        height:parent.height-40
                        x:0
                        y:0
                        color:"#ffffff"

                        Column {
                            id: column
                            width: 60
                            x:20

                            Rectangle {
                                id: app1
                                width: 60
                                height: 60
                                color: "#82829F"
                                radius: 10
                                anchors.top: column.top
                                anchors.topMargin: 10

                                MouseArea{
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onPressed:{
                                        console.log("Clicked App 1");
                                    }
                                    onEntered:{
//                                            console.log("Entered App 1");
                                    }
                                    onExited: {
//                                            console.log("Left App 1");
                                    }
                                    cursorShape: Qt.PointingHandCursor
                                }

                                Text {
                                    anchors {
                                        horizontalCenter: parent.horizontalCenter;
                                        top: app1.bottom
                                    }
                                    color: "#50506E"
                                    text: qsTr("Test App 1")
                                    font { family: montreuxCDmBd.name; pixelSize: 11; capitalization: Font.Capitalize; weight: Font.DemiBold }
                                }
                            }

                            Rectangle {
                                id: app2
                                width: 60
                                height: 60
                                color: "#82829F"
                                radius: 10
                                anchors.top: app1.bottom
                                anchors.topMargin: 20

                                MouseArea{
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onPressed:{
                                        console.log("Clicked App 2");
                                    }
                                    onEntered:{
//                                            console.log("Entered App 2");
                                    }
                                    onExited: {
//                                            console.log("Left App 2");
                                    }
                                    cursorShape: Qt.PointingHandCursor
                                }

                                Text {
                                    anchors {
                                        horizontalCenter: parent.horizontalCenter;
                                        top: app2.bottom
                                    }
                                    color: "#50506E"
                                    text: qsTr("Test App 2")
                                    font { family: montreuxCDmBd.name; pixelSize: 11; capitalization: Font.Capitalize; weight: Font.DemiBold }
                                }
                            }

                            Rectangle {
                                id: app3
                                width: 60
                                height: 60
                                color: "#82829F"
                                radius: 10
                                anchors.top: app2.bottom
                                anchors.topMargin: 20

                                MouseArea{
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onPressed:{
                                        console.log("Clicked App 3");
                                    }
                                    onEntered:{
//                                            console.log("Entered App 3");
                                    }
                                    onExited: {
//                                            console.log("Left App 3");
                                    }
                                    cursorShape: Qt.PointingHandCursor
                                }

                                Text {
                                    anchors {
                                        horizontalCenter: parent.horizontalCenter;
                                        top: app3.bottom
                                    }
                                    color: "#50506E"
                                    text: qsTr("Test App 3")
                                    font { family: montreuxCDmBd.name; pixelSize: 11; capitalization: Font.Capitalize; weight: Font.DemiBold }
                                }
                            }
                        }
                    }

                    Rectangle{
                        width:100
                        height:40
                        color:"#ffffff"
                        x:0
                        y:parent.height-40

                        Text {
                            //id: element1
                            x: 10
                            y: 12
                            text: FontAwesome.bars
                            font.family: FontAwesome.fontFamily
                            font.pixelSize: 12
                        }

                        Text {
                            //id: element
                            x: 25
                            y: 12
                            color: "#50506E"
                            text: qsTr("Apps")
                            font { family: montreuxCDmBd.name; pixelSize: 12; capitalization: Font.Capitalize; weight: Font.DemiBold }
                        }

                        MouseArea{
                            x: 10
                            y: 12
                            width:50
                            height:20
                            hoverEnabled: true
                            onPressed:{
                                console.log("Clciked Apps");
                                if(appList.visible){
                                    appList.visible = false;
                                }else{
                                    appList.visible = true;
                                }
                            }
                            onEntered:{
                                        console.log("Entered Apps");
                            }
                            onExited: {
                                        console.log("Left Apps");
                            }
                            cursorShape: Qt.PointingHandCursor
                        }

                        Text {
                            //id: element1
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
                                    console.log("Clicked Pencil");
                                }
                                onEntered:{
                                        console.log("Entered Pencil");
                                }
                                onExited: {
                                        console.log("Left Pencil");
                                }
                                cursorShape: Qt.PointingHandCursor
                            }
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

                //! [0]
                    model: DelegateModel {
                //! [0]
                        id: visualModel
                        model: ListModel {
                            id: colorModel
                            ListElement {
                                color: "#50506E"
                                text: "Welcome"
                                offColor:"#CDCDDC"
                                appId:0
                            }
                            ListElement {
                                color: "#E8A239"
                                text: "Test App 1"
                                offColor:"#CDCDDC"
                                appId:1
                            }
                            ListElement {
                                color: "#4C80F8"
                                text: "TestApp 2"
                                offColor:"#CDCDDC"
                                appId:2
                            }
                            ListElement {
                                color: "#60BA97"
                                text: "TestApp 3"
                                offColor:"#CDCDDC"
                                appId:3
                            }

                        }
                //! [1]
                        delegate: DropArea {
                            id: delegateRoot

                            width: 139; //Tab width -1
                            height: 30

                            onEntered: visualModel.items.move(drag.source.visualIndex, icon.visualIndex)

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

                                color: (parent.visualIndex == tabsContainer.activeTab)? model.color : model.offColor


                                Text {
                                    anchors.centerIn: parent
                                    color: "white"
                                    text: model.text
                                }

                                Text {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    color: "white"
                                    text: "X"
                                    anchors.rightMargin: 6
                                    anchors.topMargin: 6

                                    MouseArea{
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onPressed:{
                                            console.log("Clicked Close");
                                        }
                                        onEntered:{
                                            console.log("Entered Close");
                                        }
                                        onExited: {
                                            console.log("Left Close");
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

                                MouseArea { //May have to redraw after drag to get the click bindings back.
                                    id:msArea
                                    anchors.fill: parent
                                    onPressed: {
                                        if(tabsContainer.activeTab != parent.visualIndex){
                                            tabsContainer.activeTab = parent.visualIndex;
                                            switchTabs(index);
                                            appList.visible = false;
                                        }
                                    }
                                }

                                Drag.active: dragHandler.active
                                Drag.source: icon
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
                            console.log("Clicked Gear");
                        }
                        onEntered:{
                                console.log("Entered Gear");
                        }
                        onExited: {
                                console.log("Left Gear");
                        }
                        cursorShape: Qt.PointingHandCursor
                    }
                }


            }

            Rectangle {
                id: rectangle2
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

//                WebEngineView {
//                    id:webView0
//                    x: 1
//                    y:5
//                    height:parent.height-5
//                    width:parent.width-1
//                    visible: true;
//                }
//                WebEngineView {
//                    id:webView1
//                    x: 1
//                    y:5
//                    height:parent.height-5
//                    width:parent.width-1
//                    visible: false;
//                }
//                WebEngineView {
//                    id:webView2
//                    x: 1
//                    y:5
//                    height:parent.height-5
//                    width:parent.width-1
//                    visible: false;
//                }
//                WebEngineView {
//                    id:webView3
//                    x: 1
//                    y:5
//                    height:parent.height-5
//                    width:parent.width-1
//                    visible: false;
//                }

                Rectangle{
                    id: appList
                    x: 1
                    y:0
                    height:parent.height
                    width:parent.width-1
                    color:"#FFFFFF"
                    visible: false;

                    Text {
                        x: 30
                        y: 30
                        color: "#50506E"
                        text: qsTr("Applications")
                        font { family: montreuxCDmBd.name; pixelSize: 30; weight:Font.DemiBold;}
                    }

                    //Add the gridViews here for all the apps:
                    //https://doc.qt.io/qt-5/qml-qtquick-gridview.html
                }
            }
        }
    }
}
