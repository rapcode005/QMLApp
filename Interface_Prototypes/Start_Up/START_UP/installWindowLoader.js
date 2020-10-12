function startInstallProcess() {
    var component = Qt.createComponent("InstallScreen1.qml");
    var sprite = component.createObject(primaryWindow, {x: 0, y: 0});

    if (sprite === null) {
        // Error Handling
        console.log("Error creating object");
    }
}
