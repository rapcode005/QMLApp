
#include "navsObject.h"

#include <QDataStream>
#include <iostream>
#include <fstream>
#include <random>
#include <cstring>
#include <QCryptographicHash>
#include <QDebug>
#include <QtQuick>
#include <QQmlContext>
#include <QQmlProperty>

void NavsObject::loadMD5(const QString &filePath)
{
    qDebug() << "file: " + filePath;
    QFile f(filePath);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(&f);
        f.close();
        m_md5 = QString::fromUtf8(hash.result().toHex().toStdString().c_str());
    }
    else {
        qDebug() << "cannot open";
        m_md5 = "";
    }
    emit md5Changed(m_md5);
}

QString NavsObject::getMD5() const
{
    return m_md5;
}


void NavsObject::downloadISO(const int isoID, const QString downloadStyle){

    isoIDNavs = isoID;

    if(downloadStyle == "download"){
        //We'll first pop up a folder picker---TODO

        QQuickView *view = new QQuickView();
        view->setSource(QUrl(QStringLiteral("qrc:/SaveFileDialog.qml")));
        QObject *object = view->rootObject();

        if(object){
            QObject *objSaveDlg = object->findChild<QObject*>("saveFileDialog");
            QObject::connect(objSaveDlg, SIGNAL(acceptClicked(QString)),
                    this, SLOT(downloadISOLocation(QString)));
            QObject::connect(objSaveDlg, SIGNAL(rejectedClicked()),
                    this, SLOT(downloadRejected()));
        }

        //view->destroyed(this);
        //object->destroyed(this);

    }else{ //downloadStyle == "view"
       startThread(0);

    }
}

void NavsObject::handleResults(const bool &result, const QString &fileName, const QString &folderName)
{
    if (result) {
        //Send a iso status before running the achieverviewer
        m_isoStatus = "ready";
        emit isoStatusPercentageChanged(100);
        emit isoStatusChanged(m_isoStatus);

        const QString &dirPath = "C:\\Users\\Public\\Downloads";
        const QString &fPath = dirPath + "\\" + folderName + "\\" + fileName;
        const QString &isoParam = QString("$ImagePath= \"%1\"; $ISODrive = (Get-DiskImage -ImagePath $ImagePath | Get-Volume).DriveLetter; IF (!$ISODrive) {Mount-DiskImage -ImagePath $ImagePath -StorageType ISO}; $ISODrive = (Get-DiskImage -ImagePath $ImagePath | Get-Volume).DriveLetter; $command = \"${ISODrive}:\\ArchiveViewer.exe\"; iex \"& $command | Out-Null\";").arg(fPath);
        QProcess *process = new QProcess(this);

        //connect(process, &QProcess::errorOccurred, this, &NavsObject::errorOccurredUnableISO);

        QStringList parameters{isoParam};
        process->setWorkingDirectory(dirPath + "\\" + folderName);
        const QString &cmd("powershell");
        process->start(cmd,parameters);
        process->waitForFinished(5000);


        //Unable to mouth and open the exe file.
        if (process->state() == QProcess::NotRunning) {
            qDebug() << "test";
            emit isoStatusChanged("Unable open exe");
        }

        //Run if the achieverviewer close
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus){
            Q_UNUSED(exitCode)
            Q_UNUSED(exitStatus)

            qDebug() << "cleanup";
            QProcess *processR = new QProcess(this);
            QStringList arguments;
            arguments << "Dismount-DiskImage" << "-ImagePath" << """" + fPath + """";
            qDebug() << arguments;
            processR->start("powershell", arguments);
            processR->waitForFinished(-1);

            //After close the ArchiveViewer, this code will run.
            //Cleanup
            QDir dirs(dirPath + "\\" + folderName);
            dirs.removeRecursively();

            //process->destroyed(this);
            //processR->destroyed(this);
        });
    }
    else {
        m_isoStatus = "error";
        emit isoStatusChanged(m_isoStatus);
    }
}

void NavsObject::handleResults1(const bool &result, const QString &path)
{
    if(result) {

        emit isoStatusPercentageChanged(100);

        m_isoStatus = "ready";
        QProcess *process = new QProcess(this);
        QString prgm;
        prgm = "explorer \"" + QDir::toNativeSeparators(path) + "\"";
        process->startDetached(prgm);
        process->setWorkingDirectory(path);
        process->waitForFinished(-1);

        //process->destroyed(this);
    }
    else
        m_isoStatus = "error";

    emit isoStatusChanged(m_isoStatus);
}

void NavsObject::downloadISOProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    const double &curR = double(bytesReceived);
    const double &curT = double(bytesTotal);
    const double &curTD = (curR / curT);
    const int &total =  static_cast<int>(curTD * 100.0);
    emit isoStatusPercentageChanged(total);
}

void NavsObject::downloadISOProgressLocation(qint64 bytesReceived, qint64 bytesTotal)
{
    const double &curR = double(bytesReceived);
    const double &curT = double(bytesTotal);
    const double &curTD = (curR / curT);
    //const int &total = curTD * 100;
    const int &total = static_cast<int>(curTD * 100.0);
    emit isoStatusPercentageChanged(total);
}

void NavsObject::errorOccurred(QProcess::ProcessError error)
{
    //Q_UNUSED(error)
    qDebug() << "Failed to Open.";
    qDebug() << error;
    emit isoStatusChanged("Unable open exe");
}

void NavsObject::errorOccurredUnableISO(QProcess::ProcessError error)
{
    qDebug() << "Failed to Open.";
    qDebug() << error;
    emit isoStatusChanged("Unable to dismount ISO");
}

void NavsObject::resetBadgeCount(const int appId)
{
    qDebug() << "Reset badge count called";

    emit resetBadgeCountAction(appId);
}

