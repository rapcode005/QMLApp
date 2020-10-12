#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QJsonObject>

#include "installationTools/installationtools.h"
#include "appTools/apptools.h"
#include "userTools/usertools.h"
#include "loggingTools/loggingtools.h"
#include "organizationModel.h"
#include "appmodel.h"
#include "appquicklaunchmodel.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject userData
               READ userData
               WRITE setUserData
               NOTIFY userDataChanged)
    Q_PROPERTY(bool organizationDnsKeyStatus
               READ organizationDnsKeyStatus
               WRITE setOrganizationDnsKeyStatus
               NOTIFY organizationDnsKeyStatusChanged)
    Q_PROPERTY(OrganizationModel* organizationData
               READ organizationData
               WRITE setOrganizationData
               NOTIFY organizationDataChanged)
    Q_PROPERTY(QJsonObject installationData
               READ installationData
               WRITE setInstallationData
               NOTIFY installationDataChanged)
    Q_PROPERTY(bool registerStatus
               READ registerStatus
               WRITE setRegisterStatus
               NOTIFY registerStatusChanged)
    Q_PROPERTY(QJsonObject loginWindowsUsernameData
               READ loginWindowsUsernameData
               WRITE setLoginWindowsUsernameData
               NOTIFY loginWindowsUsernameChanged)
    Q_PROPERTY(QJsonObject loginUsernameData
               READ loginUsernameData
               WRITE setLoginUsernameData
               NOTIFY loginUsernameChanged)
    /*
    Q_PROPERTY(QJsonObject logoutOrganizationData
               READ logoutOrganizationData
               WRITE setLogoutOrganizationData
               NOTIFY logoutOrganizationChanged)
    */
    Q_PROPERTY(QJsonObject logData
               READ logData
               WRITE setLogData
               NOTIFY logChanged)
    Q_PROPERTY(AppModel* appData
               READ appData
               WRITE setAppData
               NOTIFY appDataChanged)
    Q_PROPERTY(AppQuickLaunchModel* appQuickData
               READ appQuickData
               WRITE setAppQuickData
               NOTIFY appQuickDataChanged)
public:
    explicit Backend(QObject *parent = nullptr);

    //bool installationData() const;
    //void setInstallationData(const bool &installationData);

    QJsonObject installationData() const;
    void setInstallationData(const QJsonObject &installationData);

    QJsonObject userData() const;
    void setUserData(const QJsonObject &userData);

    OrganizationModel *organizationData() const;
    void setOrganizationData(OrganizationModel *organizationData);

    bool registerStatus() const;
    void setRegisterStatus(const bool &registerStatus);

    bool organizationDnsKeyStatus() const;
    void setOrganizationDnsKeyStatus(bool organizationDnsKeyStatus);

    int getIndexOrganization(const QString &oranizationLocationName);

    QJsonObject loginWindowsUsernameData() const;
    void setLoginWindowsUsernameData(const QJsonObject &data);

    QJsonObject loginUsernameData() const;
    void setLoginUsernameData(const QJsonObject &data);

    //QJsonObject logoutOrganizationData() const;
    //void setLogoutOrganizationData(const QJsonObject &logoutOrganization);

    QJsonObject logData() const;
    void setLogData(const QJsonObject &log);

    AppModel *appData() const;
    void setAppData(AppModel *newAppData);

    AppQuickLaunchModel *appQuickData() const;
    void setAppQuickData(AppQuickLaunchModel *newAppQuickData);

    //int getValueFromColumnNameJSON(const QString &name);
    //int getValueFromColumnNameOrganizationDataModel(const QString &name, OrganizationModel *model);
    int getValueFromColumnNameJSON(const QString &name, const QJsonObject &json);
    int getValueFromColumnNameJSON(const QString &name, const QString name2,
                                       const QJsonObject &json);


private:
    InstallationTools installationTools;
    LoggingTools loggingTools;
    AppTools appTools;
    UserTools userTools;
    WebServicesTools webServicesTools;

    OrganizationModel *m_organizationData;
    AppModel *m_appData;
    AppQuickLaunchModel *m_appQuickData;
    QJsonObject m_userData;
    QJsonObject m_installationData;
    //bool m_installationData;
    QJsonObject m_loginWindowsUsername;
    //QJsonObject m_logoutOrganization;
    QJsonObject m_loginUsername;
    QJsonObject m_log;
    bool m_registerStatus;
    bool m_organizationDnsKeyStatus;


signals:
    void userDataChanged();
    void organizationDnsKeyStatusChanged();
    void organizationDataChanged();
    void installationDataChanged();
    void registerStatusChanged();
    void loginWindowsUsernameChanged();
    //void logoutOrganizationChanged();
    void logChanged();
    void appDataChanged();
    void appQuickDataChanged();
    void loginUsernameChanged();

public slots:
    void loadUser();
    void userLoaded(const QJsonObject &userData);

    void checkOrganizationDnsKeyStatus(const QString &s_organizationDnsKey);
    void organizationDnsKeyStatusLoaded(const bool &organizationDnsKeyStatus);

    void loadOrganization(const QString &s_organization);
    void organizationLoaded(OrganizationModel *organizationData);

    //void loadInstallationData();
    void loadInstallationData(QString organizationLocation = "");
    void installationDataLoaded(const QJsonObject &installationData);
    void deleteInstallationData();

    void checkRegisterStatus();
    void registerStatusLoaded(const bool &regsisterStatus);

    //void loginOrganization(const QString &username, const QString &password);
    void loginWindowsUsername();
    void loginWindowsUsernameLoaded(const QJsonObject &loginData);

    void loginUsername(const QString &username, const QString &password);
    void loginUsernameLoaded(const QJsonObject &data);
    //void logoutOrganization(const QString &username, const QString &password);
    //void logoutOrganizationLoaded(const QJsonObject &logoutOrganization);

    void createLog(const QString &formatName, const QString &logSource,
                   const QString &severity, const QString &errorMessage);
    void logLoaded(const QJsonObject &logData);

    void loadApp();
    void appLoaded(AppModel *newAppData);

    void loadAppQuick();
    void appQuickLoaded(AppQuickLaunchModel *newAppQuickData);

    void createISOFile();
};

#endif // BACKEND_H
