#include "backend.h"
#include "webServicesTools/webservicestools.h"
#include <QDebug>
#include <QJsonObject>
#include "notificationinfo.h"
#include "notificationsmodel.h"
#include <QDateTime>
#include <WinBase.h>
#include <stdlib.h>

Backend::Backend(QObject *parent) : QObject(parent),
    m_navs(new NavsObject())
{
    bootstrapSettingsFile();

    QObject::connect(&userTools, &UserTools::organizationDnsKeyStatusLoaded, this,
                     &Backend::organizationDnsKeyStatusLoaded);
    QObject::connect(&userTools, &UserTools::organizationDataLoaded, this,
                     &Backend::organizationLoaded);
    QObject::connect(&installationTools, &InstallationTools::registerStatusLoaded, this,
                     &Backend::registerStatusLoaded);
    QObject::connect(&installationTools, &InstallationTools::installationDataLoaded, this,
                     &Backend::installationDataLoaded);
    QObject::connect(&userTools, &UserTools::loginWindowsUsernameLoaded, this,
                    &Backend::loginWindowsUsernameLoaded);
    QObject::connect(&installationTools, &InstallationTools::loginUsernameLoaded, this,
                    &Backend::loginUsernameLoaded);
    QObject::connect(&loggingTools, &LoggingTools::logLoaded, this,
                     &Backend::logLoaded);
    QObject::connect(&appTools, &AppTools::appLoaded, this,
                     &Backend::appLoaded);
    QObject::connect(&appTools, &AppTools::appQuickLoaded, this,
                     &Backend::appQuickLoaded);
    QObject::connect(&appTools, &AppTools::welcomeLoaded, this,
                     &Backend::welcomeLoaded);
    QObject::connect(&appTools, &AppTools::addAppQuickLoaded, this,
                     &Backend::addAppQuickLoaded);
    QObject::connect(&appTools, &AppTools::deleteAppQuickLoaded, this,
                     &Backend::deleteAppQuickLoaded);
    QObject::connect(&appTools, &AppTools::checkAppQuickLoaded, this,
                     &Backend::checkAppQuickLoaded);
    QObject::connect(&appTools, &AppTools::profileLoaded, this,
                     &Backend::profileLoaded);
    QObject::connect(&appTools, &AppTools::sendInitialNotificationsLoaded, this,
                        &Backend::initialNotificationsLoaded);
    QObject::connect(&userTools, &UserTools::meDataLoaded, this,
                        &Backend::setMeData);
    QObject::connect(&appTools, &AppTools::newNotificationCaught, this,
                        &Backend::catchNewNotification);
    QObject::connect(&appTools, &AppTools::notificationDeleted, this,
                        &Backend::catchNotificationDeleted);
    QObject::connect(&appTools, &AppTools::newNotificationInfoItem, this,
                        &Backend::catchNewNotificationInfoItem);
    QObject::connect(&appTools, &AppTools::updatedNotificationInfoItem, this,
                        &Backend::catchUpdatedNotificationInfoItem);
    QObject::connect(&installationTools, &InstallationTools::foundAvailableVersion, this,
                        &Backend::foundAvailableVersion);
    QObject::connect(m_navs, &NavsObject::resetBadgeCountAction, this,
                        &Backend::requestAppBagdeCount);
    QObject::connect(&appTools, &AppTools::resetBadgeCountAction, this,
                        &Backend::requestAppBagdeCount);
    QObject::connect(&appTools, &AppTools::badgeCountRetrieved, this,
                     &Backend::badgeCountReceived);
    QObject::connect(&userTools, &UserTools::writeLogItem, this,
                    &Backend::writeLogItem);
    QObject::connect(m_navs, &NavsObject::appLoaded, this,
                        &Backend::appLoaded);
    QObject::connect(m_navs, &NavsObject::appQuickLoaded, this,
                        &Backend::appQuickLoaded);

    //Error Message
    connect(&userTools, &UserTools::organizationDNSKeyErrorLoaded, this,
            &Backend::organizationDNSKeyErrorLoaded);
    connect(&userTools, &UserTools::organizationByIDErrorLoaded, this,
            &Backend::organizationByIDErrorLoaded);
    connect(&userTools, &UserTools::loadMeDataErrorLoaded, this,
            &Backend::loadMeDataErrorLoaded);
    connect(&userTools, &UserTools::loginWindowsUsernameErrorLoaded, this,
            &Backend::loginWindowsUsernameErrorLoaded);
    connect(&appTools, &AppTools::appDataErrorLoaded, this,
            &Backend::appDataErrorLoaded);
    connect(&appTools, &AppTools::quickAppDataErrorLoaded, this,
            &Backend::quickAppDataErrorLoaded);
    connect(&appTools, &AppTools::welcomePageDataErrorLoaded, this,
            &Backend::welcomePageDataErrorLoaded);
    connect(&appTools, &AppTools::addQuickAppFailLoaded, this,
            &Backend::addQuickAppFailLoaded);
    connect(&appTools, &AppTools::deleteQuickAppFaileLoaded, this,
            &Backend::deleteQuickAppFaileLoaded);
    connect(&installationTools, &InstallationTools::loginUsernameErrorLoaded, this,
            &Backend::loginUsernameErrorLoaded);
    //Refresh app and quick app error
    connect(m_navs, &NavsObject::appDataErrorLoaded, this,
            &Backend::appDataErrorLoaded);
    connect(m_navs, &NavsObject::quickAppDataErrorLoaded, this,
            &Backend::quickAppDataErrorLoaded);

    //Network Errors
    connect(&webServicesTools, &WebServicesTools::networkError, this,
            &Backend::networkError);
    connect(&appTools, &AppTools::networkError, this,
            &Backend::networkError);
    connect(&userTools, &UserTools::networkError, this,
            &Backend::networkError);
    connect(&installationTools, &InstallationTools::networkError, this,
            &Backend::networkError);
}



