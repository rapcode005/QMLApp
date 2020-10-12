#include "backend.h"
#include "webServicesTools/webservicestools.h"
#include <QDebug>
#include <QJsonObject>

Backend::Backend(QObject *parent) : QObject(parent)
{
    //WebServicesTools *webTools; //The web services will not be used in this class.
    //The Backend class is simply an API abstraction layer.
    //It exposes methods, data models, and properties.
    //All business logic is done in classes like UserTools, AppTools, LoggingTools, InstallationTools etc.
    //We connect signals in those classes to slots in the Backend class.
    //An instance of WebServicesTools will likely be needed in each of these tool classes.

    //Connect the User Load
    QObject::connect(&userTools, SIGNAL(userDataLoaded(QJsonObject)), this, SLOT(userLoaded(QJsonObject)));
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
    /*QObject::connect(&userTools, &UserTools::logoutOrganizationLoaded, this,
                     &Backend::logoutOrganizationLoaded);*/
    QObject::connect(&loggingTools, &LoggingTools::logLoaded, this,
                     &Backend::logLoaded);
    QObject::connect(&appTools, &AppTools::appLoaded, this,
                     &Backend::appLoaded);
    QObject::connect(&appTools, &AppTools::appQuickLoaded, this,
                     &Backend::appQuickLoaded);
}

/*
 * User Functions
 */

//userData Getter
QJsonObject Backend::userData() const {
    return m_userData;
}

//userData Setter
void Backend::setUserData(const QJsonObject &userData){
    m_userData = userData;
    //Notify QML that the user data has changed.
    emit userDataChanged();
}

//Load the user (Called from QML)
void Backend::loadUser(){
    //Call the userTools functions to get the user data.
    userTools.loadUser();
}

//User load is complete
void Backend::userLoaded(const QJsonObject &newUserData){
    setUserData(newUserData);
}

OrganizationModel *Backend::organizationData() const
{    
    return m_organizationData;
}

void Backend::setOrganizationData(OrganizationModel *organizationData)
{
    m_organizationData = organizationData;

    emit organizationDataChanged();
}

void Backend::loadOrganization(const QString &s_organization)
{
    userTools.loadOrganization(s_organization);
}

void Backend::organizationLoaded(OrganizationModel *organizationData)
{
    setOrganizationData(organizationData);
}


//Get index from Organization Model.
int Backend::getIndexOrganization(const QString &oranizationLocationName)
{
    for (int i = 0; i < organizationData()->count(); i++ ) {
        //qDebug() << organizationData()->get(i)->location();
        if (organizationData()->get(i)->location().toUpper() == oranizationLocationName.toUpper())
            return i;
    }
    //Not exists
    return -1;
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

void Backend::loadInstallationData(QString organizationLocation)
{
    if (organizationLocation.isEmpty())
        installationTools.loadInstallationData(organizationData(), 0);
    else
        installationTools.loadInstallationData(organizationData(), getIndexOrganization(organizationLocation));

    //installationTools.loadInstallationData();
    //installationTools.loadInstallationData();
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


QJsonObject Backend::loginWindowsUsernameData() const
{
    return m_loginWindowsUsername;
}

void Backend::setLoginWindowsUsernameData(const QJsonObject &data)
{
    m_loginWindowsUsername = data;
    emit loginWindowsUsernameChanged();
}

void Backend::loginWindowsUsername()
{
    //userTools.loginOrganization(username, password);
    userTools.loginWindowsUsername();
}

void Backend::loginWindowsUsernameLoaded(const QJsonObject &loginData)
{
    setLoginWindowsUsernameData(loginData);
}


QJsonObject Backend::loginUsernameData() const
{
    return m_loginUsername;
}

void Backend::setLoginUsernameData(const QJsonObject &data)
{
    m_loginUsername = data;
    emit loginUsernameChanged();
}

void Backend::loginUsername(const QString &username, const QString &password)
{
    installationTools.loginUsername(username, password, organizationData()->get(0)->organizationID());
}

void Backend::loginUsernameLoaded(const QJsonObject &data)
{
    setLoginUsernameData(data);
}
/*
QJsonObject Backend::logoutOrganizationData() const
{
    return m_logoutOrganization;
}

void Backend::setLogoutOrganizationData(const QJsonObject &logoutOrganization)
{
    m_logoutOrganization = logoutOrganization;
    emit logoutOrganizationChanged();
}

void Backend::logoutOrganization(const QString &username, const QString &password)
{
    userTools.logoutOrganization(username, password);
}

void Backend::logoutOrganizationLoaded(const QJsonObject &logoutOrganization)
{
    setLogoutOrganizationData(logoutOrganization);
}
*/

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
    loggingTools.createLogs(formatName, logSource, severity,
                            errorMessage, getValueFromColumnNameJSON("user_type", "id",
                                                                     loginWindowsUsernameData()),
                            getValueFromColumnNameJSON("organization_id",
                                                       loginWindowsUsernameData()),
                            getValueFromColumnNameJSON("id",
                                                       installationData()));
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
    emit appDataChanged();
}

void Backend::loadApp()
{
    appTools.getAllAppInstall();
}

void Backend::appLoaded(AppModel *newAppData)
{
    setAppData(newAppData);
}


AppQuickLaunchModel *Backend::appQuickData() const
{
    return m_appQuickData;
}

void Backend::setAppQuickData(AppQuickLaunchModel *newAppQuickData)
{
    m_appQuickData = newAppQuickData;
    emit appQuickDataChanged();
}

void Backend::loadAppQuick()
{
    appTools.getAllAppQuickLaunch();
}

void Backend::appQuickLoaded(AppQuickLaunchModel *newAppQuickData)
{
    setAppQuickData(newAppQuickData);
}


int Backend::getValueFromColumnNameJSON(const QString &name, const QJsonObject &json)
{
    if(!json[name].isNull())
    {
        return json[name].toInt();
    }
    return -1;
}

int Backend::getValueFromColumnNameJSON(const QString &name, const QString name2,
                                            const QJsonObject &json)
{
    if(!json[name].isNull() && !json[name][name2].isNull())
    {
        return json[name][name2].toInt();
    }
    return -1;
}

void Backend::createISOFile()
{
    installationTools.createISOHelper();
}
