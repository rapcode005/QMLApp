#ifndef NAVSOBJECT_H
#define NAVSOBJECT_H

#include <QObject>
#include <QFile>
#include "webServicesTools/webservicestools.h"
#include "workerThread.h"
#include "appmodel.h"

class NavsObject : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE QString getMD5() const;
    Q_PROPERTY(QString getMD5 READ getMD5 NOTIFY md5Changed)

    Q_INVOKABLE QString getISODownloadStatus() const;
    Q_PROPERTY(QString getISODownloadStatus READ getISODownloadStatus NOTIFY isoStatusChanged)

    Q_INVOKABLE int getDownloadPercentage() const;
    Q_PROPERTY(int getDownloadPercentage READ getDownloadPercentage NOTIFY isoStatusPercentageChanged)

    void setSessionKey(const QString &value);


public slots:
    void loadMD5(const QString &filePath);
    void downloadISO(const int isoID, const QString downloadStyle);
    void downloadISOLocation(const QString &msg);
    void downloadRejected();

    void handleResults(const bool &result, const QString &fileName, const QString &folderName);
    void handleResults1(const bool &result, const QString &path);

    //Download Progress
    void downloadISOProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadISOProgressLocation(qint64 bytesReceived, qint64 bytesTotal);

    //Proccess Error
    void errorOccurred(QProcess::ProcessError error);
    void errorOccurredUnableISO(QProcess::ProcessError error);

    void resetBadgeCount(const int appId);

    //Refresh App
    void refreshAppList(const int &orgUserID,
                        const int &orgLctID);
    void refreshApp(const QJsonObject &jsonResult);


signals:
    void md5Changed(const QString &value);
    void isoStatusChanged(const QString &status);
    void resetBadgeCountAction(const int &appId);
    void appLoaded(AppModel *appModel);
    void appQuickLoaded(AppModel *appQuickLaunchModel);
    void isoStatusPercentageChanged(const int &percentaDownloaded);

    //Error Loaded
    void appDataErrorLoaded(const QString &code);
    void quickAppDataErrorLoaded(const QString &code);

private:
    QString m_md5;
    QString m_isoStatus;
    QString m_sessionKey;
    int isoIDNavs;
    //WebServicesTools webServiceTools;
    void createFolder(const QString &name);
    QString getSessionKey() const;
    void startThread(const int &action);
    void startThread(const int &action, const QString &destination);
    void startThread(const int &action, const int &orgUserID, const int &orgLctID);
    QJsonObject m_installedApps;
    int m_downloadPercentage = 0;
};

#endif // NAVSOBJECT_H