void Backend::checkCharlieProcess(){
    if(orgLoadComplete && getSessionKey() != ""){
        //Uncomment for Release build
        charlieTester.checkCheckCharlie1();
        //charlieTester.checkCheckCharlie2();-----Not used yet
    }
}

void Backend::createSettingsFile()
{
    const char file[13] = "settings.ini";
    QFile config(file);
    QSettings *iniFile = new QSettings(file, QSettings::IniFormat);
    if (config.open(QIODevice::ReadWrite)) {
        iniFile->beginWriteArray("NAVSEnv");

        //Default Value
        NAVSEnv navsenv;
        navsenv.name = "production";
        navsenvdata.append(navsenv);

        for (int i = 0; i < navsenvdata.size(); ++i) {
            iniFile->setArrayIndex(i);
            iniFile->setValue("Name", navsenvdata.at(i).name);
        }
        iniFile->endArray();
        config.close();
    }
}

void Backend::bootstrapSettingsFile()
{
    QString result;
    const char file[13] = "settings.ini";
    QFile config(file);
    QSettings *iniFile = new QSettings(file, QSettings::IniFormat);
    if (!config.exists()) {
        createSettingsFile();
    }
    const int &size = iniFile->beginReadArray("NAVSEnv");
    for (int i = 0; i < size; i++) {
        iniFile->setArrayIndex(i);
        result = iniFile->value("Name").toString();
    }
    iniFile->endArray();

    const QString update_url = "http://app.navsnow.com/repository/Updates.xml";
    //QString update_url = "http://localhost/repository/Updates.xml";

    const QString beta1_url = (result == "test") ?
                "https://beta1.navsnow.com/":
                "https://api.navsnow.com/";

    const QString notifications_url = (result == "test") ?
                 "http://socket.navsnow.com:80":
                //"https://jonsocket.navsnow.com:5000":
                "https://notificationsprod.navsnow.com:443";

    //qDebug() << notifications_url;

    const QString echo2_url = (result == "test") ?
                "https://echo2.navsnow.com/api/documents/":
                "https://cubackups.navsnow.com/api/documents/";

    const char group[8] = "Servers";
    //Creating QSettings object
    QSettings settings;
    settings.beginGroup(group);
    settings.setValue("update_url",update_url);
    settings.setValue("beta1_url",beta1_url);
    settings.setValue("notifications_url",notifications_url);
    settings.setValue("echo2_url",echo2_url);
    settings.endGroup();

    //iniFile->destroyed(this);
}

ErrorMessage *Backend::errorMessage() const
{
    return m_errorMessage;
}

