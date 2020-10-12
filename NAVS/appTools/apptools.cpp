
#include "apptools.h"
#include <QUrl>
#include "urlrequest.h"
#include <mutex>
#include <cstdlib>

//#define kURL "http://notifications.navsnow.com:80"

#ifdef WIN32
#define BIND_EVENT(IO,EV,FN) \
    do{ \
        socket::event_listener_aux l = FN;\
        IO->on(EV,l);\
    } while(0)

#else
#define BIND_EVENT(IO,EV,FN) \
    IO->on(EV,FN)
#endif

AppTools::AppTools(QObject *parent) :
    QObject(parent)
    ,_io(new client())
{
    QSettings settings;
    settings.beginGroup("Servers");
    QVariant value = settings.value("notifications_url", "https://notificationsprod.navsnow.com:443");

    //qDebug() << value;

    settings.endGroup();
    //Convert from QString to std string
    kURL = value.toString().toUtf8().constData();

    connect(&webServiceTools, &WebServicesTools::networkError, this,
            &AppTools::requestErrorOccurred);
}

void AppTools::getAllAppInstall(const int &orgUserID, const int &orgLctID,
                                const QString &sessionKey)
{
    const char errorApp[40] = "Apps could not be retrieved. API error.";
    const char errorAppQuick[46] = "Quick apps could not be retrieved. API error.";

    try {

        QList<int> AlreadyAppList;
        QList<int> AlreadyAppQuickList;

        const QString &url = webServiceTools.getApiUrlNavs() + "api/users-apps?organization_user_id=" + QString::number(orgUserID) +
                "&organization_location_id=" + QString::number(orgLctID);

        webServiceTools.computeJsonValueGet(url, sessionKey);

        const QJsonObject &jsonResult = webServiceTools.getJsonValue();        

        m_installedApps = jsonResult;
        appsRetrived = true;

        //Get Status Code
        //connect(&webServiceTools, &WebServicesTools::statusCode, this, [=](const int &code) {

            //emit appDataCodeLoaded(code);

        //});


        if(jsonResult.size() > 0) {

            const QJsonArray &jsonArray = jsonResult["data"].toArray(), arrayAppQuick;

            AppModel* appModel = new AppModel();
            AppModel* appQuickLaunchModel = new AppModel();

            foreach (const QJsonValue &value, jsonArray) {
                    const int &id = value.toObject().find("app")->toObject().find("id")->toInt();

                    if(AlreadyAppList.indexOf(id) != -1){
                        continue;
                    }
                    AlreadyAppList.append(id);

                    //qDebug() << value.toObject();

                    //qDebug() << "------------";

                    const QString    &categoryAppName	= value.toObject().find("organization_app_category")->toObject().find("app_category")->toObject().find("name")->toString();
                    const int        &orgLctAppId = value.toObject().find("id")->toInt();
                    const QString    &appName = value.toObject().find("app")->toObject().find("display_name")->toString();
                    const QString    &icon = value.toObject().find("app")->toObject().find("icon")->toString();
                    const int        &sortOrderAppCategory = value.toObject().find("organization_app_category")->toObject().find("sort_order")->toInt();
                    const QString    &urlApp = value.toObject().find("app")->toObject().find("start_url")->toString();
                    const int        &appInstalId = value.toObject().find("app_installations")->toArray()[0].toObject().find("id")->toInt();
                    const QJsonArray &arrayAppQuick = value.toObject().find("app_installations")->toArray()[0].toObject().find("app_quick_launch")->toArray();
                    const int &appQuickId = (arrayAppQuick.count() < 1) ? 0: arrayAppQuick[0].toObject().find("id")->toInt();

                    const QString &newAppName = appName.length() > 12 ? webServiceTools.adjustAppName(appName) : appName;
                    const int &appNameLength = appName.length(); // Real length without the adjustment

                    AppInfo* ifo = new AppInfo(
                                newAppName,
                                id,
                                orgLctAppId,
                                icon,
                                urlApp,
                                categoryAppName,
                                sortOrderAppCategory,
                                appInstalId,
                                appQuickId,
                                0,
                                0,
                                appNameLength
                    );

                    if(arrayAppQuick.count() > 0){
                        appQuickLaunchModel->append(ifo);
                    }else{
                        appModel->append(ifo);
                    }

                    //ifo->destroyed(this);
            }

            emit appLoaded(appModel);
            emit appQuickLoaded(appQuickLaunchModel);

            //appModel->destroyed(this);
            //appQuickLaunchModel->destroyed(this);

            //Sorry Jon I don't know the format Name, log source or etc.. value;
            //loggingTools->createLogs();

        }
        else {
            emit appDataErrorLoaded(errorApp);
            emit quickAppDataErrorLoaded(errorAppQuick);
        }

    } catch (...) {
        emit appDataErrorLoaded(errorApp);
        emit quickAppDataErrorLoaded(errorAppQuick);
    }
}

