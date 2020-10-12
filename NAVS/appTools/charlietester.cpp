#include "charlietester.h"

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <QDebug>
#include <QProcess>

CharlieTester::CharlieTester(QObject *parent) : QObject(parent)
{

}

bool CharlieTester::checkProcessMatch(DWORD processID, QString processName){
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.S
    if (nullptr != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    QString s = QString::fromWCharArray(szProcessName);

    // Release the handle to the process.
    CloseHandle( hProcess );

    return (s == processName) ? true: false;
}

bool CharlieTester::checkProcessName(QString processName){
    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        qDebug() << "Exiting";
        return 1;
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
            //qDebug() << "Calling";
            if(checkProcessMatch( aProcesses[i], processName )){
                return true;
            }
        }
    }

    return false;
}

void CharlieTester::checkCheckCharlie1(){
    if(!checkProcessName("NAVS_Service_Helper.exe")){
        qDebug() << "Start Charlie1";
        startCharlie1();
    }
}

//void CharlieTester::checkCheckCharlie2(){
//    if(!checkProcessName("NAVS_Service_Reporting.exe")){
//        qDebug() << "Start Charlie2";
//        startCharlie2();
//    }
//}

void CharlieTester::startCharlie1(){
    QProcess charlie1;
    charlie1.setProgram("\"C:\\Program Files\\CU Northwest\\NAVS\\NAVS_COMMUNICATION_SERVICE\\NAVS_Service_Helper.exe\"");
    charlie1.startDetached();
}

//void CharlieTester::startCharlie2(){
//    //To Be implemented when the Charlie2 python service is done.
//}
