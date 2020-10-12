
function Component() {
    
}

Component.prototype.createOperations = function()
{


    try {
        // call the base create operations function
        component.createOperations();
        if (installer.value("os") == "win") { 
            try {
                component.addOperation("CreateShortcut", "@TargetDir@\\NAVS_COMMUNICATION_SERVICE\\NAVS_Service_Helper.exe", "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\NAVS_Service_Helper.lnk");
            } catch (e) {
                // Do nothing if key doesn't exist
                //console.log("Did nothing");
            }
        }
    } catch (e) {
        //console.log(e);
    }

}