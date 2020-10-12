#include "installationtools.h"

#include <QDataStream>
#include <iostream>
#include <fstream>
#include <random>
#include <cstring>

InstallationTools::InstallationTools(QObject *parent) : QObject(parent)
{
    connect(&webServiceTools, &WebServicesTools::networkError, this,
            &InstallationTools::requestErrorOccurred);
}

QString InstallationTools::generateCode() const
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(str.begin(), str.end(), generator);

    return QString::fromStdString(str.substr(0, 32));
}

QString InstallationTools::getInstallationKeyFile() const
{
    const QString &inst = decryptInstallation();
    return inst.length() < 1? "": inst.split(" ")[0];
}

int InstallationTools::getOrganizationID() const
{
    const QString &inst = decryptInstallation();
    return inst.length() < 1? 0: inst.split(" ")[1].toInt();
}

int InstallationTools::getLocationID() const
{
    const QString &inst = decryptInstallation();
    return inst.length() < 1? 0: inst.split(" ")[2].toInt();
}

void InstallationTools::createInstallationKeyFile(const QString &code,  const int &orgId,
                                                  const int &orgLocationId)
{
    QSaveFile file(fileData());

    //Check if file open is true
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "Unable to Create a file";
    } else{
        const QString allCode =  code + " " +
                QString::number(orgId)+ " " +
                QString::number(orgLocationId) + " " +
                " rbjkrhgfiu4eybc7847##4#2tghj+8b9y8rte5lihe5hgjke";

        file.write(encryptInstallation(allCode));
        file.commit();
    }
}

void InstallationTools::hideFile()
{
    const wchar_t* fileLPCWSTR = L"C:\\Users\\Public\\Documents\\n@v$12";
    //std::size_t attr = GetFileAttributes(fileLPCWSTR);
    DWORD attr = GetFileAttributes(fileLPCWSTR);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(fileLPCWSTR, attr | FILE_ATTRIBUTE_HIDDEN |
                          FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM);
    }
    delete fileLPCWSTR;
    fileLPCWSTR = nullptr;
}

void InstallationTools::deleteInstallationID()
{
    const wchar_t* fileLPCWSTR = L"C:\\Users\\Public\\Documents\\n@v$12";
    DWORD attr = GetFileAttributes(fileLPCWSTR);
    if ((attr & FILE_ATTRIBUTE_NORMAL) == 0) {
        SetFileAttributes(fileLPCWSTR, FILE_ATTRIBUTE_NORMAL);
    }

    //Delete the file.
    if(remove("C:\\Users\\Public\\Documents\\n@v$12") != 0)
        qDebug() << "Unable to Delete";

}

QString InstallationTools::fileData() const
{
    return QStringLiteral("C:\\Users\\Public\\Documents\\n@v$12");
}

void InstallationTools::checkregisterStatus()
{
    bool registerStatus = true;

    const QString &key = getInstallationKeyFile();

    if(key.isEmpty()) {
        qDebug() << "Unable to read a file.";
        registerStatus = false;
     }
     else {
        registerStatus = true;
    }

    emit registerStatusLoaded(registerStatus);
}

void InstallationTools::sendInstallationData(OrganizationModel *organizationModel,
                                             const int &index, const QString &sessionKey)
{
    try {

        const QString &code = generateCode();

        if (code.isEmpty()) {
            emit installationDataLoaded(QJsonObject());
        }
        else {

            const int &orgId = organizationModel->get(index)->organizationID();
            const int &locationID = organizationModel->get(index)->locationID();
            const int &releaseID = getReleaseID(sessionKey);
            const QString &ipAddress = getIPAddress();
            const int &installationTypeID = getInstallationTypeID();

            const QString &url = webServiceTools.getApiUrlNavs() + "api/navs-installations?"
            "installation_type_id=" + QString::number(installationTypeID) + "&ip_address=" + ipAddress +
                    "&release_id=" + QString::number(releaseID) +
                    "&organization_location_id=" + QString::number(locationID) +
                    + "&key=" + code;

//            connect(webServiceTools, &WebServicesTools::statusCode, this, [=](const int &code) {
  //              qDebug() << "Install";
    //            qDebug() << code;
      //      });

            webServiceTools.computeJsonValuePost(url, sessionKey);

            const QJsonObject &jsonInstall = webServiceTools.getJsonValue();

            if (jsonInstall.size() > 0) {

                createInstallationKeyFile(code, orgId, locationID);

            }
            emit installationDataLoaded(jsonInstall);
            //Sorry Jon I don't know the format Name, log source or etc.. value;
            //loggingTools->createLogs();
        }

    } catch (...) {

        emit installationDataLoaded(QJsonObject());

    }
}

