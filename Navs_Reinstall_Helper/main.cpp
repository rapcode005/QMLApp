#include <QCoreApplication>
#include <Windows.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const wchar_t* fileLPCWSTR = L"C:\\Users\\Public\\Documents\\n@v$12";
    //std::size_t attr = GetFileAttributes(fileLPCWSTR);
    DWORD attr = GetFileAttributes(fileLPCWSTR);
    if ((attr & FILE_ATTRIBUTE_NORMAL) == 0) {
        SetFileAttributes(fileLPCWSTR, FILE_ATTRIBUTE_NORMAL);
    }

    //Delete the file.
    if(remove("C:\\Users\\Public\\Documents\\n@v$12") != 0){
        qDebug() << "Unable to Delete the installation file. \nThis usually means that the file does not exist, or has already been deleted.";
    }else{
        qDebug() << "Installation file has been deleted. You may close this window.";
    }

    return a.exec();
}