//Error handling-------------

void Backend::appDataErrorLoaded(const QString &code)
{
    m_errorMessage->setAppDataError(code);
    emit errorMessageChanged(1001);
}

void Backend::quickAppDataErrorLoaded(const QString &code)
{
    m_errorMessage->setQuickAppDataError(code);
    emit errorMessageChanged(1002);
}

void Backend::welcomePageDataErrorLoaded(const QString &code)
{
    m_errorMessage->setWelcomeDataError(code);
    emit errorMessageChanged(1003);
}

void Backend::addQuickAppFailLoaded(const QString &code)
{
    m_errorMessage->setaddQuickAppDataError(code);
    emit errorMessageChanged(1004);
}

void Backend::deleteQuickAppFaileLoaded(const QString &code)
{
    m_errorMessage->setDeleteQuickAppDataError(code);
    emit errorMessageChanged(1005);
}

void Backend::organizationDNSKeyErrorLoaded(const QString &code)
{
    m_errorMessage->setOrganizationDNSKeyError(code);
    emit errorMessageChanged(1006);
}

void Backend::organizationByIDErrorLoaded(const QString &code)
{
    m_errorMessage->setOrganizationByIDError(code);
    emit errorMessageChanged(1007);
}

void Backend::loginWindowsUsernameErrorLoaded(const QString &code)
{
    m_errorMessage->setLoginWindowsUsernameDataError(code);
    emit errorMessageChanged(1008);
}

void Backend::loadMeDataErrorLoaded(const QString &code)
{
    m_errorMessage->setLoadMeDataError(code);
    emit errorMessageChanged(1009);
}

void Backend::loginUsernameErrorLoaded(const QString &code)
{
    m_errorMessage->setLoginUsernamePasswordDataError(code);
    emit errorMessageChanged(1010);
}

void Backend::networkError(QNetworkReply::NetworkError code)
{
    //qDebug() << "Backend Catch network error: " << code;

    if(code == QNetworkReply::UnknownNetworkError ||
            code == QNetworkReply::TemporaryNetworkFailureError ||
            code == QNetworkReply::NetworkSessionFailedError ||
            code == QNetworkReply::ProxyTimeoutError ||
            code == QNetworkReply::ProxyNotFoundError ||
            code == QNetworkReply::ProxyConnectionClosedError ||
            code == QNetworkReply::ProxyAuthenticationRequiredError){
        m_errorMessage->setNetworkError("Please check your network connection and try again.");
        emit errorMessageChanged(1011);
    }else if(code == QNetworkReply::ConnectionRefusedError ||
            code == QNetworkReply::RemoteHostClosedError ||
            code == QNetworkReply::ContentAccessDenied ||
            code == QNetworkReply::ContentOperationNotPermittedError ||
             code == QNetworkReply::InternalServerError ||
             code == QNetworkReply::ServiceUnavailableError ||
             code == QNetworkReply::UnknownServerError
            ){
        m_errorMessage->setNetworkError("Error handling request. Please try your request again later.");
        emit errorMessageChanged(1012);
    }else if(code == QNetworkReply::AuthenticationRequiredError){
         m_errorMessage->setNetworkError("Your NAVS Session is no longer valid.  Please log in again.");
         emit errorMessageChanged(1013);
     }

}

//End error handling-------------


QString Backend::betaUrl()
{
    return webServicesTools.getBetaUrl();
}

OrganizationModel *Backend::organizationData() const
{    
    return m_organizationData;
}

void Backend::setOrganizationData(OrganizationModel *organizationData)
{
    m_organizationData = organizationData;

    emit organizationDataChanged();
    orgLoadComplete = true;
}

void Backend::loadOrganization(const QString sessionKey)
{
    int orgID = installationTools.getOrganizationID();
    userTools.loadOrganizationByID(orgID, sessionKey);
}

void Backend::organizationLoaded(OrganizationModel *organizationData)
{
    setOrganizationData(organizationData);
}


//Get index from Organization Model.
int Backend::getIndexOrganization(const QString &oranizationLocationName)
{
    for (int i = 0; i < organizationData()->count(); i++ ) {
        if (organizationData()->get(i)->location().toUpper().trimmed()
                == oranizationLocationName.toUpper().trimmed())
            return i;
    }
    //Not exists
    return -1;
}

