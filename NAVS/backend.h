#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QJsonObject>

#include "installationTools/installationtools.h"
#include "appTools/apptools.h"
#include "appTools/charlietester.h"
#include "userTools/usertools.h"
#include "loggingTools/loggingtools.h"
#include "organizationModel.h"
#include "appmodel.h"
#include "notificationsmodel.h"
#include <QQuickWebEngineProfile>
#include "urlrequest.h"
#include "navsObject.h"
#include <socket/sio_socket.h>
#include <socket/sio_client.h>
#include <socket/sio_message.h>
#include "errormessage.h"
#include "appfilterproxy.h"

using namespace sio;

class Backend : public QObject
{
    Q_OBJECT
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
    Q_PROPERTY(QJsonObject loginData
               READ loginData
               WRITE setLoginData
               NOTIFY loginDataChanged)
    Q_PROPERTY(QJsonObject logData
               READ logData
               WRITE setLogData
               NOTIFY logChanged)
    Q_PROPERTY(AppModel* appData
               READ appData
               WRITE setAppData
               NOTIFY appDataChanged)
    Q_PROPERTY(AppModel* appQuickData
               READ appQuickData
               WRITE setAppQuickData
               NOTIFY appQuickDataChanged)
    Q_PROPERTY(QString welcomeData
               READ welcomeData
               WRITE setWelcomeData
               NOTIFY welcomeChanged)
    Q_PROPERTY(QJsonObject addAppQuickLaunchData
               READ addAppQuickLaunchData
               WRITE setAddAppQuickLaunchData
               NOTIFY addAppQuickChanged)
    Q_PROPERTY(QJsonObject deleteAppQuickLaunchData
               READ deleteAppQuickLaunchData
               WRITE setDeleteAppQuickLaunchData
               NOTIFY deleteAppQuickChanged)
    Q_PROPERTY(bool checkAppQuickData
               READ checkAppQuickData
               WRITE setCheckAppQuickData
               NOTIFY checkAppQuickChanged)
    Q_PROPERTY(QString betaUrl
               READ betaUrl)
    Q_PROPERTY(QString getSessionKey
               READ getSessionKey)
    Q_PROPERTY(QQuickWebEngineProfile* profileData
               READ profileData
               WRITE setProfileData
               NOTIFY profileChanged)

    Q_PROPERTY(NotificationsModel* notificationsData
               READ notificationsData
               WRITE setNotificationsData
               NOTIFY notificationsDataChanged)

    Q_PROPERTY(QJsonObject meData
               READ meData
               WRITE setMeData
               NOTIFY meDataChanged)

    Q_PROPERTY(NavsObject* navsObject
               READ navsObject)

