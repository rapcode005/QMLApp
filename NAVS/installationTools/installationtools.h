#ifndef INSTALLATIONTOOLS_H
#define INSTALLATIONTOOLS_H

#include <QObject>
#include <webServicesTools/webservicestools.h>
#include <installationTools/cryptdevice.h>
#include <organizationModel.h>

class InstallationTools : public QObject
{
    Q_OBJECT
public:
    explicit InstallationTools(QObject *parent = nullptr);
    QString generateCode() const;
    void createInstallationKeyFile(const QString &code,
                                   const int &orgId,
                                   const int &orgLocationId);
    QString getInstallationKeyFile() const;
    int getOrganizationID() const;
    int getLocationID() const;
    void checkregisterStatus();
    void sendInstallationData(OrganizationModel *organizationModel,
                              const int &index,
                              const QString &sessionKey);
    void loginUsername(const QString &username,
                       const QString &password,
                       const int &organizationID);
    int getInstallationTypeID();
    QString getIPAddress() const;
    int getReleaseID(const QString &sessionKey);
    void deleteInstallationID();
    void getAvailableVersionNumberXML();

signals:
    void successLoaded(const QJsonObject &installationObject);
    void registerStatusLoaded(const bool &status);
    void installationDataLoaded(const QJsonObject &installationObject);
    void loginUsernameLoaded(const QJsonObject &data);
    void foundAvailableVersion(const QString versionFound);
    void loginUsernameErrorLoaded(const QString &code);
    void networkError(QNetworkReply::NetworkError code);

public slots:
        void requestErrorOccurred(QNetworkReply::NetworkError code);

private:
    WebServicesTools webServiceTools;
    void hideFile();
    void downloadISOFiles();
    QString fileData() const;
    QByteArray encryptInstallation(const QString &code);
    QString decryptInstallation() const;
    int newCodes;
};

#endif // INSTALLATIONTOOLS_H