void InstallationTools::loginUsername(const QString &username, const QString &password,
                                      const int &organizationID)
{
    const QString &url = webServiceTools.getApiUrlNavs() + "api/users/login?"
             "login_type_id=" + QString::number(getInstallationTypeID()) +
             "&organization_id=" + QString::number(organizationID) +
             "&ip=" + getIPAddress() +
             "&username=" + username +
             "&password=" + password;

    //qDebug() << url;

    webServiceTools.computeJsonValuePost(url);

    const QJsonObject &jsonResult = webServiceTools.getJsonValue();

    if(jsonResult.length() > 0){
        emit loginUsernameLoaded(jsonResult);
        //Sorry Jon I don't know the format Name, log source or etc.. value;
        //loggingTools->createLogs();

    }else{
        emit loginUsernameErrorLoaded("Login system is down");
    }
}

int InstallationTools::getInstallationTypeID()
{
    const QString &url = webServiceTools.getApiUrlNavs() + "api/installation-types/1";

    webServiceTools.computeJsonValueGet(url);

    const QJsonObject &jsonInstallationType = webServiceTools.getJsonValue();

    if (jsonInstallationType.size() > 0)
    {
        return jsonInstallationType["id"].toInt();
    }

    return 1;
}

QString InstallationTools::getIPAddress() const
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
              return address.toString();
    }
    return QStringLiteral("192.168.0.1");
}

int InstallationTools::getReleaseID(const QString &sessionKey)
{
    const QString &url = webServiceTools.getApiUrlNavs() + "api/releases/1";

    webServiceTools.computeJsonValueGet(url, sessionKey);

    const QJsonObject &jsonRelease = webServiceTools.getJsonValue();

    if (jsonRelease.size() > 0)
    {
        return jsonRelease["id"].toInt();
    }

    return 1;
}

QByteArray InstallationTools::encryptInstallation(const QString &code){
    return code.toUtf8();
}

QString InstallationTools::decryptInstallation() const {
    QFile file(fileData());
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        return QStringLiteral("");
    }
    const QByteArray encodeText = file.readAll();
    return  QString::fromStdString(encodeText.toStdString());
}

void InstallationTools::getAvailableVersionNumberXML()
{
    QEventLoop eventLoop;
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req;

    QSettings settings;
    settings.beginGroup("Servers");
    QVariant value = settings.value("update_url", "http://app.navsnow.com/repository/Updates.xml");
    settings.endGroup();
    QString res = value.toString();

    //qDebug() << "Chgecking " << res << "for updates";

    req.setUrl(QUrl(res));

    QNetworkReply *reply = mgr->get(req);
    eventLoop.exec();\
    const QString xml =static_cast<QString>(reply->readAll());
    QXmlStreamReader reader(xml);
    reply->abort();

    //mgr->destroyed(this);
    //reply->destroyed(this);

    try {
        if (reader.readNextStartElement()) {
                if (reader.name() == QStringLiteral("Updates")){
                    while(reader.readNextStartElement()){
                        if(reader.name() == QStringLiteral("PackageUpdate")){
                            while(reader.readNextStartElement()){
                                if(reader.name() == QStringLiteral("Version")){
                                    const QString s = reader.readElementText();

                                    //qDebug() << "Current version = " << s;

                                    emit foundAvailableVersion(s);
                                    return;
                                }
                                else
                                    reader.skipCurrentElement();
                            }
                        }
                        else
                            reader.skipCurrentElement();
                    }
                }
                else
                    reader.raiseError(QObject::tr("Incorrect file"));
        }
    } catch (...) {
        //qDebug() << "Parse XML failed";
    }

    emit foundAvailableVersion(QStringLiteral(""));
}

void InstallationTools::requestErrorOccurred(QNetworkReply::NetworkError code)
{
    //qDebug() << "Installation Tools Caught Network error. Sending on.";

    emit networkError(code);
}


