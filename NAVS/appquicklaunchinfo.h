#ifndef APPQUICKLAUNCHINFO_H
#define APPQUICKLAUNCHINFO_H

#include <QObject>

class AppQuickLaunchInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int appId READ appId WRITE setAppId NOTIFY appIdChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(int sortOrder READ sortOrder WRITE setSortOrder
               NOTIFY sortOrderChanged)
    Q_PROPERTY(int badgeCount READ badgeCount WRITE setBadgeCount
               NOTIFY badgeCountChanged)
public:
    AppQuickLaunchInfo(const QString &newName,
                       const int &newId,
                       const int &newAppId,
                       const QString &newIcon,
                       const QString &newUrl,
                       const int &newSortOrder,
                       const int &newBadgeCount);

    Q_INVOKABLE QString name() const;
    Q_INVOKABLE void setName(const QString &newName);

    Q_INVOKABLE int appId() const;
    Q_INVOKABLE void setAppId(const int &newAppId);

    Q_INVOKABLE QString icon() const;
    Q_INVOKABLE void setIcon(const QString &newIcon);

    Q_INVOKABLE int id() const;
    Q_INVOKABLE void setId(const int &newId);

    Q_INVOKABLE QString url() const;
    Q_INVOKABLE void setUrl(const QString &newUrl);

    Q_INVOKABLE int sortOrder() const;
    Q_INVOKABLE void setSortOrder(const int &newSortOrder);

    Q_INVOKABLE int badgeCount() const;
    Q_INVOKABLE void setBadgeCount(const int &newBadgeCount);

signals:
    void nameChanged();
    void appIdChanged();
    void iconChanged();
    void idChanged();
    void urlChanged();
    void sortOrderChanged();
    void badgeCountChanged();

private:
    QString m_name;
    QString m_icon;
    QString m_url;
    int m_appId;
    int m_id;
    int m_sortOrder;
    int m_badgeCount;
};

#endif // APPQUICKLAUNCHINFO_H
