#include "apptools.h"
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

AppTools::AppTools(QObject *parent) : QObject(parent)
{
}

void AppTools::getAllAppInstall()
{
    QString name, startUrl, icon;
    int appId, orgId, appQuickId, appCategoryId;
    QString url = webServiceTools.getApiUrlNavs() + "api/app-installations";

    webServiceTools.computeJsonValueGet(url);

    QJsonObject jsonResult = webServiceTools.getJsonValue();

    appModel = new AppModel();
    int count = 0;

    foreach (const QJsonValue &values, jsonResult) {

        name = values.toObject().find("display name")->toString();
        appId = values.toObject().find("id")->toInt();

        orgId = values.toObject().find("organization_location_app_id")->toInt();

        //appModel->append(new AppInfo(name, ));
        count += 1;

    }

    appModel->setCount(count);

    emit appLoaded(appModel);
}

void AppTools::getAllAppQuickLaunch()
{
    QString name;
    int appId;
    QString url = webServiceTools.getApiUrlNavs() + "api/app-quick-launches";

    webServiceTools.computeJsonValueGet(url);

    QJsonObject jsonResult = webServiceTools.getJsonValue();

    appQuickLaunchModel = new AppQuickLaunchModel();

    int count = 0;

    foreach (const QJsonValue &values, jsonResult) {
        name = values.toObject().find("name")->toString();
        appId = values.toObject().find("id")->toInt();

        appQuickLaunchModel->append(new AppQuickLaunchInfo(name, appId));

        count += 1;
    }

    appQuickLaunchModel->setCount(count);

    emit appQuickLoaded(appQuickLaunchModel);
}
/*
void AppTools::downloadDataFile()
{

}

void AppTools::createISOFile()
{
    QString downloadFile = "Data";
    QString folder = "sample";

    //Create Temp Folder
    createFolder(folder);

    //Copy isoFiles to Temp Folder
    QDir targetDir(":/isoFiles");
    targetDir.setFilter(QDir::NoDotAndDotDot| QDir::Dirs | QDir::Files);

    QDirIterator it(targetDir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
       it.next();
       QFileInfo Info = it.fileInfo();
       QString testName = Info.fileName();

       QString testPath = QString(targetDir.relativeFilePath(Info.absoluteFilePath()));

       qDebug() << "TEST:" << testPath;
       if(Info.isDir()) {
           QString dirname = Info.filePath();
       }
    }

     Download Codes
    QString url = "WEB_SERVICE_URL";
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(url)));
    QEventLoop event;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString content = response->readAll();




}

void AppTools::createFolder(const QString &name)
{
    if (!QDir(name).exists())
        QDir().mkdir(name);
}
*/
