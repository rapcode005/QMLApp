import QtQuick 2.0
import QtQuick.Particles 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: cornerNavsButton
    y: 0
    Layout.fillWidth: true
    height: 41
    border.width: 1
    border.color: "#c2c1cc"

    gradient: Gradient {
        GradientStop {position: 0.0; color: "#ffffff"}
        GradientStop{position: 1.0; color: "#82829F"}
    }

    ParticleSystem {
        anchors.fill: parent
        ImageParticle {
            source: "qrc:/images/glowdot3.png"
            opacity: 1
        }
        Emitter {
            width: parent.width
            height: parent.height
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            lifeSpan: 15000
            startTime: 15000
            emitRate: 20
            size: 2
            sizeVariation: 16
            acceleration: PointDirection{y: -22; xVariation: 2; yVariation: 2}
        }

        Wander {
            xVariance: 25
            pace: 25
        }

        Text {
            id: element
            x: 21
            y: 11
            color: "#ffffff"
            text: qsTr("NAVS")
            font { family: montreuxCDmBd.name; pixelSize: 18; capitalization: Font.Capitalize; weight: Font.DemiBold }
        }
    }
}
