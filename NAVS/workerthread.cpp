#include "workerThread.h"
#include <QDebug>

void WorkerThread::run()
{
    bool result = false;
    QString folderName = "", fileName = "";

    QSettings settings;
    settings.beginGroup("Servers");
    QVariant value = settings.value("echo2_url", "https://cubackups.navsnow.com/api/documents/");
    settings.endGroup();
    QString res = value.toString();

    const QString &urlISO = res + QString::number(getDocumentID()) + "/download";

    switch (getAction()) {
        case 0: {
            const bool &download = webServiceTools.downloadURL(urlISO, getSessionKey());
            if(!download) {
                result = false;
            }
            else {
                fileName = webServiceTools.getfileName();
                folderName = webServiceTools.getFolderName();
                result = true;
            }
            emit resultReady(result, fileName, folderName);
            break;
        }
        case 1: {
            const bool &download = webServiceTools.downloadURL(urlISO, getSessionKey(), getDestination());
            if(!download) {
                result = false;
            }
            else {
                result = true;
            }
            emit resultReady1(result, getDestination());
            break;
        }
        default: {

            const QString &url = webServiceTools.getApiUrlNavs() + "api/users-apps?organization_user_id=" + QString::number(getOrgUserID()) +
                    "&organization_location_id=" + QString::number(getOrgLctID());

            webServiceTools.computeJsonValueGet(url, getSessionKey());

            const QJsonObject &result = webServiceTools.getJsonValue();

            emit refreshApp(result);
            break;
        }
    }
}

int WorkerThread::getAction() const
{
    return m_action;
}

void WorkerThread::setAction(const int &action)
{
    m_action = action;
}

int WorkerThread::getDocumentID() const
{
    return m_documentID;
}

void WorkerThread::setDocumentID(const int &value)
{
    m_documentID = value;
}

QString WorkerThread::getDestination() const
{
    return m_destination;
}

void WorkerThread::setDestination(const QString &value)
{
    m_destination = value;
}

QString WorkerThread::getSessionKey() const
{
    return m_sessionKey;
}

void WorkerThread::setSessionKey(const QString &value)
{
    m_sessionKey = value;
}

int WorkerThread::getOrgUserID() const
{
    return m_orgUserID;
}

void WorkerThread::setOrgUserID(const int &orgUserID)
{
    m_orgUserID = orgUserID;
}

int WorkerThread::getOrgLctID() const
{
    return m_orgLctID;
}

void WorkerThread::setOrgLctID(const int &orgLctID)
{
    m_orgLctID = orgLctID;
}
