import QtQuick 2.12
import QtQuick.Particles 2.0

Rectangle {
    id:root
    anchors.fill: parent
    opacity: 0.7

    gradient: Gradient {
        GradientStop {position: 0.0; color: "#bbbbc5"}
        GradientStop{position: 1.0; color: "#50526b"}
    }

    ParticleSystem {
        anchors.fill: parent
        ImageParticle {
            source: "qrc:/images/glowdot3.png"
            opacity: 0.25
        }
        Emitter {
            width: parent.width
            height: 150
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            lifeSpan: 15000
            startTime: 15000
            emitRate: 20
            size: 24
            sizeVariation: 16
            acceleration: PointDirection{y: -22; xVariation: 2; yVariation: 2}
        }

        Wander {
            xVariance: 25
            pace: 25
        }
    }
}
