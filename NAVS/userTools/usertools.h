#ifndef USERTOOLS_H
#define USERTOOLS_H

#include <QAbstractListModel>
#include <QObject>
#include <QJsonObject>
#include "webServicesTools/webservicestools.h"
#include "organizationModel.h"
#include "installationTools/installationtools.h"

class UserTools : public QObject
{
    Q_OBJECT
public:
    explicit UserTools(QObject *parent = nullptr);
    void checkOrganization(const QString &s_organizationDnsKey);
    void loginWindowsUsername();
    int getLoginTypeID(const QJsonObject &user) const;
    void loadMeData(const QString &sessionKey);
    void loadOrganizationByID(const int organizationID, const QString &sessionKey);

signals:
    void organizationDnsKeyStatusLoaded(const bool &organizationDnsKeyStatus);
    void organizationDataLoaded(OrganizationModel *organizationModel);
    void loginWindowsUsernameLoaded(const QJsonObject &result);
    void meDataLoaded(const QJsonObject jsonResult);
    void writeLogItem(const QString logVal);
    void organizationDNSKeyErrorLoaded(const QString &code);
    void organizationByIDErrorLoaded(const QString &code);
    void loginWindowsUsernameErrorLoaded(const QString &code);
    void loadMeDataErrorLoaded(const QString &code);
    void networkError(QNetworkReply::NetworkError code);

public slots:
        void requestErrorOccurred(QNetworkReply::NetworkError code);

private:
    QJsonObject jsonOrganization;
    WebServicesTools webServiceTools;
    InstallationTools installationTools;
};

#endif // USERTOOLS_H
