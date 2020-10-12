import QtQuick 2.0
import QtQml.Models 2.1

Component {
    id: appDelegate

    Flow {
        id:appsRowView
        y:0
        layoutDirection: Qt.LeftToRight
        spacing: 10;
        width:appsListView.width
        move: Transition {
            NumberAnimation {
                properties: "x"
            }
        }

        Rectangle{
            y:0
            color: "#E9E9E9";
            height:30
            width:appsListView.width

            Text {
                anchors.fill: parent
                id: catApp
                text: category
                font { family: montreuxCDmBd.name;
                pixelSize: 20; weight:Font.DemiBold;}
            }
        }

        Repeater {
            y:40
            id:appCategoryRepeater

            model: DelegateModel {
                id: appRowModel;
                //model: index+5;
                delegate: AppItemDelegate {}
            }

            Component.onCompleted: {
                appRowModel.model = backend.getAppFilterProxyByIndex(index);
            }

        }

    }
}
