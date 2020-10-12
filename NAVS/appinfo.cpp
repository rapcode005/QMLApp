#include "appinfo.h"

AppInfo:: AppInfo(const QString &newName,
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
                  const int &newNameLength)
{
    setName(newName);
    setAppId(newAppID);
    setOrganizationLocationAppId(newOrganizationLocationAppId);
    setIcon(newIcon);
    setUrl(newUrl);
    setCategoryApp(newCategoryApp);
    setSortOrderAppCategory(newSortOrderAppCategory);
    setAppInstallId(newAppInstallId);
    setAppQuickLaunchId(newAppQuickLaunchId);
    setAppQuickLaunchSortOrder(newAppQuickLaunchSortOrder);
    setBadgeCount(newBadgeCount);
    setNameLength(newNameLength);
}

QString AppInfo::name() const
{
    return m_name;
}

void AppInfo::setName(const QString &newName)
{
    m_name = newName;
    emit nameChanged();
}

int AppInfo::appId() const
{
    return m_appId;
}

void AppInfo::setAppId(const int &newAppId)
{
    m_appId = newAppId;
    emit appIdChanged();
}

int AppInfo::organizationLocationAppId() const
{
    return m_organizationLocationAppId;
}

void AppInfo::setOrganizationLocationAppId(const int &organizationLocationAppId)
{
    m_organizationLocationAppId = organizationLocationAppId;
    emit organizationLocationAppIdChanged();
}

QString AppInfo::icon() const
{
    return m_icon;
}

void AppInfo::setIcon(const QString &newIcon)
{
    m_icon = newIcon;
    emit iconChanged();
}

QString AppInfo::url() const
{
    return m_url;
}

void AppInfo::setUrl(const QString &newUrl)
{
    m_url = newUrl;
    emit urlChanged();
}

QString AppInfo::categoryApp() const
{
    return m_categoryApp;
}

void AppInfo::setCategoryApp(const QString &newCategoryApp)
{
    m_categoryApp = newCategoryApp;
    emit categoryAppChanged();
}

int AppInfo::sortOrderAppCategory() const
{
    return m_sortOrderAppCategory;
}

void AppInfo::setSortOrderAppCategory(const int &newSortOrderAppCategory)
{
    m_sortOrderAppCategory = newSortOrderAppCategory;
    emit sortOrderAppCategoryChanged();
}

int AppInfo::appInstallId() const
{
    return m_appInstallId;
}

void AppInfo::setAppInstallId(const int &newAppInstallId)
{
    m_appInstallId = newAppInstallId;
    emit appInstallIdChanged();
}

int AppInfo::appQuickLaunchId() const
{
    return m_appQuickLaunchId;
}

void AppInfo::setAppQuickLaunchId(const int &newAppQuickLaunchId)
{
    m_appQuickLaunchId = newAppQuickLaunchId;
    emit appQuickLaunchIdChanged();
}

int AppInfo::appQuickLaunchSortOrder() const
{
    return m_appQuickLaunchSortOrder;
}

void AppInfo::setAppQuickLaunchSortOrder(const int &newAppQuickLaunchSortOrder)
{
    m_appQuickLaunchSortOrder = newAppQuickLaunchSortOrder;
    emit appQuickLaunchSortOrderChanged();
}

int AppInfo::badgeCount() const
{
    return m_badgeCount;
}

void AppInfo::setBadgeCount(const int &newBadgeCount)
{
    m_badgeCount = newBadgeCount;
    emit badgeCountChanged();
}

int AppInfo::nameLength() const
{
    return m_nameLength;
}

void AppInfo::setNameLength(const int &newNameLength)
{
    m_nameLength = newNameLength;
    emit nameLengthChanged();
}