void NavsObject::refreshAppList(const int &orgUserID, const int &orgLctID)
{
    qDebug() << "start refresh App";
    startThread(2, orgUserID, orgLctID);
}

void NavsObject::refreshApp(const QJsonObject &jsonResult)
{
    try {

        QList<int> AlreadyAppList;
        QList<int> AlreadyAppQuickList;

        qDebug() << jsonResult;

        if(jsonResult.size() > 0) {

            const QJsonArray &jsonArray = jsonResult["data"].toArray(), arrayAppQuick;

            AppModel *appModel = new AppModel();
            AppModel *appQuickLaunchModel = new AppModel();
            WebServicesTools webServicesTools;

            foreach (const QJsonValue &value, jsonArray) {
                    const int &id = value.toObject().find("app")->toObject().find("id")->toInt();

                    if(AlreadyAppList.indexOf(id) != -1){
                        continue;
                    }
                    AlreadyAppList.append(id);

                    //qDebug() << value.toObject();

                    //qDebug() << "------------";

                    const QString    &categoryAppName	= value.toObject().find("organization_app_category")->toObject().find("app_category")->toObject().find("name")->toString();
                    const int        &orgLctAppId = value.toObject().find("id")->toInt();
                    const QString    &appName = value.toObject().find("app")->toObject().find("display_name")->toString();
                    const QString    &icon = value.toObject().find("app")->toObject().find("icon")->toString();
                    const int        &sortOrderAppCategory = value.toObject().find("organization_app_category")->toObject().find("sort_order")->toInt();
                    const QString    &urlApp = value.toObject().find("app")->toObject().find("start_url")->toString();
                    const int        &appInstalId = value.toObject().find("app_installations")->toArray()[0].toObject().find("id")->toInt();
                    const QJsonArray &arrayAppQuick = value.toObject().find("app_installations")->toArray()[0].toObject().find("app_quick_launch")->toArray();
                    const int        &appQuickId = (arrayAppQuick.count() < 1) ? 0: arrayAppQuick[0].toObject().find("id")->toInt();

                    const QString &newAppName = appName.length() > 12 ? webServicesTools.adjustAppName(appName) : appName;
                    const int &appNameLength = appName.length(); // Real length without the adjustment

                    AppInfo* ifo = new AppInfo(
                                newAppName,
                                id,
                                orgLctAppId,
                                icon,
                                urlApp,
                                categoryAppName,
                                sortOrderAppCategory,
                                appInstalId,
                                appQuickId,
                                0,
                                0,
                                appNameLength
                    );

                    if(arrayAppQuick.count() > 0){
                        appQuickLaunchModel->append(ifo);
                    }else{
                        appModel->append(ifo);
                    }

                    //ifo->destroyed(this);
            }

            emit appLoaded(appModel);
            emit appQuickLoaded(appQuickLaunchModel);

            //appModel->destroyed(this);
            //appQuickLaunchModel->destroyed(this);

            //Sorry Jon I don't know the format Name, log source or etc.. value;
            //loggingTools->createLogs();

        }
        else {
            emit appDataErrorLoaded("Apps could not be retrieved. API error.");
            emit quickAppDataErrorLoaded("Quick apps could not be retrieved. API error.");
        }

    } catch (...) {
        emit appDataErrorLoaded("Apps could not be retrieved. API error.");
        emit quickAppDataErrorLoaded("Quick apps could not be retrieved. API error.");
    }
}

QString NavsObject::getISODownloadStatus() const
{
    return m_isoStatus;
}

int NavsObject::getDownloadPercentage() const
{
    return m_downloadPercentage;
}

void NavsObject::downloadISOLocation(const QString &msg)
{
    qDebug() << "Called the C++ slot with message:" << msg;
    startThread(1, msg);
}

void NavsObject::downloadRejected()
{
    emit isoStatusChanged("rejected");
}

void NavsObject::createFolder(const QString &name)
{
    if (!QDir(name).exists())
        QDir().mkdir(name);
    else
    {
        //Delete if Exists then create again.
        QDir dir(name);
        dir.removeRecursively();
    }
}

QString NavsObject::getSessionKey() const
{
    return m_sessionKey;
}

void NavsObject::setSessionKey(const QString &value)
{
    m_sessionKey = value;
}

void NavsObject::startThread(const int &action)
{
    WorkerThread *workerThread = new WorkerThread();
    workerThread->setAction(action);
    workerThread->setDocumentID(isoIDNavs);
    workerThread->setSessionKey(getSessionKey());
    connect(workerThread, &WorkerThread::resultReady, this, &NavsObject::handleResults);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    connect(&workerThread->webServiceTools, &WebServicesTools::sendDownloadProgress,
            this, &NavsObject::downloadISOProgress);
    workerThread->start();
}

void NavsObject::startThread(const int &action, const QString &destination)
{
    WorkerThread *workerThread = new WorkerThread();
    workerThread->setAction(action);
    workerThread->setDestination(destination);
    workerThread->setDocumentID(isoIDNavs);
    workerThread->setSessionKey(getSessionKey());
    connect(workerThread, &WorkerThread::resultReady1, this, &NavsObject::handleResults1);
    connect(&workerThread->webServiceTools, &WebServicesTools::sendDownloadProgress,
            this, &NavsObject::downloadISOProgressLocation);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

void NavsObject::startThread(const int &action, const int &orgUserID, const int &orgLctID)
{
    WorkerThread *workerThread = new WorkerThread();
    workerThread->setAction(action);
    workerThread->setOrgLctID(orgLctID);
    workerThread->setOrgUserID(orgUserID);
    workerThread->setSessionKey(getSessionKey());
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    connect(workerThread, &WorkerThread::refreshApp, this, &NavsObject::refreshApp);
    workerThread->start();
}