void AppTools::loadWelcome(const QString &sessionKey, const QJsonObject user)
{
    const char errorWelcome[34] = "Welcome was not loaded. API error";

    try {
        QString html = "";

        const QString &userTypeId = QString::number(userTools.getLoginTypeID(user));
        const QString &url = webServiceTools.getApiUrlNavs() +
                "api/welcome-screens?user_type_id=" + userTypeId;

        webServiceTools.computeJsonValueGet(url, sessionKey);

        const QJsonObject &jsonResult = webServiceTools.getJsonValue();

        if (jsonResult.size() <= 0) {
            emit welcomePageDataErrorLoaded(errorWelcome);
        }
        else {
            const QJsonArray &jsonArray = jsonResult["data"].toArray();

            foreach(const QJsonValue &value, jsonArray) {
                html = value.toObject().find("content")->toString();
            }

            const QString &urlResult = createWebPageFile("welcome.html", html);

            emit welcomeLoaded(urlResult);

            //Sorry Jon I don't know the format Name, log source or etc.. value;
            //loggingTools->createLogs();
        }
    } catch (...) {
        emit welcomePageDataErrorLoaded(errorWelcome);
    }
}

void AppTools::addAppQuickLaunch(const int &appInstallId, const int &orgUserId,
                                 const int &sortOrder, const QString &sessionKey)
{
    const char errorAdd[33] = "The operation failed. Api Error.";

    try {

        const QString &url = webServiceTools.getApiUrlNavs() + "api/app-quick-launches?"
                "app_installation_id=" + QString::number(appInstallId) +
                "&organization_user_id=" + QString::number(orgUserId) +
                "&sort_order=" + QString::number(sortOrder);

        webServiceTools.computeJsonValuePost(url, sessionKey);

        const QJsonObject &jsonResult = webServiceTools.getJsonValue();

        if(jsonResult.length() > 0){
            emit addAppQuickLoaded(jsonResult);
            //Sorry Jon I don't know the format Name, log source or etc.. value;
            //loggingTools->createLogs();
        }else{
            emit addQuickAppFailLoaded(errorAdd);
        }

    } catch (...){
        emit addQuickAppFailLoaded(errorAdd);
    }
}

void AppTools::deleteAppQuickLaunch(const int &appQuickLaunchId, const QString &sessionKey)
{
    try {

        const QString &url = webServiceTools.getApiUrlNavs() +
                "api/app-quick-launches/" + QString::number(appQuickLaunchId);

        webServiceTools.deleteJsonValue(url, sessionKey);
        const QJsonObject &jsonResult = webServiceTools.getJsonValue();

        if (jsonResult.size() > 0) {
             emit deleteAppQuickLoaded(jsonResult);
            //Sorry Jon I don't know the format Name, log source or etc.. value;
            //loggingTools->createLogs();

        }
        else {
            emit deleteQuickAppFaileLoaded("The operation failed. Api Error.");
        }

    } catch (...) {
        emit deleteQuickAppFaileLoaded("The operation failed. Api Error.");
    }
}

QString AppTools::createWebPageFile(const QString &filename, const QString &content)
{
    QString urlResult;
    QFile file(filename);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << content;

        urlResult = QDir::currentPath() + "/" + filename;

        //Flush all stream text
        file.flush();
        file.close();
    }
    return urlResult;
}
/*
int AppTools::getLocationAppId(const int &locationId, const QString &sessionKey)  //Is this function still in use? I can't find any reference to it---------------------------------------If not, remove it.
{
    try {

        const QString &url = webServiceTools.getApiUrlNavs() + "api/organization-location-apps?"
                                                              "organization_location_id=" + QString::number(locationId);

        webServiceTools.computeJsonValueGet(url, sessionKey);

        const QJsonObject &jsonResult = webServiceTools.getJsonValue();

        if(jsonResult.size() > 0) {
            return jsonResult["id"].toInt();
        }else{
            //We need to emit an error that the front end can pick up.--------------------------------------
            emit organizationLocationAppIDErrorLoaded("Unable to load Organization Location App ID.");
            return -1;
        }

    } catch (...) {
        //We need to emit an error that the front end can pick up.-------------------------------------------
        emit organizationLocationAppIDErrorLoaded("Unable to load Organization Location App ID.");
        return -1;
    }
}*/

