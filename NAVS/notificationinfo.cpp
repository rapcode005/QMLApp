#include "notificationinfo.h"
#include <QJsonObject>
#include <QVariant>
#include <QDebug>

NotificationInfo::NotificationInfo(QString id,
                                   int64_t notification_id,
                                   int64_t app_id,
                                   int64_t organization_user_id,
                                   QString message,
                                   QJsonObject data,
                                   QString action_name,
                                   QString delivered_date,
                                   QString app_icon_val)
{
      setId(id);
      setNotification_id(notification_id);
      setApp_id(app_id);
      setOrganization_user_id(organization_user_id);
      setMessage(message);
      setData(data);
      setAction_name(action_name);
      setDelivered_date(delivered_date);
      setApp_icon(app_icon_val);
}

QString NotificationInfo::id() const {
    return m_id;
}
void NotificationInfo::setId(const QString &id){
    m_id = id;
    emit idChanged();
}

int64_t NotificationInfo::notification_id() const {
    return m_notification_id;
}
void NotificationInfo::setNotification_id(const int64_t &notification_id) {
    m_notification_id = notification_id;
    emit notification_idChanged();
    QVariant v = QVariant::fromValue(notification_id);
    setNotification_id_int(v.toInt());
}

int64_t NotificationInfo::app_id() const {
    return m_app_id;
}
void NotificationInfo::setApp_id(const int64_t &app_id) {
    m_app_id = app_id;
    emit app_idChanged();
    QVariant v = QVariant::fromValue(app_id);
    setApp_id_int(v.toInt());
}

int64_t NotificationInfo::organization_user_id() const {
    return m_organization_user_id;
}
void NotificationInfo::setOrganization_user_id(const int64_t &organization_user_id) {
    m_organization_user_id = organization_user_id;
    emit oidChanged();
    QVariant v = QVariant::fromValue(organization_user_id);
    setOrganization_user_id_int(v.toInt());
}

int NotificationInfo::notification_id_int() const
{
    return m_notification_id_int;
}

void NotificationInfo::setNotification_id_int(const int &notification_id)
{
    m_notification_id_int = notification_id;
    emit notification_id_int_Changed();
}

int NotificationInfo::app_id_int() const
{
    return m_app_id_int;
}

void NotificationInfo::setApp_id_int(const int &app_id)
{
    m_app_id_int = app_id;
    emit app_id_int_Changed();
}

int NotificationInfo::organization_user_id_int() const
{
    return m_organization_user_id_int;
}

void NotificationInfo::setOrganization_user_id_int(const int &organization_user_id)
{
    m_organization_user_id_int = organization_user_id;
    emit oid_int_Changed();
}

QString NotificationInfo::message() const {
    return m_message;
}
void NotificationInfo::setMessage(const QString &message) {
    m_message = message;
    emit messageChanged();
}

QJsonObject NotificationInfo::data() const {
    return m_data;
}
void NotificationInfo::setData(const QJsonObject &data) {
    m_data = data;
    emit dataChanged();
}

QString NotificationInfo::action_name() const {
    return m_action_name;
}
void NotificationInfo::setAction_name(const QString &action_name) {
    m_action_name = action_name;
    emit action_nameChanged();
}

QString NotificationInfo::delivered_date() const {
    return m_delivered_date;
}
void NotificationInfo::setDelivered_date(const QString &delivered_date) {
    m_delivered_date = delivered_date;
    emit delivered_dateChanged();
}

QString NotificationInfo::app_icon() const {
    return m_app_icon;
}
void NotificationInfo::setApp_icon(const QString &app_icon) {
    m_app_icon = app_icon;
    emit app_iconChanged();
}