QString Backend::getOrgName(){
    return organizationData()->get(0)->name();
}

QString Backend::getOrgIcon()
{
    return organizationData()->get(0)->icon();
}

QString Backend::getAppTitle(const int appId)
{
    return appTools.getAppTitle(appId);
}

QString Backend::getAppURL(const int appId)
{
    return appTools.getAppURL(appId);
}


QJsonObject Backend::installationData() const
{
    return m_installationData;
}

void Backend::setInstallationData(const QJsonObject &installationData)
{
    m_installationData = installationData;
    emit installationDataChanged();
}

void Backend::sendInstallationDataToBeta1(QString organizationLocation, int idx)
{
    if (organizationLocation.isEmpty())
        installationTools.sendInstallationData(organizationData(),
                                               idx,
                                               loginData()["session_key"].toString());
    else
        installationTools.sendInstallationData(organizationData(),
                                               getIndexOrganization(organizationLocation),
                                               loginData()["session_key"].toString());
}

//This code uses to delete the installation key.
void Backend::deleteInstallationData()
{
    installationTools.deleteInstallationID();
}

void Backend::installationDataLoaded(const QJsonObject &installationData)
{
    setInstallationData(installationData);
}

bool Backend::registerStatus() const
{
    return m_registerStatus;
}

void Backend::setRegisterStatus(const bool &registerStatus)
{
    m_registerStatus = registerStatus;
    emit registerStatusChanged();
}

void Backend::checkRegisterStatus()
{
    //deleteInstallationData();
    installationTools.checkregisterStatus();
}

void Backend::registerStatusLoaded(const bool &registerStatus)
{
    setRegisterStatus(registerStatus);
}

bool Backend::organizationDnsKeyStatus() const
{
    return m_organizationDnsKeyStatus;
}

void Backend::setOrganizationDnsKeyStatus(bool organizationDnsKeyStatus)
{
    m_organizationDnsKeyStatus = organizationDnsKeyStatus;
    emit organizationDnsKeyStatusChanged();
}

void Backend::checkOrganizationDnsKeyStatus(const QString &s_organizationDnsKey)
{
    userTools.checkOrganization(s_organizationDnsKey);
}

void Backend::organizationDnsKeyStatusLoaded(const bool &organizationDnsKeyStatus)
{
    setOrganizationDnsKeyStatus(organizationDnsKeyStatus);
}

void Backend::loginWindowsUsername()
{
    userTools.loginWindowsUsername();
}

void Backend::loginWindowsUsernameLoaded(const QJsonObject &loginData)
{
    setLoginData(loginData);
    m_navs->setSessionKey(getSessionKey());
}

void Backend::loginUsernameLoaded(const QJsonObject &data)
{
    if(appTools.appsLoaded()){
        m_appData->deleteAll();
        m_appQuickData->deleteAll();
        m_notificationsData->deleteAll();
        appTools.closeSocketConnection();
    }

    setLoginData(data);
    m_navs->setSessionKey(getSessionKey());
}

void Backend::writeLogItem(const QString logItem)
{
    emit writeQMLLogItem(logItem);
}

void Backend::catchNotificationDeleted(const QString notificationID)
{
    for(int i=0; i<m_notificationsData->count(); i++){
        try {
            if(m_notificationsData->get(i)->id() == notificationID){
               m_notificationsData->deleteItem(i);
               return;
            }
        } catch (...) {
               //qDebug() << "error 1";
        }
    }
}

void Backend::createAppFilterModel(const QString categoryName)
{
    AppFilterProxy *filterModel = new AppFilterProxy();
    filterModel->setSourceModel(m_appData);
    filterModel->setFilterRole(m_appData->CategoryRole);
    filterModel->setSortRole(m_appData->CategoryRole);
    filterModel->setFilterString(categoryName);
    filterModel->setDynamicSortFilter(true);
    appFilterModels << filterModel;
}

int Backend::getAppIndexFromAppInstallId(const int appInstallId)
{
    for(int i = 0; i<m_appData->count(); i++){
        try {
            if(m_appData->get(i)->appInstallId() == appInstallId){
                return i;
            }
        } catch (...) {
               //qDebug() << "error 1";
        }

    }
    return -1;
}

