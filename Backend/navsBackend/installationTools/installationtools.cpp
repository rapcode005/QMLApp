#include "installationtools.h"

#include <QDataStream>
#include <QCryptographicHash>
#include <iostream>
#include <fstream>
#include <random>
#include <cstring>

InstallationTools::InstallationTools(QObject *parent) : QObject(parent)
{

}

QString InstallationTools::generateCode() const
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);

    return QString::fromStdString(str.substr(0, 32));
}

QString InstallationTools::getInstallationKeyFile()
{
    QFile file("n@v$12");
    //CryptDevice deviceFilter(&file);
    //deviceFilter.open(QIODevice::ReadOnly);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        return "";
    }
    else {
        QTextStream stream(&file);
        QString str = encryptDecrypt(stream.readAll());
        //stream >> str;
        file.flush();
        file.close();
        return str.split(" ")[7];
    }
}

void InstallationTools::createInstallationKeyFile(const QString &code)
{
    QFile file("n@v$12");
    //Check if file open is true
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "Unable to Create a file";
    } else{
        //Write a text
        QTextStream out(&file);
        out << encryptDecrypt(stringMultiple() + " " + code);
        //Flush all stream text
        file.flush();
        file.close();
        hideFile();
    }
}

void InstallationTools::hideFile()
{
    const wchar_t* fileLPCWSTR = L"n@v$12";
    //std::size_t attr = GetFileAttributes(fileLPCWSTR);
    DWORD attr = GetFileAttributes(fileLPCWSTR);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(fileLPCWSTR, attr | FILE_ATTRIBUTE_HIDDEN |
                          FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM);
    }
}

void InstallationTools::downloadISOFiles()
{
    QString url = "https://navs-assets.s3-us-west-2.amazonaws.com/isoHelperFiles/isoHelper.zip";
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));
    QEventLoop event;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();

    if (response->error() == QNetworkReply::NoError)
    {
        QFile file("isoHelper.zip");
        //file.close();
        if(!file.open(QFile::WriteOnly))
        {
            /*
            file.close();
            file.open(QIODevice::WriteOnly);
            file.write(response->readAll());
            */
            qDebug() << "Unable to download";
        }
        else
        {
            //file.open(QIODevice::WriteOnly);
            file.write(response->readAll());
        }
        file.close();
    }
    else {
        qDebug() << "error";
    }
}

void InstallationTools::createFolder(const QString &name)
{
    if (!QDir(name).exists())
        QDir().mkdir(name);
    else
    {
        //Delete if Exists to overwrite.
        QDir dir(name);
        dir.removeRecursively();
    }
}

void InstallationTools::unZipFile()
{

    QString c = "isoHelper.zip";

    createFolder(getInfoFromBeta());

    QString d = getInfoFromBeta();

    QString dirPath = QDir::toNativeSeparators(QDir::currentPath());

    QFile::copy(":/7-Zip/7z.exe", dirPath + "\\7z.exe");

    const QString program = "7z.exe";

    QStringList arguments;
    arguments << "x" << dirPath + "\\" + c << "-o" + dirPath + "\\" + d;
    QProcess *process = new QProcess(this);
    process->setWorkingDirectory(dirPath);
    process->start(program, arguments);
    process->waitForFinished(-1);
    //rocess->close();
}

void InstallationTools::runFile()
{
    QString dirPath = QDir::toNativeSeparators(QDir::currentPath());
    QProcess *process = new QProcess(this);
    QString program = getInfoFromBeta() + "\\ArchiveViewer.exe";
    process->setWorkingDirectory(dirPath);
    process->start(program);
    process->waitForFinished(-1);
    //process->close();
}

void InstallationTools::deleteInstallationID()
{
    const wchar_t* fileLPCWSTR = L"n@v$12";
    //std::size_t attr = GetFileAttributes(fileLPCWSTR);
    DWORD attr = GetFileAttributes(fileLPCWSTR);
    if ((attr & FILE_ATTRIBUTE_NORMAL) == 0) {
        SetFileAttributes(fileLPCWSTR, FILE_ATTRIBUTE_NORMAL);
    }

    //Delete the file.
    if(remove("n@v$12") != 0)
        qDebug() << "Unable to Delete";

}

void InstallationTools::createISOHelper()
{
    qDebug() << "Download Start";
    downloadISOFiles();
    qDebug() << "UnZipping File";
    unZipFile();
    qDebug() << "run the application";
    runFile();
    qDebug() << "done";
    cleanup();
    qDebug() << "clean up done.";
}

QString InstallationTools::getInfoFromBeta()
{
    QString result = "isoHelper";

    return result;
}

void InstallationTools::cleanup()
{
   deleteFiles("isoHelper.zip");
   QFile fileZip("7z.exe");
   fileZip.setPermissions(QFile::ReadOther | QFile::WriteOther);
   fileZip.close();
   deleteFiles("7z.exe");
}

