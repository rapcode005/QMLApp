#ifndef ORGANIZATIONINFO_H
#define ORGANIZATIONINFO_H

#include <QObject>
#include <QJsonObject>

class OrganizationInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString dnsKey READ dnsKey WRITE setDnsKey NOTIFY dnsKeyChanged)
    Q_PROPERTY(int number_location READ number_location WRITE setNumberLocation NOTIFY numberLocationChanged)
    Q_PROPERTY(int locationID READ locationID WRITE setLocationID NOTIFY locationIDChanged)
    Q_PROPERTY(int organizationID READ organizationID WRITE setOrganizationID NOTIFY organizationIDChanged)
    Q_PROPERTY(QString logo READ logo WRITE setLogo NOTIFY logoChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QJsonObject locationData READ locationData WRITE setLocationData NOTIFY locationDataChanged)

public:
    OrganizationInfo(QString newName,
                     QString newLocation,
                     int new_number_location,
                     int new_locationID,
                     int new_organizationID,
                     QString newDnsKey,
                     QString logoVal,
                     QString iconVal,
                     QJsonObject dataVal);

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

    Q_INVOKABLE QString logo() const;
    Q_INVOKABLE void setLogo(const QString &logoVal);

    Q_INVOKABLE QString icon() const;
    Q_INVOKABLE void setIcon(const QString &iconVal);

    Q_INVOKABLE QJsonObject locationData() const;
    Q_INVOKABLE void setLocationData(const QJsonObject &dataVal);


signals:
    void nameChanged();
    void locationChanged();
    void numberLocationChanged();
    void locationIDChanged();
    void organizationIDChanged();
    void dnsKeyChanged();
    void logoChanged();
    void iconChanged();
    void locationDataChanged();


private:
    QString m_name;
    QString m_location;
    QString m_dnsKey;
    int m_number_location;
    int m_locationID;
    int m_organizationID;
    QString m_logo;
    QString m_icon;
    QJsonObject m_locationData;
};

#endif // ORGANIZATIONINFO_H