void AppTools::loadProfile(const QString &sessionKey)
{
    QQuickWebEngineProfile *profile = new QQuickWebEngineProfile();
    try {
        UrlRequest *urlRequest = new UrlRequest();
        urlRequest->sessionKey = sessionKey;
        profile->setUrlRequestInterceptor(urlRequest);
        emit profileLoaded(profile);
    } catch (...) {
        emit profileLoaded(profile);
    }

    //profile->destroyed(this);
}

void AppTools::checkAppQuick(const int &appID)
{
    try {

        bool result = false;
        const QString &url = webServiceTools.getApiUrlNavs() + QString::number(appID);

        webServiceTools.computeJsonValueGet(url);

        const QJsonObject &jsonResult = webServiceTools.getJsonValue();

        if(jsonResult.size() > 0)
            result = true;

        emit checkAppQuickLoaded(result);

    } catch (...) {
        emit checkAppQuickLoaded(false);
    }
}

void AppTools::closeSocketConnection(){
    _io->clear_con_listeners();
    _io->close();
}

void AppTools::initialSocketConnection(int organization_user_id, QString beta1_session_key){
        qDebug() << "Attempting Socket connection";

        m_organization_user_id = organization_user_id;
        m_beta1_session_key = beta1_session_key;
        notificationsmodel = new NotificationsModel();

        _io->connect(kURL);
        socket::ptr sock = _io->socket();

        _io->set_socket_open_listener(socket_listener([this](std::string const& nsp){
            Q_UNUSED(nsp)
            getConnected();
        }));

        _io->set_close_listener(close_listener([this](client::close_reason const& reason){
            getClosed(reason);
        }));

        _io->set_fail_listener(con_listener([this]() {
            getFailed();
        }));

        BIND_EVENT(sock, "initial_notifications",
           sio::socket::event_listener_aux([this](std::string const& name,
           sio::message::ptr const& data, bool hasAck,
           sio::message::list &ack_resp)
            {
                Q_UNUSED(this)
                getInitialNotifications(name, data, hasAck, ack_resp);
            }
        ));

        BIND_EVENT(sock, "new_notification",
           sio::socket::event_listener_aux([this](std::string const& name,
           sio::message::ptr const& data, bool hasAck,
           sio::message::list &ack_resp)
            {
                Q_UNUSED(this)
                catchNewNotification(name, data, hasAck, ack_resp);
            }
        ));

        BIND_EVENT(sock, "deleted_notification",
           sio::socket::event_listener_aux([this](std::string const& name,
           sio::message::ptr const& data, bool hasAck,
           sio::message::list &ack_resp)
            {
                Q_UNUSED(this)
                catchDeletedNotification(name, data, hasAck, ack_resp);
            }
        ));

        BIND_EVENT(sock, "updated_notification",
           sio::socket::event_listener_aux([this](std::string const& name,
           sio::message::ptr const& data, bool hasAck,
           sio::message::list &ack_resp)
            {
                Q_UNUSED(this)
                catchUpdatedNotification(name, data, hasAck, ack_resp);
            }
        ));
}

void AppTools::updateNotificationDeliveryDate(const QJsonObject communication)
{
        qDebug() << "Updating notification";
        QJsonDocument doc(communication);
        QString strJson(doc.toJson(QJsonDocument::Compact));
        QByteArray bytes = strJson.toUtf8();
        std::string msg(bytes.data(), static_cast<std::size_t>(bytes.length()));
        _io->socket()->emitSocket("notification_read", msg);
}

void AppTools::deleteNotification(const QJsonObject communication)
{
    qDebug() << "Deleting notification";
    QJsonDocument doc(communication);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    QByteArray bytes = strJson.toUtf8();
    std::string msg(bytes.data(), static_cast<std::size_t>(bytes.length()));
    _io->socket()->emitSocket("delete_notification", msg);
    qDebug() << communication;
}

