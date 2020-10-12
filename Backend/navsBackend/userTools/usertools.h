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
    void checkOrganization(QString s_organizationDnsKey);
    void loadOrganization(QString s_organization);
    //void loginOrganization(const QString &username, const QString &password);
    void loginWindowsUsername();
    //void logoutOrganization(const QString &username, const QString &password);
    void loadUser();

signals:
    void organizationDnsKeyStatusLoaded(const bool &organizationDnsKeyStatus);
    void organizationDataLoaded(OrganizationModel *organizationModel);
    void userDataLoaded(QJsonObject userObject);
    void loginWindowsUsernameLoaded(QJsonObject result);
    //void logoutOrganizationLoaded(QJsonObject result);

public slots:


private:
    QJsonObject jsonOrganization;
    OrganizationModel *item;
    WebServicesTools webServiceTools;
    InstallationTools installationTools;

    int getLoginTypeID();
    //QObject* fromJson(const QMetaObject &meta, QString &json);
    //QObject* fromJson(const QMetaObject *meta, QJsonObject &jsonObject);
    //QVariant jsonValueToProperty(QObject *object, QMetaProperty &property, QJsonValue value);
};

#endif // USERTOOLS_H
