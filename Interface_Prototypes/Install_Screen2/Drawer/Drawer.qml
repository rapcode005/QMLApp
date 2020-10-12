import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.5
import QtQuick.Particles 2.0

ApplicationWindow {
    visible: true
    width: 50
    height: 100
    x:Screen.width-50
    y:(Screen.height/2)-50
    flags:Qt.FramelessWindowHint|Qt.WindowStaysOnTopHint

    Rectangle {
        id:drawer
        anchors.fill: parent
        opacity: 0.7
        visible:false

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
        }
    }
}
