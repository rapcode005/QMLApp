#include "tester.h"

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <QDebug>
#include <QProcess>
#include <QFileInfo>

tester::tester(QObject *parent) : QObject(parent)
{

}

bool tester::IsNAVS( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
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

    return (s == "navs.exe") ? true: false;
}

bool tester::runCheck( void )
{
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

    bool isNAVSFound = false;

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
            //qDebug() << "Calling";
            if(IsNAVS( aProcesses[i] )){
                isNAVSFound = true;
            }
        }
    }

    if(isNAVSFound){
        return true;
    }

    return false;
}

void tester::startNAVS(){
    //qDebug() << "Starting exe";
    QProcess avrdude;
    avrdude.setProgram("\"C:\\Program Files\\CU Northwest\\NAVS\\NAVS_DESKTOP\\navs.exe\"");
    avrdude.startDetached();
}

bool tester::isUpdating()
{
    //Make sure an update is not in process.
    QFileInfo check_file("C:\\Users\\Public\\Documents\\update_initiated.txt");
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        qDebug() << "Found file.";
        return true;
    }
    return false;
}