QJsonObject Backend::loginData() const
{
    return m_loginData;
}

void Backend::setLoginData(const QJsonObject &data)
{
    //qDebug() << data;
    //qDebug() << installationTools.getLocationID();

    m_loginData = data;
    emit loginDataChanged();
}

void Backend::loginUsername(const QString &username, const QString &password)
{
    installationTools.loginUsername(username, password, organizationData()->get(0)->organizationID());
}

void Backend::loginUsernameSecondary(const QString &username, const QString &password)
{
    //Get the organization ID from the installation file.
    const int orgId = installationTools.getOrganizationID();
    installationTools.loginUsername(username, password, orgId);
}

QJsonObject Backend::logData() const
{
    return m_log;
}

void Backend::setLogData(const QJsonObject &log)
{
    m_log = log;
    emit logChanged();
}

void Backend::createLog(const QString &formatName, const QString &logSource, const QString &severity,
                        const QString &errorMessage)
{
    loggingTools.createLogs(formatName,
                            logSource,
                            severity,
                            errorMessage,
                            getValueFromColumnNameJSON("organization_id",loginData()),
                            getValueFromColumnNameJSON("id",installationData()));
}

void Backend::logLoaded(const QJsonObject &logData)
{
    setLogData(logData);
}


AppModel *Backend::appData() const
{
    return m_appData;
}

void Backend::setAppData(AppModel *newAppData)
{
    m_appData = newAppData;

    if(appFilterModels.length() < 1){
        QStringList appCategories = appTools.getAppCategories();
        for(int i=0; i<appCategories.length(); i++){
            createAppFilterModel(appCategories[i]);
        }
    }

    emit appDataChanged();
}

void Backend::loadApps()
{
    if(!isCUAdmin())
        appTools.getAllAppInstall(loginData()["user"].toObject()["organizationUser"].
                                toObject()["id"].toInt(),
                             installationTools.getLocationID(),
                             getSessionKey());

    //Testing the value
    //qDebug() << webServicesTools.getServerName();
}

void Backend::appLoaded(AppModel *newAppData)
{
    setAppData(newAppData);
}


AppModel *Backend::appQuickData() const
{
    //qDebug() << m_appQuickData->count();
    return m_appQuickData;
}

void Backend::setAppQuickData(AppModel *newAppQuickData)
{
    m_appQuickData = newAppQuickData;
    emit appQuickDataChanged();
}

void Backend::appQuickLoaded(AppModel *newAppQuickData)
{
    setAppQuickData(newAppQuickData);
}


int Backend::getValueFromColumnNameJSON(const QString &name, const QJsonObject &json)
{
    if(!json[name].isUndefined() && !json[name].isNull())
    {
        return json[name].toInt();
    }
    return -1;
}

int Backend::getValueFromColumnNameJSON(const QString &name, const QString name2,
                                            const QJsonObject &json)
{
    if(!json[name].isUndefined() && !json[name].isNull() && !json[name][name2].isUndefined() && !json[name][name2].isNull())
    {
        return json[name][name2].toInt();
    }
    return -1;
}


QString Backend::welcomeData() const
{
    return m_welcome;
}

void Backend::setWelcomeData(QString welcome)
{
    m_welcome = welcome;
    emit welcomeChanged();
}

void Backend::loadWelcome()
{
   const QString sKey = getSessionKey();
   appTools.loadWelcome(sKey,loginData());
}

void Backend::welcomeLoaded(const QString &welcome)
{
    setWelcomeData(welcome);
}

QJsonObject Backend::addAppQuickLaunchData() const
{
    return m_addAppQuickLaunch;
}

void Backend::setAddAppQuickLaunchData(const QJsonObject &newAppQuickLaunch)
{
    m_addAppQuickLaunch = newAppQuickLaunch;
    emit addAppQuickChanged();
}

void Backend::addAppQuick(const int &id)
{
    if(!isCUAdmin()){
        const int appIndex = getAppIndexFromAppInstallId(id);

        m_appQuickData->append(m_appData->get(appIndex));

        //qDebug() << "id = " << id << ", index = " << appIndex;

        if(appIndex != -1){
            m_appData->deleteItem(appIndex);
            appTools.addAppQuickLaunch(id,
                                loginData()["user"].toObject().find("organizationUser")->toObject().find("id")->toInt(),
                                0,
                                getSessionKey());
        }
    }

}

