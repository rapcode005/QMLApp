#ifndef NOTIFICATIONINFO_H
#define NOTIFICATIONINFO_H

#include <QObject>
#include <QJsonObject>

class NotificationInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int64_t notification_id READ notification_id WRITE setNotification_id NOTIFY notification_idChanged)
    Q_PROPERTY(int64_t app_id READ app_id WRITE setApp_id NOTIFY app_idChanged)
    Q_PROPERTY(int64_t organization_user_id READ organization_user_id WRITE setOrganization_user_id NOTIFY oidChanged)
    Q_PROPERTY(int notification_id_int READ notification_id_int WRITE setNotification_id_int NOTIFY notification_id_int_Changed)
    Q_PROPERTY(int app_id_int READ app_id_int WRITE setApp_id_int NOTIFY app_id_int_Changed)
    Q_PROPERTY(int organization_user_id_int READ organization_user_id_int WRITE setOrganization_user_id_int NOTIFY oid_int_Changed)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QJsonObject data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QString action_name READ action_name WRITE setAction_name NOTIFY action_nameChanged)
    Q_PROPERTY(QString delivered_date READ delivered_date WRITE setDelivered_date NOTIFY delivered_dateChanged)
    Q_PROPERTY(QString app_icon READ app_icon WRITE setApp_icon NOTIFY app_iconChanged)

public:
    NotificationInfo(
        QString id,
        //from int to int64_t
        int64_t notification_id,
        int64_t app_id,
        int64_t organization_user_id,
        QString message,
        QJsonObject data,
        QString action_name,
        QString delivered_date,
        QString app_icon_val
    );

    Q_INVOKABLE QString id() const;
    Q_INVOKABLE void setId(const QString &id);

    Q_INVOKABLE int64_t notification_id() const;
    Q_INVOKABLE void setNotification_id(const int64_t &notification_id);

    Q_INVOKABLE int64_t app_id() const;
    Q_INVOKABLE void setApp_id(const int64_t &app_id);

    Q_INVOKABLE int64_t organization_user_id() const;
    Q_INVOKABLE void setOrganization_user_id(const int64_t &organization_user_id);

    Q_INVOKABLE int notification_id_int() const;
    Q_INVOKABLE void setNotification_id_int(const int &notification_id);

    Q_INVOKABLE int app_id_int() const;
    Q_INVOKABLE void setApp_id_int(const int &app_id);

    Q_INVOKABLE int organization_user_id_int() const;
    Q_INVOKABLE void setOrganization_user_id_int(const int &organization_user_id);

    Q_INVOKABLE QString message() const;
    Q_INVOKABLE void setMessage(const QString &message);

    Q_INVOKABLE QJsonObject data() const;
    Q_INVOKABLE void setData(const QJsonObject &data);

    Q_INVOKABLE QString action_name() const;
    Q_INVOKABLE void setAction_name(const QString &action_name);

    Q_INVOKABLE QString delivered_date() const;
    Q_INVOKABLE void setDelivered_date(const QString &delivered_date);

    Q_INVOKABLE QString app_icon() const;
    Q_INVOKABLE void setApp_icon(const QString &app_icon);

signals:
    void idChanged();

    void notification_idChanged();
    void app_idChanged();
    void oidChanged();
    void notification_id_int_Changed();
    void app_id_int_Changed();
    void oid_int_Changed();
    void messageChanged();
    void dataChanged();
    void action_nameChanged();
    void delivered_dateChanged();
    void app_iconChanged();

private:
    QString m_id;
    int64_t m_notification_id;
    int64_t m_app_id;
    int64_t m_organization_user_id;
    int m_notification_id_int;
    int m_app_id_int;
    int m_organization_user_id_int;
    QString m_message;
    QJsonObject m_data;
    QString m_action_name;
    QString m_delivered_date;
    QString m_app_icon;
};

#endif // NOTIFICATIONINFO_H


/* Data Looks like this
{
    _id: 5 ec05da91e95c15e4edde0b9,
    notification_id: 3,
    app_id: 2,
    organization_user_id: 4,
    message: "This is the message updated 1",
    data: {},
    action_name: "Go forth",
    delivered_date: null,
    updatedAt: 2020 - 05 - 16 T21: 47: 12.843 + 00: 00,
    createdAt: 2020 - 05 - 16 T21: 39: 53.484 + 00: 00,
    __v: 0
}

//Also we want to app icon image url.
*/