void InstallationTools::deleteFiles(const QString &name)
{
    QByteArray ba = name.toLocal8Bit();
    const char *filename = ba.data();

    if(remove(filename) != 0)
        qDebug() << "Unable to Delete " << name;
}

void InstallationTools::checkregisterStatus()
{
    bool registerStatus = true;

    QString key = getInstallationKeyFile();

    if(key.isEmpty()) {
        qDebug() << "Unable to read a file.";
        registerStatus = false;
     }
     else {
        registerStatus = true;
    }

    emit registerStatusLoaded(registerStatus);
}

void InstallationTools::loadInstallationData(OrganizationModel *organizationModel, const int &index)
{
    QJsonObject jsonResult;
    QString code = generateCode();

    if (code.isEmpty()) {
        qDebug() << code;
    }
    else {
        int locationID = organizationModel->get(index)->locationID();
        int releaseID = getReleaseID();
        QString ipAddress = getIPAddress();
        int installationTypeID = getInstallationTypeID();

        qDebug() << releaseID;

        QString url = webServiceTools.getApiUrlNavs() + "api/navs-installations?"
        "installation_type_id=" + QString::number(installationTypeID) + "&ip_address=" + ipAddress +
                "&release_id=" + QString::number(releaseID) +
                "&organization_location_id=" + QString::number(locationID) +
                + "&key=" + code;

        qDebug() << url;

        webServiceTools.computeJsonValuePost(url);

        jsonResult = webServiceTools.getJsonValue();

        if (jsonResult.size() > 0) {

            createInstallationKeyFile(code);
        }
    }


    emit installationDataLoaded(jsonResult);
    /*
    QJsonObject jsonInstallationData;

    QString ipAddress = getIPAddress();
    int locationID = organizationModel->get(index)->locationID();
    int installationTypeID = getInstallationTypeID();
    //int installationTypeID = 1;
    //int releaseID = 1;
    int releaseID = getReleaseID();

    QString code;

    QString url = webServiceTools.getApiUrlNavs() + "api/navs-installations?"
            "ip_address=" + ipAddress + "&release_id=" + QString::number(releaseID) +
            "&installation_type_id=" + QString::number(installationTypeID) +
            "&organization_location_id=" + QString::number(locationID);

    webServiceTools.computeJsonValuePost(url);

    jsonInstallationData = webServiceTools.getJsonValue();

    if (jsonInstallationData.size() > 0) {

        code = jsonInstallationData["key"].toString();

        qDebug() << code;

        //Create the file.
        createInstallationKeyFile(code);
    }

    emit installationDataLoaded(jsonInstallationData);
    */
}

void InstallationTools::loginUsername(const QString &username, const QString &password,
                                      const int &organizationID)
{
    QJsonObject jsonResult;
    QString url = webServiceTools.getApiUrlNavs() + "api/users/login?"
             "login_type_id=" + QString::number(getInstallationTypeID()) +
             "&organization_id=" + QString::number(organizationID) +
             "&ip=" + getIPAddress() +
             "&username=" + username +
             "&password=" + password;

    webServiceTools.computeJsonValuePost(url);

    jsonResult = webServiceTools.getJsonValue();

    emit loginUsernameLoaded(jsonResult);
}

int InstallationTools::getInstallationTypeID()
{
    QString url = webServiceTools.getApiUrlNavs() + "api/installation-types/1";
    QJsonObject jsonInstallationType;

    webServiceTools.computeJsonValueGet(url);

    jsonInstallationType = webServiceTools.getJsonValue();

    if (jsonInstallationType.size() > 0)
    {
        if (jsonInstallationType["id"].toInt() == 0)
            return 1;
        else
            return jsonInstallationType["id"].toInt();
    }

    return 1;
}

QString InstallationTools::getIPAddress()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
              return address.toString();
    }
    return "192.168.0.1";
}

int InstallationTools::getReleaseID()
{
    QString url = webServiceTools.getApiUrlNavs() + "api/releases/1";
    QJsonObject jsonRelease;

    webServiceTools.computeJsonValueGet(url);

    jsonRelease = webServiceTools.getJsonValue();

    if (jsonRelease.size() > 0)
    {
        if (jsonRelease["id"].toInt() == 0)
            return 1;
        else
            return jsonRelease["id"].toInt();
    }


    return 1;
}

QString InstallationTools::stringMultiple() const
{
    return "mmkcxzzedfzss342 lllkkmnbbvtb12343 jijijimjdnjuj123 lkkiif3sdsd3 mmkmmkmkmkm22332 "
           "lololnnhnjjh2323111 11234323564563jjii 2344343355334553";
}

QString InstallationTools::encryptDecrypt(const QString &code)
{
    char key = 'K'; //Any char will work
    std::string input = code.toStdString();
    std::string output = input;

    for (std::size_t i = 0; i < input.length(); i++)
        output[i] = input[i] ^ key;

    return QString::fromStdString(output);
}

void InstallationTools::loadInfoFromBeta1()
{

}






