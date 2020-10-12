#include "organizationinfo.h"

OrganizationInfo::OrganizationInfo(QString newName,
                                   QString newLocation,
                                   int new_number_location,
                                   int new_locationID,
                                   int new_organizationID,
                                   QString newDnsKey,
                                   QString logoVal,
                                   QString iconVal,
                                   QJsonObject dataVal)
{
    setName(newName);
    setLocation(newLocation);
    setNumberLocation(new_number_location);
    setLocationID(new_locationID);
    setOrganizationID(new_organizationID);
    setDnsKey(newDnsKey);
    setLogo(logoVal);
    setIcon(iconVal);
    setLocationData(dataVal);
}

QString OrganizationInfo::name() const
{
    return m_name;
}

void OrganizationInfo::setName(const QString &name)
{
    m_name = name;
    emit nameChanged();
}

QString OrganizationInfo::location() const
{
    return m_location;
}

void OrganizationInfo::setLocation(const QString &location)
{
    m_location = location;
    emit locationChanged();
}

QString OrganizationInfo::dnsKey() const
{
    return m_dnsKey;
}

void OrganizationInfo::setDnsKey(const QString &dnsKey)
{
    m_dnsKey = dnsKey;
    emit dnsKeyChanged();
}

int OrganizationInfo::number_location() const
{
    return m_number_location;
}

void OrganizationInfo::setNumberLocation(const int &numberLocation)
{
    m_number_location = numberLocation;
    emit numberLocationChanged();
}

int OrganizationInfo::locationID() const
{
    return m_locationID;
}

void OrganizationInfo::setLocationID(const int &location_id)
{
    m_locationID = location_id;
    emit locationIDChanged();
}

int OrganizationInfo::organizationID() const
{
    return m_organizationID;
}

void OrganizationInfo::setOrganizationID(const int &organizationID)
{
    m_organizationID = organizationID;
    emit organizationIDChanged();
}

QString OrganizationInfo::logo() const
{
    return m_logo;
}

void OrganizationInfo::setLogo(const QString &logoVal)
{
    m_logo = logoVal;
    emit logoChanged();
}

QString OrganizationInfo::icon() const
{
    return m_icon;
}

void OrganizationInfo::setIcon(const QString &iconVal)
{
    m_icon = iconVal;
    emit iconChanged();
}

QJsonObject OrganizationInfo::locationData() const
{
    return m_locationData;
}

void OrganizationInfo::setLocationData(const QJsonObject &dataVal)
{
    m_locationData = dataVal;
    emit locationChanged();
}
