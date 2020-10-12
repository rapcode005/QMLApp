#include "organizationinfo.h"

OrganizationInfo::OrganizationInfo(QString newName, QString newLocation,
                                   int new_number_location, int new_locationID, int new_organizationID
                                   , QString newDnsKey)
                                   /*QString newIpAddress, int newInstallationTypeID,
                                   int newReleaseID)*/
{
    setName(newName);
    setLocation(newLocation);
    setNumberLocation(new_number_location);
    setLocationID(new_locationID);
    setOrganizationID(new_organizationID);
    setDnsKey(newDnsKey);
    //setIPAddress(newIpAddress);
    //setInstallationTypeID(newInstallationTypeID);
    //setReleaseID(newReleaseID);
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
/*
QString OrganizationInfo::ipAddress() const
{
    return m_ipAddress;
}

void OrganizationInfo::setIPAddress(const QString &ipAddress)
{
    m_ipAddress = ipAddress;
    emit ipAddressChanged();
}
*/
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
/*
int OrganizationInfo::installationTypeID() const
{
    return m_installationTypeID;
}

void OrganizationInfo::setInstallationTypeID(const int &installationTypeID)
{
    m_installationTypeID = installationTypeID;
    emit installationTypeIDChanged();
}

int OrganizationInfo::releaseID() const
{
    return m_releaseID;
}

void OrganizationInfo::setReleaseID(const int &releaseID)
{
    m_releaseID = releaseID;
    emit releaseIDChanged();
}
*/
