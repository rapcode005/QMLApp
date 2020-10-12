#ifndef LOGGINGTOOLS_H
#define LOGGINGTOOLS_H

#include <QObject>
#include "webServicesTools/webservicestools.h"
#include "installationTools/installationtools.h"

class LoggingTools : public QObject
{
    Q_OBJECT
public:
    explicit LoggingTools(QObject *parent = nullptr);

    void createLogs(const QString &formatName, const QString &logSource,
                    const QString &severity, const QString &errorMessage,
                    const int &userTypeID, const int &organizationID,
                    const int &navsID);
    QJsonObject getLogFormat(const QString &name);
    int getLogSourceID(const QString &name);
    int getOrganizationUserID(const int &organization, const int &userType);
signals:
    void logLoaded(const QJsonObject &jsonResult);

public slots:

private:
    WebServicesTools webServiceTools;
    InstallationTools installationTools;
    //int m_organization_user_id;
};

#endif // LOGGINGTOOLS_H