void Backend::addAppQuickLoaded(const QJsonObject &json)
{
    const int val = json.find("app_installation_id")->toString().toInt();

    for(int p = 0; p<m_appQuickData->count(); p++){
        try {
            if(m_appQuickData->get(p)->appInstallId() == val){
                m_appQuickData->get(p)->setAppQuickLaunchId(json.find("id")->toInt());
            }
        } catch (...) {
               qDebug() << "error 2";
        }
    }
    setAddAppQuickLaunchData(json);
}


QJsonObject Backend::deleteAppQuickLaunchData() const
{
    return m_deleteAppQuickLaunch;
}

void Backend::setDeleteAppQuickLaunchData(const QJsonObject &appQuickLaunch)
{
    m_deleteAppQuickLaunch = appQuickLaunch;
    emit deleteAppQuickChanged();
}

void Backend::deleteAppQuick(const int &appId)
{
    if(!isCUAdmin()){
        const int qAppId = m_appQuickData->get(appId)->appQuickLaunchId();
        m_appData->append(m_appQuickData->get(appId));
        m_appQuickData->deleteItem(appId);
        appTools.deleteAppQuickLaunch(qAppId, getSessionKey());
    }
}

void Backend::deleteAppQuickLoaded(const QJsonObject &json)
{
    setDeleteAppQuickLaunchData(json);
}


bool Backend::checkAppQuickData() const
{
    return m_checkAppQuick;
}

void Backend::setCheckAppQuickData(const bool &result)
{
    m_checkAppQuick = result;
    emit checkAppQuickChanged();
}

void Backend::checkAppQuick(const int &appId)
{
    appTools.checkAppQuick(appId);
}

void Backend::checkAppQuickLoaded(const bool &result)
{
    setCheckAppQuickData(result);
}

QQuickWebEngineProfile *Backend::profileData() const
{
    return m_profileData;
}

void Backend::setProfileData(QQuickWebEngineProfile *newProfile)
{
    m_profileData = newProfile;
    emit profileChanged();
}

void Backend::loadProfile()
{
    appTools.loadProfile(getSessionKey());
}

void Backend::profileLoaded(QQuickWebEngineProfile *newProfile)
{
    setProfileData(newProfile);
}

void Backend::setMeData(const QJsonObject &meData)
{
    m_meData = meData;
    emit meDataChanged();
}


QJsonObject Backend::meData() const
{
    return m_meData;
}

void Backend::loadMeData()
{
    QString sKey = getSessionKey();
    userTools.loadMeData(sKey);
    //At the same time, we'll load the organization.
    if(!orgLoadComplete){
        loadOrganization(sKey);
    }
}

void Backend::startInitialSocketConnection(){
    if(isCUAdmin()){
        return;
    }

    //qDebug() << loginData();

    appTools.initialSocketConnection(loginData()["user"].toObject()["organizationUser"].
            toObject()["id"].toInt(), getSessionKey());
}

void Backend::initialNotificationsLoaded(NotificationsModel *notificationsModel){
    setNotificationsData(notificationsModel);
}

void Backend::setNotificationsData(NotificationsModel *notModel){
    m_notificationsData = notModel;
    emit notificationsDataChanged();
}

NotificationsModel *Backend::notificationsData() const
{
    return m_notificationsData;
}

QString Backend::getSessionKey()
{
    if(loginData().isEmpty() == false)
        return loginData()["session_key"].toString();
    else
        return "";
}

bool Backend::isCUAdmin(){
    return getProfileTypeId() == 1;
}

int Backend::getProfileTypeId()
{
    return userTools.getLoginTypeID(loginData());
}


NavsObject *Backend::navsObject() const
{
    return m_navs;
}


QString Backend::getJsCode(const QString jsFileName) const
{
    QString dirPath = QDir::toNativeSeparators(QDir::currentPath());
    QResource common(":/js/"+jsFileName);
    QFile apiFile(common.absoluteFilePath());
    if(!apiFile.open(QIODevice::ReadOnly)){
        //qDebug() <<  "unable to open "+jsFileName;
        return "";
    }else{
        //qDebug() << "successfully opened "+jsFileName;
        QByteArray contents = apiFile.readAll();
        QTextStream in(&contents);
        QString apiScript = in.readAll();
        apiFile.close();
        return apiScript;
    }
}

