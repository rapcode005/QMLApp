#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QObject>
#include "webServicesTools/webservicestools.h"

class WorkerThread : public QThread
{
    Q_OBJECT
    void run() override;
public:
    int getAction() const;
    void setAction(const int &action);
    int getDocumentID() const;
    void setDocumentID(const int &value);
    QString getDestination() const;
    void setDestination(const QString &value);
    QString getSessionKey() const;
    void setSessionKey(const QString &value);
    int getOrgUserID() const;
    void setOrgUserID(const int &orgUserID);
    int getOrgLctID() const;
    void setOrgLctID(const int &orgLctID);
    WebServicesTools webServiceTools;

signals:
    void resultReady(const bool &result, const QString &fileName, const QString &folderName);
    void resultReady1(const bool &result, const QString &path);
    //Refresh App List
    void refreshApp(const QJsonObject &result);
    void sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);


private:
    QString m_destination, m_sessionKey;
    int m_action, m_documentID, m_orgUserID, m_orgLctID;
};

#endif // WORKERTHREAD_H