bool AppTools::appsLoaded()
{
    if(!m_installedApps.isEmpty()){
        return true;
    }
    return false;
}

void AppTools::requestSingleAppBadgeUpdate(const QString appUrl, const int appId, const QString sessionKey)
{
    const QString &url = appUrl;

    webServiceTools.computeJsonValueGet(url, sessionKey);
    const QJsonObject &jsonResult = webServiceTools.getJsonValue();

    if(jsonResult.isEmpty()){
        //This app does not have a badge count-----
        //qDebug() << "empty";
    }else{
        //qDebug() << "full";
        emit badgeCountRetrieved(appId, jsonResult["count"].toInt());
        //qDebug() << jsonResult["count"].toInt();
    }
}

void AppTools::runAllBadgeUpdate(const QString sessionKey, const QString organization_user_id)
{
    const QJsonArray &jsonArray = m_installedApps["data"].toArray();
    foreach (const QJsonValue &value, jsonArray) {
        const QString    &url = value.toObject().find("app")->toObject().find("start_url")->toString() +
                "/api/notifications-count?organization_user_id=" + organization_user_id;
        const int        &appId = value.toObject().find("app_id")->toInt();

        requestSingleAppBadgeUpdate(url, appId, sessionKey);
    }
}

void AppTools::requestErrorOccurred(QNetworkReply::NetworkError code)
{
    //qDebug() << "App Tools Caught Network error. Sending on.";

    emit networkError(code);
}

QString AppTools::getAppIcon(int64_t searchAppID)
{
    if(appsRetrived){
        const QJsonArray &jsonArray = m_installedApps["data"].toArray();
        foreach (const QJsonValue &value, jsonArray) {
            const QString    &icon = value.toObject().find("app")->toObject().find("icon")->toString();
            const int        &appId = value.toObject().find("app_id")->toInt();
            if(searchAppID == appId){
                return icon;
            }
        }
    }
    return "";
}

void AppTools::getInitialNotifications(const std::string &name, const message::ptr &data, bool hasAck, message::list &ack_resp)
{
    Q_UNUSED(ack_resp)
    Q_UNUSED(name)
    Q_UNUSED(hasAck)

    qDebug() << "Initial Notifications Arrived";

    if(data->get_flag() == message::flag_array) {
        std::vector<message::ptr> notificationsVector = data->get_vector();
        for(unsigned long i =0; i < notificationsVector.size(); i++){
            try {
                message::ptr notificationMessage = notificationsVector[i];

                std::map<std::string,message::ptr> notificationMap = notificationMessage->get_map();

                QString notification_mongo_id = QString::fromUtf8(notificationMap["_id"]->get_string().c_str());
                QString notification_message = QString::fromUtf8(notificationMap["message"]->get_string().c_str());
                QString notification_action_name = QString::fromUtf8(notificationMap["action_name"]->get_string().c_str());
                int64_t notification_id = notificationMap["notification_id"]->get_int();
                int64_t notification_app_id = notificationMap["app_id"]->get_int();
                int64_t notification_organization_user_id = notificationMap["organization_user_id"]->get_int();
                QString notification_delivered_date = "";
                try {
                     notification_delivered_date = QString::fromUtf8(notificationMap["delivered_date"]->get_string().c_str());
                } catch (...) {
                    //Go with the default.
                    qDebug("Error in date");
                }

                std::map<std::string,message::ptr> dataMap = notificationMap["data"]->get_map();
                QString targetUrl = QString::fromUtf8(dataMap["target_url"]->get_string().c_str());
                QString message_body = QString::fromUtf8(dataMap["message_body"]->get_string().c_str());
                QJsonObject notification_data
                {
                    {"target_url", targetUrl},
                    {"message_body", message_body}
                };
                QString appIcon = getAppIcon(notification_app_id);

                notificationsmodel->append(new NotificationInfo(
                                                notification_mongo_id,
                                                notification_id,
                                                notification_app_id,
                                                notification_organization_user_id,
                                                notification_message,
                                                notification_data,
                                                notification_action_name,
                                                notification_delivered_date,
                                                appIcon
                                            ));



                if(notification_delivered_date.length() < 1){
                    QJsonObject new_notification
                    {
                        {"target_url", targetUrl},
                        {"message_body", message_body},
                        {"appId" , notificationsmodel->get(notificationsmodel->count()-1)->app_id_int()},
                        {"notificationId" , notificationsmodel->get(notificationsmodel->count()-1)->notification_id_int()},
                        {"appIcon" , appIcon},
                        {"messageTitle" , notification_message},
                        {"messageBody" , message_body},
                        {"callToAction" , notification_action_name},
                        {"actionUrl" , targetUrl},
                        {"notificationType" , 1},
                        {"id" , notification_id},
                        {"organization_user_id" , notificationsmodel->get(notificationsmodel->count()-1)->organization_user_id_int()}
                    };

                    emit newNotificationCaught(new_notification);
                }

            } catch (...) {
                //Ignore the notification
                qDebug("Error in insert");
            }
        }
        emit sendInitialNotificationsLoaded(notificationsmodel);
    }
}

