
function Component() {
    
}

Component.prototype.createOperations = function()
{


    try {
        // call the base create operations function
        component.createOperations();
        if (installer.value("os") == "win") { 
            try {
                var userProfile = installer.environmentVariable("USERPROFILE");
                installer.setValue("UserProfile", userProfile);
                component.addOperation("CreateShortcut", "@TargetDir@\\NAVS_DESKTOP\\navs.exe", "@UserProfile@\\Desktop\\NAVS.lnk");
            } catch (e) {
                // Do nothing if key doesn't exist
                //console.log("Did nothing");
            }

            try {
                component.addOperation("CreateShortcut", "@TargetDir@\\NAVS_DESKTOP\\navs.exe", "@StartMenuDir@\\NAVS.lnk");
            } catch (e) {
                // Do nothing if key doesn't exist
                //console.log("Did nothing");
            }

            try {
                component.addOperation("CreateShortcut", "@TargetDir@\\NAVS_DESKTOP\\Navs_Reinstall_Helper.exe", "@StartMenuDir@\\Navs Reinstall Helper.lnk");
            } catch (e) {
                // Do nothing if key doesn't exist
                //console.log("Did nothing");
            }

            try {
                component.addOperation("CreateShortcut", "@TargetDir@\\maintenancetool.exe", "@StartMenuDir@\\NAVS Maintenance Tool.lnk");
            } catch (e) {
                // Do nothing if key doesn't exist
                //console.log("Did nothing");
            }
            
        }
    } catch (e) {
        //console.log(e);
    }

}