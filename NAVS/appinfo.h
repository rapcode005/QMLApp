#ifndef APPINFOMODEL_H
#define APPINFOMODEL_H

#include <QObject>

class AppInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int appId READ appId WRITE setAppId NOTIFY appIdChanged)
    Q_PROPERTY(int organizationLocationAppId READ organizationLocationAppId
               WRITE setOrganizationLocationAppId
               NOTIFY organizationLocationAppIdChanged)
    Q_PROPERTY(QString icon READ icon
               WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString url READ url
               WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString categoryApp READ categoryApp WRITE setCategoryApp
               NOTIFY categoryAppChanged)
    Q_PROPERTY(int sortOrderAppCategory READ sortOrderAppCategory WRITE setSortOrderAppCategory
               NOTIFY sortOrderAppCategoryChanged)
    Q_PROPERTY(int appInstallId READ appInstallId WRITE setAppInstallId
               NOTIFY appInstallIdChanged)
    Q_PROPERTY(int appQuickLaunchId READ appQuickLaunchId WRITE setAppQuickLaunchId
               NOTIFY appQuickLaunchIdChanged)
    Q_PROPERTY(int appQuickLaunchSortOrder READ appQuickLaunchSortOrder WRITE setAppQuickLaunchSortOrder
               NOTIFY appQuickLaunchSortOrderChanged)
    Q_PROPERTY(int badgeCount READ badgeCount WRITE setBadgeCount
               NOTIFY badgeCountChanged)
    Q_PROPERTY(int nameLength READ nameLength WRITE setNameLength
                   NOTIFY nameLengthChanged)

public:
    AppInfo(const QString &newName,
            const int &newAppID,
            const int &newOrganizationLocationAppId,
            const QString &newIcon,
            const QString &newUrl,
            const QString &newCategoryApp,
            const int &newSortOrderAppCategory,
            const int &newAppInstallId,
            const int &newAppQuickLaunchId,
            const int &newAppQuickLaunchSortOrder,
            const int &newBadgeCount,
            const int &newNameLength);

    Q_INVOKABLE QString name() const;
    Q_INVOKABLE void setName(const QString &newName);

    Q_INVOKABLE int appId() const;
    Q_INVOKABLE void setAppId(const int &newAppId);

    Q_INVOKABLE int organizationLocationAppId() const;
    Q_INVOKABLE void setOrganizationLocationAppId(const int &organizationLocationAppId);

    Q_INVOKABLE QString icon() const;
    Q_INVOKABLE void setIcon(const QString &newIcon);

    Q_INVOKABLE QString url() const;
    Q_INVOKABLE void setUrl(const QString &newUrl);

    Q_INVOKABLE QString categoryApp() const;
    Q_INVOKABLE void setCategoryApp(const QString &newCategoryApp);

    Q_INVOKABLE int sortOrderAppCategory() const;
    Q_INVOKABLE void setSortOrderAppCategory(const int &newSortOrderAppCategory);

    Q_INVOKABLE int appInstallId() const;
    Q_INVOKABLE void setAppInstallId(const int &newAppInstallId);

    Q_INVOKABLE int appQuickLaunchId() const;
    Q_INVOKABLE void setAppQuickLaunchId(const int &newAppQuickLaunchId);

    Q_INVOKABLE int appQuickLaunchSortOrder() const;
    Q_INVOKABLE void setAppQuickLaunchSortOrder(const int &newAppQuickLaunchSortOrder);

    Q_INVOKABLE int badgeCount() const;
    Q_INVOKABLE void setBadgeCount(const int &newBadgeCount);

    Q_INVOKABLE int nameLength() const;
    Q_INVOKABLE void setNameLength(const int &newNameLength);

signals:
    void nameChanged();
    void appIdChanged();
    void organizationLocationAppIdChanged();
    void iconChanged();
    void urlChanged();
    void categoryAppChanged();
    void sortOrderAppCategoryChanged();
    void appInstallIdChanged();
    void appQuickLaunchIdChanged();
    void appQuickLaunchSortOrderChanged();
    void badgeCountChanged();
    void nameLengthChanged();

private:
    QString m_name;
    QString m_icon;
    QString m_url;
    QString m_categoryApp;
    int m_appId;
    int m_organizationLocationAppId;
    int m_sortOrderAppCategory;
    int m_appInstallId;
    int m_appQuickLaunchId;
    int m_appQuickLaunchSortOrder;
    int m_badgeCount;
    int m_nameLength;
};

#endif // APPINFOMODEL_H
