import QtQuick 2.0
import QtQml.Models 2.1
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.14

Component {
    id: appItemDelegate

    Rectangle {
         id: rect
         width: 90
         height: 120
         x:0
         y:0
         color: "transparent"
         property point beginDrag
         property bool caught: false
         property int idx : dataObject.appInstallId;

         Drag.active: dragArea.drag.active //dragStatus  //Changing value when you click the pencil

         Drag.onDragStarted: {
            console.log("Started");
         }

         Drag.onDragFinished: {
             console.log("Stopped");
         }

         states: State {
             when: dragArea.drag.active
             ParentChange { target: rect; parent: dragContainer }
         }

         MouseArea {
             id: dragArea
             anchors.fill: parent
             drag.target: parent

             onPressed: {
                rect.beginDrag = Qt.point(rect.x, rect.y);
             }

             onReleased: {
                 if(!rect.caught) {
                     backAnimX.from = rect.x;
                     backAnimX.to = beginDrag.x;
                     backAnimY.from = rect.y;
                     backAnimY.to = beginDrag.y;
                     backAnim.start();
                 }else{
                     rect.Drag.drop();
                 }
             }

             ParallelAnimation {
                 id: backAnim
                 SpringAnimation { id: backAnimX; target: rect; property: "x"; duration: 10; spring: 2; damping: 0.2 }
                 SpringAnimation { id: backAnimY; target: rect; property: "y"; duration: 10; spring: 2; damping: 0.2 }
             }

             onDoubleClicked: {
                 openApp(dataObject.appId);
             }

             Rectangle {
                 id: tile
                 width: 80;
                 height: 120
                 color: "transparent"

                 ToolTip.visible: dragArea.containsMouse
                 ToolTip.text: "Click to drag/drop. Double click to open."
                 ToolTip.delay: 500
                 ToolTip.timeout: 5000

                Column {

                    Image {
                          id:appLoaderIcon
                          height: 80; width: 80
                          source: qsTr(dataObject.icon)
                          fillMode: Image.PreserveAspectFit

                          property bool rounded: true
                          property bool adapt: true

                          layer.enabled: rounded
                          layer.effect: OpacityMask {
                              maskSource: Item {
                                  width: appLoaderIcon.width
                                  height: appLoaderIcon.height
                                  Rectangle {
                                      anchors.centerIn: parent
                                      width: appLoaderIcon.adapt ? appLoaderIcon.width : Math.min(appLoaderIcon.width, appLoaderIcon.height)
                                      height: appLoaderIcon.adapt ? appLoaderIcon.height : width
                                      radius: 16
                                  }
                              }
                          }

                      }


                    Text {
                        width: 80
                        text: dataObject.name
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Label.WordWrap
                    }
                }

                 Rectangle {
                     id:redBadge
                     x: 65
                     y: -7
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
