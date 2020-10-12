import os
import glob
from pathlib import Path
import shutil
from urllib.request import urlopen
from datetime import datetime
from packaging import version
import requests
import xml.etree.ElementTree as ET 
import psutil
import subprocess
from time import sleep
from functions import writeLocalLog

def runCycle():
    directory = Path('C:/Program Files/CU Northwest/NAVS')
    signalFileDirectory = "C:/Users/Public/Documents"
    #repoUrl = "http://localhost/"
    repoUrl = "http://app.navsnow.com/"
    

    #Check if the update_initialized.txt file is there.
    #If the update_initialized.txt file exists, start the update.
    if check_initialized_file_exists(signalFileDirectory):
        availableVersion = getAvailableVersion(repoUrl)
        if availableVersion != False:
            runUpdateOp(repoUrl, directory, signalFileDirectory, availableVersion)

    else:
        #If not, Check if the update_available.txt file is there.
        if check_update_file_exists(signalFileDirectory):
            #If it is, see if it's been 24hrs since the time in the file.
            dt = getDateInFile(signalFileDirectory+'/update_available.txt')
            if was24HrsAgo(dt):
                #If so, create the update_initialized.txt and start the update.
                createUpdateInitialized(signalFileDirectory)
                availableVersion = getAvailableVersion(repoUrl)
                if availableVersion != False:
                    runUpdateOp(repoUrl, directory, signalFileDirectory, availableVersion)


        else:
            #If neither file exists and it's midnight, go look at the repo to see if there's an update. 
            if checkIfMidnight():
                currentVersion = getCurrentVersionNumber(directory)
                if currentVersion != False:

                    availableVersion = getAvailableVersion(repoUrl)
                    if availableVersion != False:
                        if version.parse(currentVersion) < version.parse(availableVersion):
                            writeLocalLog("Update found. Version: " + availableVersion + " starting the 24 hour clock.")
                            #If so, create the update_available.txt file
                            createUpdateAvailable(signalFileDirectory)


def runUpdateOp(repoUrl, directory, signalFileDirectory, availableVersion):
    if availableVersion != False:
        writeLocalLog("Updating now to version " + availableVersion)
        kill_by_process_name("navs.exe")
        sleep(10)
        deleteOldUpdate7fFile(directory)
        if updateFiles(directory, repoUrl, availableVersion):
            updateVersionFile(directory, availableVersion)
            deleteUpdateFiles(signalFileDirectory, directory)
            writeLocalLog("Update to version " + availableVersion + " complete")