    //Http Status Code
    Q_PROPERTY(ErrorMessage* errorMessage
               READ errorMessage
               NOTIFY errorMessageChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    QJsonObject installationData() const;
    void setInstallationData(const QJsonObject &installationData);

    OrganizationModel *organizationData() const;
    void setOrganizationData(OrganizationModel *organizationData);

    bool registerStatus() const;
    void setRegisterStatus(const bool &registerStatus);

    bool organizationDnsKeyStatus() const;
    void setOrganizationDnsKeyStatus(bool organizationDnsKeyStatus);

    int getIndexOrganization(const QString &oranizationLocationName);

    QJsonObject loginData() const;
    void setLoginData(const QJsonObject &data);

    bool checkAppQuickData() const;
    void setCheckAppQuickData(const bool &result);

    QJsonObject logData() const;
    void setLogData(const QJsonObject &log);

    AppModel *appData() const;
    void setAppData(AppModel *newAppData);

    AppModel *appQuickData() const;
    void setAppQuickData(AppModel *newAppQuickData);

    QString welcomeData() const;
    void setWelcomeData(QString welcome);

    QJsonObject addAppQuickLaunchData() const;
    void setAddAppQuickLaunchData(const QJsonObject &newAppQuickLaunch);

    QJsonObject deleteAppQuickLaunchData() const;
    void setDeleteAppQuickLaunchData(const QJsonObject &appQuickLaunch);

    QQuickWebEngineProfile *profileData() const;
    void setProfileData(QQuickWebEngineProfile *newProfile);

    NotificationsModel *notificationsData() const;
    void setNotificationsData(NotificationsModel *notModel);

    QJsonObject meData() const;
    void setMeData(const QJsonObject &meData);

    QString betaUrl();
    QString getSessionKey();

    NavsObject *navsObject() const;

    int getValueFromColumnNameJSON(const QString &name, const QJsonObject &json);
    int getValueFromColumnNameJSON(const QString &name, const QString name2,
                                       const QJsonObject &json);
    void bootstrapSettingsFile();

    ErrorMessage *errorMessage() const;

    //AppFilterProxy filterModel;

private:
    InstallationTools installationTools;
    LoggingTools loggingTools;
    AppTools appTools;
    UserTools userTools;
    WebServicesTools webServicesTools;
    CharlieTester charlieTester;
    OrganizationModel *m_organizationData;
    AppModel *m_appData;
    AppModel *m_appQuickData;
    QQuickWebEngineProfile *m_profileData;
    QJsonObject m_userData;
    QJsonObject m_installationData;
    QJsonObject m_loginData;
    QJsonObject m_log;
    QJsonObject m_addAppQuickLaunch;
    QJsonObject m_deleteAppQuickLaunch;
    QString m_betaUrl;
    QString m_welcome;
    ErrorMessage *m_errorMessage = new ErrorMessage();
    NotificationsModel *m_notificationsData;
    QJsonObject m_meData;
    bool orgLoadComplete = false;
    void catchNewNotification(const QJsonObject new_notification);
    void catchNewNotificationInfoItem(NotificationInfo* newItm);
    void catchUpdatedNotificationInfoItem(NotificationInfo* updatedItm);
    bool m_registerStatus = false;
    bool m_organizationDnsKeyStatus = false;
    bool m_checkAppQuick = false;
    NavsObject *m_navs;
    QString m_jsCode;
    void foundAvailableVersion(QString versionNumber);
    void requestAppBagdeCount(const int appId);
    void badgeCountReceived(const int appId, const int badgeTotal);
    void loginWindowsUsernameLoaded(const QJsonObject &loginData);
    void loginUsernameLoaded(const QJsonObject &data);
    void createSettingsFile();
    struct NAVSEnv {
        QString name;
    };
    QList<NAVSEnv> navsenvdata;
    void writeLogItem(const QString logItem);
    void catchNotificationDeleted(const QString notificationID);
    QList<AppFilterProxy*> appFilterModels;
    void createAppFilterModel(const QString categoryName);
    int getAppIndexFromAppInstallId(const int appInstallId);

signals:
    void organizationDnsKeyStatusChanged();
    void organizationDataChanged();
    void installationDataChanged();
    void registerStatusChanged();
    void logChanged();
    void appDataChanged();
    void appQuickDataChanged();
    void loginDataChanged();
    void welcomeChanged();
    void addAppQuickChanged();
    void deleteAppQuickChanged();
    void checkAppQuickChanged();
    void titleChanged();
    void betaUrlChanged();
    void profileChanged();
    void notificationsDataChanged();
    void meDataChanged();
    void newNotificationConveyer(const QJsonObject new_notification);
    void versionInfoFound(const QString versionInfo);
    void updateCompleted();
    void writeQMLLogItem(const QString logItem);
    void totalBadgeCountChanged(const int totalBadgeCount);
    //Error API
    void errorMessageChanged(const int errorId);

public slots:

    void checkOrganizationDnsKeyStatus(const QString &s_organizationDnsKey);
    void organizationDnsKeyStatusLoaded(const bool &organizationDnsKeyStatus);

    void loadOrganization(const QString sessionKey);
    void organizationLoaded(OrganizationModel *organizationData);

    void sendInstallationDataToBeta1(QString organizationLocation = "", int idx = 0);
    void installationDataLoaded(const QJsonObject &installationData);
    void deleteInstallationData();

    void checkRegisterStatus();
    void registerStatusLoaded(const bool &regsisterStatus);

    void loginWindowsUsername();
    void loginUsername(const QString &username, const QString &password);
    void loginUsernameSecondary(const QString &username, const QString &password);

    void createLog(const QString &formatName, const QString &logSource,
                   const QString &severity, const QString &errorMessage);
    void logLoaded(const QJsonObject &logData);

    void loadApps();
    void appLoaded(AppModel *newAppData);

    void appQuickLoaded(AppModel *newAppQuickData);

    void loadWelcome();
    void welcomeLoaded(const QString &welcome);

    void addAppQuick(const int &id);
    void addAppQuickLoaded(const QJsonObject &json);

    void deleteAppQuick(const int &appId);
    void deleteAppQuickLoaded(const QJsonObject &json);

    void checkAppQuick(const int &appId);
    void checkAppQuickLoaded(const bool &result);

    void loadProfile();
    void profileLoaded(QQuickWebEngineProfile *newProfile);

    //HTTP Status Code Error
    void appDataErrorLoaded(const QString &code);
    void quickAppDataErrorLoaded(const QString &code);
    void welcomePageDataErrorLoaded(const QString &code);
    void addQuickAppFailLoaded(const QString &code);
    void deleteQuickAppFaileLoaded(const QString &code);
    void organizationDNSKeyErrorLoaded(const QString &code);
    void organizationByIDErrorLoaded(const QString &code);
    void loginWindowsUsernameErrorLoaded(const QString &code);
    void loadMeDataErrorLoaded(const QString &code);
    void loginUsernameErrorLoaded(const QString &code);
    void networkError(QNetworkReply::NetworkError code);

    void initialNotificationsLoaded(NotificationsModel *notificationsModel);

    void startInitialSocketConnection();

    void loadMeData();

    QString getOrgName();

    QString getOrgIcon();

    QString getAppTitle(const int appId);

    QString getAppURL(const int appId);

    bool isCUAdmin();

    int getProfileTypeId();

    //Charlie functions
    void checkCharlieProcess();

    //JavaScript Functions
    QString getJsCode(const QString jsFileName) const;

    void updateNotificationDeliveryDate(QJsonObject communication);

    void deleteNotification(const int notification_id, const int notificationIndex);

    void launchUpdate();

    void getAvailableVersionNumber();

    void updateAllAppBadges();

    bool isTestEnv();

    void startUpdateClock();

    void installCharlie2();

    QStringList getAppCategories();

    AppFilterProxy* getAppFilterProxyByIndex(const int idx);

    void deleteAllCategoryProxyModels();
};

#endif // BACKEND_H
