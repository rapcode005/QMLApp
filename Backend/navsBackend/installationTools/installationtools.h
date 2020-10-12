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
    //void createInstallationFile(QString *code);
    QString generateCode() const;
    void createInstallationKeyFile(const QString &code);
    QString getInstallationKeyFile();
    void checkregisterStatus();
    //void loadInstallationData();
    void loadInstallationData(OrganizationModel *organizationModel, const int &index);
    void loginUsername(const QString &username,
                       const QString &password,
                       const int &organizationID);
    //void loadInstallationData();
    int getInstallationTypeID();
    QString getIPAddress();
    int getReleaseID();
    void deleteInstallationID();
    void createISOHelper();
    QString getInfoFromBeta();

signals:
    void successLoaded(QJsonObject installationObject);
    void registerStatusLoaded(const bool &status);
    void installationDataLoaded(const QJsonObject &installationObject);
    void loginUsernameLoaded(const QJsonObject &data);

private:
    WebServicesTools webServiceTools;
    QString stringMultiple() const;
    void hideFile();
    void downloadISOFiles();
    void createFolder(const QString &name);
    void unZipFile();
    void runFile();
    QString encryptDecrypt(const QString &code);
    void loadInfoFromBeta1();
    void cleanup();
    void deleteFiles(const QString &name);
};

#endif // INSTALLATIONTOOLS_H