def updateVersionFile(directory, versionNumber):
    try:
        f = open(str(directory) + "\\NAVS_DESKTOP\\release_version.txt", "w")
        f.write(versionNumber)
        f.close()
    except OSError as err:
        writeLocalLog("updateVersionFile Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))


def kill_by_process_name(name):
    for proc in psutil.process_iter():
        if proc.name() == name:
            print("Killing process: " + name)
            if(check_process_exist_by_name(name)):
                os.system("taskkill /f /im " + name)
            return


def check_process_exist_by_name(name):
    for proc in psutil.process_iter():
        if proc.name() == name:
            return True
 
    return False


def deleteUpdateFiles(signalFileDirectory, directory):
    try:
        if check_update_file_exists(signalFileDirectory):
            print("Deleting the update available file")
            os.remove(signalFileDirectory+"/update_available.txt")
    except OSError as err:
        writeLocalLog("deleteUpdateFiles (update_available.txt) Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))
        
    try:
        if check_initialized_file_exists(signalFileDirectory):
            print("Deleting the update initiated file")
            os.remove(signalFileDirectory+"/update_initiated.txt")
    except OSError as err:
        writeLocalLog("deleteUpdateFiles (update_initiated.txt) Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))

    deleteOldUpdate7fFile(directory)
    

def deleteOldUpdate7fFile(directory):
    try:
        if check_update7zip_file_exists(directory, "NAVS_COMPONENTS.7z"):
            print("Attempting to delete " + str(directory)+"\\NAVS_COMPONENTS.7z")
            os.remove(str(directory)+"\\NAVS_COMPONENTS.7z")
            print(str(directory)+"\\NAVS_COMPONENTS.7z deleted")
    except OSError as err:
        writeLocalLog("deleteOldUpdate7fFile (update.7z) Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))
        
    try:
        if check_update7zip_file_exists(directory, "NAVS_DESKTOP.7z"):
            print("Attempting to delete " + str(directory)+"\\NAVS_DESKTOP.7z")
            os.remove(str(directory)+"\\NAVS_DESKTOP.7z")
            print(str(directory)+"\\NAVS_DESKTOP.7z deleted")
    except OSError as err:
        writeLocalLog("deleteOldUpdate7fFile (update.7z) Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))


def getAvailableVersion(repoUrl):
    try:
        url = repoUrl + 'repository/Updates.xml'
        resp = requests.get(url) 
        tree = ET.fromstring(resp.content)
         
        for item in tree.findall('./PackageUpdate'):
            for child in item: 
                if child.tag == "Version":
                    return child.text
        
    except OSError as err:
        writeLocalLog("getAvailableVersion Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))
        sleep(300)

    return False


def getCurrentVersionNumber(directory):
    try:
        f = open(str(directory) + "\\NAVS_DESKTOP\\release_version.txt", "r")
        val = f.read()
        f.close()
        return val
    except OSError as err:
        writeLocalLog("getCurrentVersionNumber Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))
    return False


def checkIfMidnight():
    now = datetime.now()
    seconds_since_midnight = (now - now.replace(hour=0, minute=0, second=0, microsecond=0)).total_seconds()
    return seconds_since_midnight == 0


def createUpdateAvailable(signalFileDirectory):
    try:
        f = open(signalFileDirectory + "/update_available.txt", "w")
        x = datetime.now()
        f.write(x.strftime("%Y-%m-%dT%H:%M:%S"))
        f.close()
    except OSError as err:
        writeLocalLog("createUpdateAvailable Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))


def createUpdateInitialized(signalFileDirectory):
    try:
        f = open(signalFileDirectory + "/update_initiated.txt", "w")
        x = datetime.now()
        f.write(x.strftime("%Y-%m-%dT%H:%M:%S"))
        f.close()
    except OSError as err:
        writeLocalLog("createUpdateInitialized Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))


def check_update7zip_file_exists(directory, fileName):
    return os.path.exists(str(directory) + '\\' + fileName)


def check_initialized_file_exists(signalFileDirectory):
    return os.path.exists(signalFileDirectory + '/update_initiated.txt')


def check_update_file_exists(signalFileDirectory):
    return os.path.exists(signalFileDirectory + '/update_available.txt')


def getDateInFile(pth):
    f = open(pth, "r")
    date_time_str = f.read()
    f.close()
    date_time_obj = datetime.strptime(date_time_str, '%Y-%m-%dT%H:%M:%S') 
    
    return date_time_obj


def was24HrsAgo(chkDate):
    tod = datetime.now()
    dif = tod - chkDate
    return dif.days > 0


def downloadFile(url, fileName, directory):
    try:
        zipresp = urlopen(url)
        tempzip = open(str(directory)+"/"+fileName, "wb")
        tempzip.write(zipresp.read())
        tempzip.close()
        return True
        
    except OSError as err:
        writeLocalLog("downloadFile Exception handled: {0}".format(err))
        print("Exception handled: {0}".format(err))
        return False

                    
def removeOldFiles(directory):
    saveFiles = ['NAVS_COMPONENTS.7z','NAVS_DESKTOP.7z']
    saveDirs = ['Licenses', 'NAVS_COMMUNICATION_SERVICE', 'NAVS_SUPPORT_SERVICE']
    
    for entry in os.scandir(directory):
        if entry.is_file():
            if not os.path.basename(entry) in saveFiles:
                #print(os.path.basename(entry))
                try:
                    os.remove(entry)
                except OSError as err:
                    print("Exception handled: {0}".format(err))
        elif entry.is_dir:
            if not os.path.basename(entry) in saveDirs:
                #print(os.path.basename(entry)
                try:
                    shutil.rmtree(entry)
                except OSError as err:
                    writeLocalLog("removeOldFiles Exception handled: {0}".format(err))
                    print("Exception handled: {0}".format(err))


def updateFiles(directory, repoUrl, availableVersion):
    coreDownloaded = downloadFile(repoUrl + 'NAVS_COMPONENTS.7z','NAVS_COMPONENTS.7z', directory)
    alpha1Downloaded = downloadFile(repoUrl + 'repository/com.navsnow.alpha1/'+availableVersion+"NAVS_DESKTOP.7z",'NAVS_DESKTOP.7z', directory)
    
    if coreDownloaded and alpha1Downloaded:
        try:
            print("Both files downloaded. Starting")
            
            removeOldFiles(directory)
            sleep(10)
            
            subprocess.call('"'+str(directory)+'\\NAVS_SUPPORT_SERVICE\\7-Zip\\7z.exe" x "'+str(directory)+'\\NAVS_COMPONENTS.7z" -o"'+str(directory)+'" -aoa ')
            subprocess.call('"'+str(directory)+'\\NAVS_SUPPORT_SERVICE\\7-Zip\\7z.exe" x "'+str(directory)+'\\NAVS_DESKTOP.7z" -o"'+str(directory)+'" -aoa ')
            sleep(10)
            
            return True
        
        except OSError as err:
            writeLocalLog("updateFiles Exception handled: {0}".format(err))
            print("Exception handled: {0}".format(err))
            sleep(300)
            return False
            
    
    return False