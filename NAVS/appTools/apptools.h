#ifndef APPTOOLS_H
#define APPTOOLS_H

#include <QObject>
#include "webServicesTools/webservicestools.h"
#include "appmodel.h"
#include "appinfo.h"
#include "organizationModel.h"
#include "notificationinfo.h"
#include "notificationsmodel.h"
#include "userTools/usertools.h"
#include <QQuickWebEngineProfile>
#include <socket/sio_socket.h>
#include <socket/sio_client.h>
#include <socket/sio_message.h>
#include <functional>
#include <QJsonObject>
#include <loggingTools/loggingtools.h>

using namespace sio;

class AppTools : public QObject
{
    Q_OBJECT

public:
    explicit AppTools(QObject *parent = nullptr);
    void getAllAppInstall(const int &orgUserID,
                          const int &orgLctID,
                          const QString &sessionKey);

    void loadWelcome(const QString &sessionKey, const QJsonObject user);
    void addAppQuickLaunch(const int &appInstallId,
                           const int &orgUserId,
                           const int &sortOrder,
                           const QString &sessionKey);

    void deleteAppQuickLaunch(const int &appQuickLaunchId,
                              const QString &sessionKey);
    void checkAppQuick(const int &appID);

    //Use for Welcome Screen
    QString createWebPageFile(const QString &filename, const QString &content);
    //int getLocationAppId(const int &locationId, const QString &sessionKey);

    //Setup sessionkey as a header.
    void loadProfile(const QString &sessionKey);

    void getInitialNotifications(std::string const& name, message::ptr const& data,
                                                    bool hasAck, message::list &ack_res);
    void catchNewNotification(std::string const& name, message::ptr const& data,
                              bool hasAck, message::list &ack_resp);
    void catchDeletedNotification(std::string const& name, message::ptr const& data,
                                  bool hasAck, message::list &ack_resp);

    void catchUpdatedNotification(std::string const& name, message::ptr const& data,
                                  bool hasAck, message::list &ack_resp);
    void getConnected();
    void getClosed(client::close_reason const& reason);
    void getFailed();
    QString getAppTitle(const int appId);
    QString getAppURL(const int appId);
    QStringList getAppCategories();
    void closeSocketConnection();
    void initialSocketConnection(int organization_user_id, QString beta1_session_key);
    void updateNotificationDeliveryDate(const QJsonObject communication);
    void deleteNotification(const QJsonObject communication);
    bool appsLoaded();
    void requestSingleAppBadgeUpdate(const QString appUrl, const int appId, const QString sessionKey);
    void runAllBadgeUpdate(const QString sessionKey, const QString organization_user_id);

signals:
    void appLoaded(AppModel *appModel);
    void appQuickLoaded(AppModel *appQuickLaunchModel);
    void welcomeLoaded(const QString &welcomeHTML);
    void addAppQuickLoaded(const QJsonObject &jsonApp);
    void deleteAppQuickLoaded(const QJsonObject &jsonApp);
    void checkAppQuickLoaded(const bool &result);
    void profileLoaded(QQuickWebEngineProfile *profile);
    void newSocketConnection(NotificationsModel *notificationsmodel);
    void sendInitialNotificationsLoaded(NotificationsModel *notificationsmodel);
    void newNotificationCaught(QJsonObject new_notification);
    void notificationDeleted(const QString notificationID);
    void newNotificationInfoItem(NotificationInfo* newItm);
    void updatedNotificationInfoItem(NotificationInfo* updatedItm);
    void badgeCountRetrieved(const int appId, const int badgeTotal);
    void resetBadgeCountAction(const int &appId);
    //Error Message
    void appDataErrorLoaded(const QString &code);
    void quickAppDataErrorLoaded(const QString &code);
    void welcomePageDataErrorLoaded(const QString &code);
    void addQuickAppFailLoaded(const QString &code);
    void deleteQuickAppFaileLoaded(const QString &code);
    //void organizationLocationAppIDErrorLoaded(const QString &code);
    void networkError(QNetworkReply::NetworkError code);

public slots:
        void requestErrorOccurred(QNetworkReply::NetworkError code);

private:
    WebServicesTools webServiceTools;
    UserTools userTools;
    NotificationsModel *notificationsmodel;
    InstallationTools installationTools;
    LoggingTools loggingTools;
    int m_organization_user_id;
    QString m_beta1_session_key;
    std::unique_ptr<client> _io;

    //connection listener declare:
    typedef std::function<void(message::ptr const& message)> error_listener;
    typedef std::function<void(std::string const& nsp)> socket_listener;
    typedef std::function<void(void)> con_listener;
    typedef std::function<void(client::close_reason const& reason)> close_listener;

    QJsonObject m_installedApps;
    bool appsRetrived = false;
    QString getAppIcon(int64_t searchAppID);
    std::string kURL;
    int Codes;
};

#endif // APPTOOLS_H
