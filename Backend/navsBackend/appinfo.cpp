#include "appinfo.h"

AppInfo:: AppInfo(const QString &newName, const QString &newIconApp, const QString &newStartUrl,
                  const int &newAppID, const int &newOrganizationId, const int &newAppQuickLaunchId,
                  const int &newAppCategoryId)
{
    setName(newName);
    setIconApp(newIconApp);
    setStartUrl(newStartUrl);
    setAppId(newAppID);
    setOrganizationId(newOrganizationId);
    setAppQuickLaunchId(newAppQuickLaunchId);
    setAppCategoryId(newAppCategoryId);
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

QString AppInfo::iconApp() const
{
    return m_iconApp;
}

void AppInfo::setIconApp(const QString &newIconApp)
{
    m_iconApp = newIconApp;
    emit iconAppChanged();
}

QString AppInfo::startUrl() const
{
    return m_startUrl;
}

void AppInfo::setStartUrl(const QString &newStartUrl)
{
    m_startUrl = newStartUrl;
    emit startUrlChanged();
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

int AppInfo::organizationId() const
{
    return m_organizationId;
}

void AppInfo::setOrganizationId(const int &newOrganizationLocationId)
{
    m_organizationId = newOrganizationLocationId;
    emit organizationIdChanged();
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

int AppInfo::appCategoryId() const
{
    return m_appCategoryId;
}

void AppInfo::setAppCategoryId(const int &newAppCategoryId)
{
    m_appCategoryId = newAppCategoryId;
    emit appCategoryIdChanged();
}

