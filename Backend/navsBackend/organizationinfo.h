#ifndef ORGANIZATIONINFO_H
#define ORGANIZATIONINFO_H

#include <QObject>

class OrganizationInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString dnsKey READ dnsKey WRITE setDnsKey NOTIFY dnsKeyChanged)
    Q_PROPERTY(int number_location READ number_location WRITE setNumberLocation NOTIFY numberLocationChanged)
    Q_PROPERTY(int locationID READ locationID WRITE setLocationID NOTIFY locationIDChanged)
    Q_PROPERTY(int organizationID READ organizationID WRITE setOrganizationID NOTIFY organizationIDChanged)
    //Q_PROPERTY(int installationTypeID READ installationTypeID WRITE setInstallationTypeID NOTIFY installationTypeIDChanged)
    //Q_PROPERTY(int releaseID READ releaseID WRITE setReleaseID NOTIFY releaseIDChanged)

public:
    OrganizationInfo(QString newName, QString newLocation,
                     int new_number_location, int new_locationID, int new_organizationID
                     , QString newDnsKey);
                     /*QString newIpAddress, int newInstallationTypeID,
                     int newReleaseID);*/

    Q_INVOKABLE QString name() const;
    Q_INVOKABLE void setName(const QString &name);

    Q_INVOKABLE QString location() const;
    Q_INVOKABLE void setLocation(const QString &location);

    Q_INVOKABLE QString dnsKey() const;
    Q_INVOKABLE void setDnsKey(const QString &dnsKey);

    Q_INVOKABLE int number_location() const;
    Q_INVOKABLE void setNumberLocation(const int &numberLocation);

    Q_INVOKABLE int locationID() const;
    Q_INVOKABLE void setLocationID(const int &location_id);

    Q_INVOKABLE int organizationID() const;
    Q_INVOKABLE void setOrganizationID(const int &organizationID);

    //Q_INVOKABLE int installationTypeID() const;
    //Q_INVOKABLE void setInstallationTypeID(const int &installationTypeID);

    //Q_INVOKABLE int releaseID() const;
    //Q_INVOKABLE void setReleaseID(const int &releaseID);

signals:
    void nameChanged();
    void locationChanged();
    void numberLocationChanged();
    void locationIDChanged();
    void organizationIDChanged();
    void dnsKeyChanged();
    //void installationTypeIDChanged();
    //void releaseIDChanged();


private:
    QString m_name;
    QString m_location;
    QString m_dnsKey;
    int m_number_location;
    int m_locationID;
    int m_organizationID;
    //int m_installationTypeID;
    //nt m_releaseID;
};

#endif // ORGANIZATIONINFO_H
