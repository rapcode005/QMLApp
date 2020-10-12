import subprocess
import copy
from collections import Counter
import os
import time
import psutil
import getpass
from datetime import datetime

knownApps = []

def get_logged_in_user():
    checkuser = "unknown"
    try:
        checkuser = getpass.getuser()
        writeLocalLog("The user currently logged in is: " + checkuser)
    except Exception as e:
        writeLocalLog("Error finding logged in user.")
        print(e)
        #raise e

    return checkuser


def getRunningApplications():
    applications = []
    # Iterate over all running process
    for proc in psutil.process_iter():
        try:
            # Get process name & pid from process object.
            processName = proc.name()
            applications.append(processName)
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass
    return applications


def getInstallationKey():
    values = {
        "installationKey":"key",
        "organizationId":0,
        "organizationLocationId":0
    }
    try:
        if os.path.exists("C:/Users/Public/Documents/n@v$12"): #Replace with the path of the terminal program.
            charlieKey = "ggbdfvcft%%$#**&^&^tgfg%$%@@!@SW#EdedS4454**&^h^h55%"
            #Call the NAVS terminal exe to get the decrypted value.---------------
            
    except Exception as e:
        writeLocalLog("Installation file does not exist")
        print(e)
        
    return values


def sendAppStartLog(appName, appCount):
    installData = getInstallationKey()
    if(installData["organizationId"] == 0):
        return
    
    user = get_logged_in_user()
    
    i=0
    while i < appCount:
        
        #Send to Beta1 the log item-------------To Do
        
        writeLocalLog("{} opened new instance of the app: {}".format(user,appName))
        i = i+1


def sendAppEndLog(appName, appCount):
    installData = getInstallationKey()
    if(installData["organizationId"] == 0):
        return
    
    user = get_logged_in_user()
    i=0
    while i < appCount:
        
        #Send to Beta1 the log item-------------To Do
        
        writeLocalLog("{} closed instance of the app: {}".format(user, appName))
        i = i+1


def writeLocalLog(msg):
    curDate = datetime.now().strftime("%c")
    try:
        with open('C:\\Program Files\\CU Northwest\\NAVS\\NAVS_SUPPORT_SERVICE\\ServiceLog.log', 'a') as f:
            f.write('{} {}\n'.format(curDate, msg))
            f.close()
    except OSError as err:
        writeLocalLog("Exception handled: {0}".format(err))
    

#----Can't do this. The service runs in session 0.  The user is a different session.
# def startAlpha1():
#     #print("Starting Alpha1")
#     if os.path.exists("C:/Program Files/CU Northwest/NAVS/navs.exe"): #Should change to the running directory of this service.
#         writeLocalLog("Alpha 1 start")
#         #os.startfile('"C:/Program Files/CU Northwest/NAVS/navs.exe"')
#         #os.system('"C:\\Program Files\\CU Northwest\\NAVS\\navs.exe"')
#         subprocess.Popen('C:\\Program Files\\CU Northwest\\NAVS\\navs.exe')
#         #time.sleep(10)


def appCheck():
    writeLocalLog("App Check")
    currentApps = getRunningApplications()
    global knownApps
    if len(knownApps) != 0:
        x = dict(Counter(knownApps))
        y = dict(Counter(currentApps))
        
        if x != y:
            removedApps = [i for i in x.keys() if i not in y.keys()]
            addedApps = [i for i in y.keys() if i not in x.keys()]
            sameApps = {k: (x[k], y[k]) for k in x.keys() & y.keys()}
            
            if len(removedApps) > 0:
                for key in removedApps:
                    #print("User closed {} old instances of the app: {}".format(x[key], key))
                    sendAppEndLog(key, x[key])
            
            if len(addedApps) > 0:
                for key in addedApps:
                    #print("User opened {} new instances of the app: {}".format(y[key], key))
                    sendAppStartLog(key, y[key])
                
            for key in sameApps:
                if sameApps[key][0] > sameApps[key][1]:
                    #print("User has {} fewer instances of the app: {}".format(sameApps[key][0]-sameApps[key][1], key))
                    sendAppEndLog(key, sameApps[key][0]-sameApps[key][1])
                    
                if sameApps[key][1] > sameApps[key][0]:
                    #print("User added {} more instances of the app: {}".format(sameApps[key][1]-sameApps[key][0], key))
                    sendAppStartLog(key, sameApps[key][1]-sameApps[key][0])
                    
    knownApps = copy.copy(currentApps)
    # if "navs.exe" not in knownApps: 
    #     startAlpha1()