void Backend::updateNotificationDeliveryDate(QJsonObject communication)
{
    communication["delivered_date"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonObject innerData {
        {"target_url", communication["actionUrl"]},
        {"message_body", communication["messageBody"]}
    };
    QJsonObject outbound
    {
        {"notification_id", communication["id"]},
        {"app_id" , communication["appId"]},
        {"organization_user_id" , communication["organization_user_id"]},
        {"message" , communication["messageTitle"]},
        {"data" , innerData},
        {"action_name", communication["callToAction"]},
        {"delivered_date", communication["delivered_date"]},
    };

    appTools.updateNotificationDeliveryDate(outbound);
}

void Backend::deleteNotification(const int notification_id, const int notificationIndex)
{
    QJsonObject notDetails {
        {"notification_id", notification_id},
        {"organization_user_id", m_meData["organizationUser"].toObject()["id"].toInt()}
    };
    appTools.deleteNotification(notDetails);
    m_notificationsData->deleteItem(notificationIndex);
}

void Backend::launchUpdate()
{
//If we wanted to do it and wait-----------
//    QProcess *process = new QProcess(this);
//    process->setProgram("\"C:\\Program Files\\CU Northwest\\NAVS\\maintenancetool.exe\"");
//    process->start();
//    process->waitForFinished(-1);
//----------------
    //If we want to do it using the maintenance tool.
//    QProcess::startDetached("\"C:\\Program Files\\CU Northwest\\NAVS\\maintenancetool.exe\"");
//    QProcess::startDetached("taskkill /IM \"NAVS_Service_Helper.exe\" /F");
//    emit updateCompleted();

    //Create the file that Charlie2 will use to see that the update needs to happen right now.
    QFile file("C:\\Users\\Public\\Documents\\update_initiated.txt");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        // Create the file.
        qDebug() << "Create the update initiated file.";
        QSaveFile file("C:\\Users\\Public\\Documents\\update_initiated.txt");

        //Check if file open is true
        if(!file.open(QIODevice::WriteOnly)){
            qDebug() << "Unable to Create a file";
        } else{
            QString updateTimer = QDateTime::currentDateTime().toString(Qt::ISODate);

            file.write(updateTimer.toUtf8());
            file.commit();
        }
    }
    //QProcess::startDetached("\"C:\\Program Files\\CU Northwest\\NAVS\\updatemessage.exe\"");//---------------------
    emit updateCompleted();
}

void Backend::getAvailableVersionNumber()
{
    installationTools.getAvailableVersionNumberXML();
}

void Backend::updateAllAppBadges()
{
    if(isCUAdmin()){
        return;
    }
    const QString myOrgUserId = QString::number(m_meData["organizationUser"].toObject()["id"].toInt());
    appTools.runAllBadgeUpdate(loginData()["session_key"].toString(), myOrgUserId);
}

bool Backend::isTestEnv()
{
    QSettings settings;
    settings.beginGroup("Servers");
    QVariant value = settings.value("beta1_url", "https://beta1.navsnow.com/");
    settings.endGroup();
    //Convert from QString to std string
    if(strcmp(value.toString().toUtf8().constData(), "https://api.navsnow.com/") == 0){
        return false;
    }
    return true;
}

void Backend::startUpdateClock()
{
    //Check if the file already exists.
    //Create it if not.
    QFile file("C:\\Users\\Public\\Documents\\update_available.txt");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        // Create the file.
        qDebug() << "Create the update file.";
        QSaveFile file("C:\\Users\\Public\\Documents\\update_available.txt");

        //Check if file open is true
        if(!file.open(QIODevice::WriteOnly)){
            qDebug() << "Unable to Create a file";
        } else{
            QString updateTimer = QDateTime::currentDateTime().toString(Qt::ISODate);

            file.write(updateTimer.toUtf8());
            file.commit();
        }
    }
    //Example comparing the dates.
