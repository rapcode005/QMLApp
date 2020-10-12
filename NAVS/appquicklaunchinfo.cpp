#include "appquicklaunchinfo.h"

AppQuickLaunchInfo::AppQuickLaunchInfo(const QString &newName,
                                       const int &newId,
                                       const int &newAppId,
                                       const QString &newIcon,
                                       const QString &newUrl,
                                       const int &newSortOrder,
                                       const int &newBadgeCount)
{
    setName(newName);
    setId(newId);
    setAppId(newAppId);
    setIcon(newIcon);
    setUrl(newUrl);
    setSortOrder(newSortOrder);
    setBadgeCount(newBadgeCount);
}

QString AppQuickLaunchInfo::name() const
{
    return m_name;
}

void AppQuickLaunchInfo::setName(const QString &newName)
{
    m_name = newName;
    emit nameChanged();
}

int AppQuickLaunchInfo::id() const
{
    return m_id;
}

void AppQuickLaunchInfo::setId(const int &newId)
{
    m_id = newId;
    emit idChanged();
}

QString AppQuickLaunchInfo::url() const
{
    return m_url;
}

void AppQuickLaunchInfo::setUrl(const QString &newUrl)
{
    m_url = newUrl;
    emit urlChanged();
}

int AppQuickLaunchInfo::sortOrder() const
{
    return m_sortOrder;
}

void AppQuickLaunchInfo::setSortOrder(const int &newSortOrder)
{
    m_sortOrder = newSortOrder;
    emit sortOrderChanged();
}

int AppQuickLaunchInfo::badgeCount() const
{
    return m_badgeCount;
}

void AppQuickLaunchInfo::setBadgeCount(const int &newBadgeCount)
{
    m_badgeCount = newBadgeCount;
    emit badgeCountChanged();
}

int AppQuickLaunchInfo::appId() const
{
    return m_appId;
}

void AppQuickLaunchInfo::setAppId(const int &newAppId)
{
    m_appId = newAppId;
    emit appIdChanged();
}

QString AppQuickLaunchInfo::icon() const
{
    return m_icon;
}

void AppQuickLaunchInfo::setIcon(const QString &newIcon)
{
    m_icon = newIcon;
    emit iconChanged();
}