void AppTools::catchDeletedNotification(const std::string &name, const message::ptr &data, bool hasAck, message::list &ack_resp){
    Q_UNUSED(ack_resp)
    Q_UNUSED(name)
    Q_UNUSED(hasAck)
    qDebug() << "Notification deleted";

    if(data->get_flag() == message::flag_string){
        try {
            QString notificationId = QString::fromUtf8(data->get_string().c_str());
            emit notificationDeleted(notificationId);
        } catch (...) {
            qDebug() << "Error in delete notification";
        }
    }
}

void AppTools::catchUpdatedNotification(const std::string &name, const message::ptr &data, bool hasAck, message::list &ack_resp)
{
    Q_UNUSED(ack_resp)
    Q_UNUSED(name)
    Q_UNUSED(hasAck)
    qDebug() << "Updated Notification arrived";

    if(data->get_flag() == message::flag_object) {
        try {
            std::map<std::string,message::ptr> notificationMap = data->get_map();
            QString notification_mongo_id = QString::fromUtf8(notificationMap["_id"]->get_string().c_str());
            QString notification_message = QString::fromUtf8(notificationMap["message"]->get_string().c_str());
            QString notification_action_name = QString::fromUtf8(notificationMap["action_name"]->get_string().c_str());
            int64_t notification_id = notificationMap["notification_id"]->get_int();
            int64_t notification_app_id = notificationMap["app_id"]->get_int();
            int64_t notification_organization_user_id = notificationMap["organization_user_id"]->get_int();
            QString notification_delivered_date = "";
            try {
                 notification_delivered_date = QString::fromUtf8(notificationMap["delivered_date"]->get_string().c_str());
            } catch (...) {
                //Go with the default.
                qDebug("Error in updated notification date");
            }

            std::map<std::string,message::ptr> dataMap = notificationMap["data"]->get_map();
            QString targetUrl = QString::fromUtf8(dataMap["target_url"]->get_string().c_str());
            QString message_body = QString::fromUtf8(dataMap["message_body"]->get_string().c_str());
            QJsonObject notification_data
            {
                {"target_url", targetUrl},
                {"message_body", message_body}
            };
            QString appIcon = getAppIcon(notification_app_id);

            NotificationInfo* newItm = new NotificationInfo(
                        notification_mongo_id,
                        notification_id,
                        notification_app_id,
                        notification_organization_user_id,
                        notification_message,
                        notification_data,
                        notification_action_name,
                        notification_delivered_date,
                        appIcon
                    );

            emit updatedNotificationInfoItem(newItm);
            //newItm->destroyed(this);

        } catch (...) {
            //Ignore the notification
            qDebug("Error in updated notification outer");
        }

    }
}

