import servicemanager
import socket
import sys
import win32event
import win32service
import win32serviceutil
# from functions import appCheck
from functions import writeLocalLog
from updater import runCycle

class Charlie1(win32serviceutil.ServiceFramework):
    _svc_name_ = "NAVSHelper"
    _svc_display_name_ = "NAVS Support Service"
    _svc_description_ = "The NAVS Support Service helps ensure that NAVS operates as expected and stays updated."

    def __init__(self, args):
        win32serviceutil.ServiceFramework.__init__(self, args)
        self.hWaitStop = win32event.CreateEvent(None, 0, 0, None)
        socket.setdefaulttimeout(60)

    def SvcStop(self):
        self.ReportServiceStatus(win32service.SERVICE_STOP_PENDING)
        win32event.SetEvent(self.hWaitStop)

    def SvcDoRun(self):
        writeLocalLog("NAVS Support Service Started")
        rc = None
        while rc != win32event.WAIT_OBJECT_0:
            #appCheck()
            runCycle()
            print("Cycle has run")
            rc = win32event.WaitForSingleObject(self.hWaitStop, 1000)

if __name__ == '__main__':
    if len(sys.argv) == 1:
        servicemanager.Initialize()
        servicemanager.PrepareToHostSingle(Charlie1)
        servicemanager.StartServiceCtrlDispatcher()
    else:
        win32serviceutil.HandleCommandLine(Charlie1)