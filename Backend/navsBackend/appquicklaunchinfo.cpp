#include "appquicklaunchinfo.h"

AppQuickLaunchInfo::AppQuickLaunchInfo(const QString &newName, const int &newAppId)
{
    setName(newName);
    setAppId(newAppId);
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

int AppQuickLaunchInfo::appId() const
{
    return m_appId;
}

void AppQuickLaunchInfo::setAppId(const int &newAppId)
{
    m_appId = newAppId;
    emit appIdChanged();
}