//    else{
//        //Read the date in the file.
//        QByteArray encodeText = file.readAll();
//        QString dte = QString::fromStdString(encodeText.toStdString());
//        qDebug() << "Read the update file = " << dte;
//        QDateTime uDate = QDateTime::fromString(dte, "yyyy-MM-ddTHH:mm:ss");
//        QDateTime nw = QDateTime::currentDateTime();
//        qint64 millisecondsDiff = uDate.msecsTo(nw);

//        if(millisecondsDiff > 864000000){
//            //Up[date now
//        }
    //    }
}

void Backend::installCharlie2()
{
    const QString pth = "powershell";
    ShellExecuteA(nullptr, "runas", pth.toUtf8().constData(),"-executionpolicy bypass \"& \"\"C:\\Program Files\\CU Northwest\\NAVS\\NAVS_SUPPORT_SERVICE\\NAVS_PATH.ps1\"\"\"", nullptr, SW_SHOWNORMAL);
}

QStringList Backend::getAppCategories()
{
    return appTools.getAppCategories();
}

AppFilterProxy* Backend::getAppFilterProxyByIndex(const int idx)
{
    return appFilterModels[idx];
}

void Backend::deleteAllCategoryProxyModels()
{
    if(appFilterModels.length() > 0){
        appFilterModels.clear();
    }
}

void Backend::catchNewNotification(QJsonObject new_notification){
    emit newNotificationConveyer(new_notification);
}

void Backend::catchNewNotificationInfoItem(NotificationInfo* newItm){
    m_notificationsData->insert(newItm, 0);
}

void Backend::catchUpdatedNotificationInfoItem(NotificationInfo *updatedItm)
{
    for(int i = 0; i<m_notificationsData->count(); i++){
        try {
            if(m_notificationsData->get(i)->id() == updatedItm->id()){
                m_notificationsData->deleteItem(i);
                m_notificationsData->insert(updatedItm, i);

                qDebug() << "Update successful.";

                return;
            }
        } catch (...) {
               //qDebug() << "error 1";
        }
    }
}

void Backend::foundAvailableVersion(QString versionNumber)
{
    emit versionInfoFound(versionNumber);
}

void Backend::requestAppBagdeCount(const int appId)
{
    if(isCUAdmin()){
        return;
    }
    QString url = "";
    const QString myOrgUserId = QString::number(m_meData["organizationUser"].toObject()["id"].toInt());

    for(int i = 0; i<m_appData->count(); i++){
        try {
            if(m_appData->get(i)->appId() == appId){
                url = m_appData->get(i)->url() + "/api/notifications-count?organization_user_id=" + myOrgUserId;
                appTools.requestSingleAppBadgeUpdate(url, appId, loginData()["session_key"].toString());
                return;
            }
        } catch (...) {
               //qDebug() << "error 1";
        }
    }
    for(int p = 0; p<m_appQuickData->count(); p++){
        try {
            if(m_appQuickData->get(p)->appId() == appId){
                url = m_appQuickData->get(p)->url() + "/api/notifications-count?organization_user_id=" + myOrgUserId;
                appTools.requestSingleAppBadgeUpdate(url, appId, loginData()["session_key"].toString());
                return;
            }
        } catch (...) {
               //qDebug() << "error 2";
        }
    }
}

void Backend::badgeCountReceived(const int appId, const int badgeTotal)
{
    int totalBadgeCount = 0;
    //Find the model it's in. then update the item.
    for(int i = 0; i<m_appData->count(); i++){
        try {
            if(m_appData->get(i)->appId() == appId){
                m_appData->get(i)->setBadgeCount(badgeTotal);
                totalBadgeCount += badgeTotal;
            }else{
                int bd1 = m_appData->get(i)->badgeCount();
                if(bd1 > 0){
                    totalBadgeCount += bd1;
                }
            }
        } catch (...) {
               //qDebug() << "error 1";
        }

    }
    for(int p = 0; p<m_appQuickData->count(); p++){
        try {
            if(m_appQuickData->get(p)->appId() == appId){
                m_appQuickData->get(p)->setBadgeCount(badgeTotal);
                 totalBadgeCount += badgeTotal;
            }else{
                int bd1 = m_appQuickData->get(p)->badgeCount();
                if(bd1 > 0){
                    totalBadgeCount += bd1;
                }
            }
        } catch (...) {
              //qDebug() << "8" << "error 2";
        }
    }

    emit totalBadgeCountChanged(totalBadgeCount);
}