void AppTools::catchNewNotification(const std::string &name, const message::ptr &data, bool hasAck, message::list &ack_resp)
{
        Q_UNUSED(ack_resp)
        Q_UNUSED(name)
        Q_UNUSED(hasAck)
        qDebug() << "New Notification arrived";

        if(data->get_flag() == message::flag_object) {
            try {
                std::map<std::string,message::ptr> notificationMap = data->get_map();
                QString notification_mongo_id = QString::fromUtf8(notificationMap["_id"]->get_string().c_str());
                QString notification_message = QString::fromUtf8(notificationMap["message"]->get_string().c_str());
                QString notification_action_name = QString::fromUtf8(notificationMap["action_name"]->get_string().c_str());
                int64_t notification_id = notificationMap["notification_id"]->get_int();
                int64_t notification_app_id = notificationMap["app_id"]->get_int();
                int64_t notification_organization_user_id = notificationMap["organization_user_id"]->get_int();
                QString notification_delivered_date = "";
                try {
                     notification_delivered_date = QString::fromUtf8(notificationMap["delivered_date"]->get_string().c_str());
                } catch (...) {
                    //Go with the default.
                    qDebug("Error in insert notification date");
                }

                std::map<std::string,message::ptr> dataMap = notificationMap["data"]->get_map();
                QString targetUrl = QString::fromUtf8(dataMap["target_url"]->get_string().c_str());
                QString message_body = QString::fromUtf8(dataMap["message_body"]->get_string().c_str());
                QJsonObject notification_data
                {
                    {"target_url", targetUrl},
                    {"message_body", message_body}
                };
                QString appIcon = getAppIcon(notification_app_id);

                NotificationInfo* newItm = new NotificationInfo(
                            notification_mongo_id,
                            notification_id,
                            notification_app_id,
                            notification_organization_user_id,
                            notification_message,
                            notification_data,
                            notification_action_name,
                            notification_delivered_date,
                            appIcon
                        );

                emit newNotificationInfoItem(newItm);
                //newItm->destroyed(this);

                if(notification_delivered_date.length() < 1){
                    QJsonObject new_notification
                    {
                        {"target_url", targetUrl},
                        {"message_body", message_body},
                        {"appId" , notification_app_id},
                        {"notificationId" , notification_mongo_id},
                        {"appIcon" , appIcon},
                        {"messageTitle" , notification_message},
                        {"messageBody" , message_body},
                        {"callToAction" , notification_action_name},
                        {"actionUrl" , targetUrl},
                        {"notificationType" , 1},
                        {"id" , notification_id},
                        {"organization_user_id" , notification_organization_user_id}
                    };

                    emit newNotificationCaught(new_notification);

                    QVariant appIdInt = QVariant::fromValue(notification_app_id);
                    emit resetBadgeCountAction(appIdInt.toInt());
                }

            } catch (...) {
                //Ignore the notification
                qDebug("Error in insert notification outer");
            }

        }
}

void AppTools::getConnected()
{
    QString jsonVal = "{\"organization_user_id\":"
            + QString::number(m_organization_user_id)
            +", \"beta1_session_key\":\""
            + m_beta1_session_key +"\"}";
    std::string preppedVals =  jsonVal.toStdString();
    _io->socket()->emitSocket("authentication", preppedVals);
    emit newSocketConnection(notificationsmodel);
    qDebug() << "get connected called";
}


void AppTools::getClosed(const client::close_reason &reason)
{
    Q_UNUSED(reason)

    qDebug() << "get closed called";
}

void AppTools::getFailed()
{
    qDebug() << "get failed called";
}

QString AppTools::getAppTitle(const int appId)
{
    try {
        const QJsonArray &jsonArray = m_installedApps["data"].toArray();
        foreach (const QJsonValue &value, jsonArray) {
            const int &id = value.toObject().find("app")->toObject().find("id")->toInt();
            if(appId == id){

                //qDebug() << "found title for AppID = " <<  value.toObject().find("app")->toObject().find("id")->toInt();

                return value.toObject().find("app")->toObject().find("display_name")->toString();
            }
        }
        return "";
    } catch (...) {
        return "";
    }
}

QString AppTools::getAppURL(const int appId)
{
    //qDebug() << "Looking for url for appId = " << appId;
    try {

        const QJsonArray &jsonArray = m_installedApps["data"].toArray();
        foreach (const QJsonValue &value, jsonArray) {
            const int &id = value.toObject().find("app")->toObject().find("id")->toInt();
            if(appId == id){

                //qDebug() << "found url for AppID = " <<  value.toObject().find("app")->toObject().find("id")->toInt();

                return value.toObject().find("app")->toObject().find("start_url")->toString();
            }
        }
        return "";

    } catch (...) {
        return "";
    }
}

QStringList AppTools::getAppCategories()
{
    QStringList categories = {};
    try {
        const QJsonArray &jsonArray = m_installedApps["data"].toArray();
        foreach (const QJsonValue &value, jsonArray) {
            const QString &categoryAppName = value.toObject().find("organization_app_category")->toObject().find("app_category")->toObject().find("name")->toString();
            if(!categories.contains(categoryAppName)){
                categories << categoryAppName;
            }
        }
        return categories;
    } catch (...) {
        return categories;
    }
}
