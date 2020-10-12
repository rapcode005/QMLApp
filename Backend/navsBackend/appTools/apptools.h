#ifndef APPTOOLS_H
#define APPTOOLS_H

#include <QObject>
#include "webServicesTools/webservicestools.h"
#include "appmodel.h"
#include "appinfo.h"
#include "appquicklaunchmodel.h"

class AppTools : public QObject
{
    Q_OBJECT

public:
    explicit AppTools(QObject *parent = nullptr);
    void getAllAppInstall();
    void getAllAppQuickLaunch();
    //void downloadDataFile();
    //void createISOFile();
    //void createFolder(const QString &name);

signals:
    void appLoaded(AppModel *appModel);
    void appQuickLoaded(AppQuickLaunchModel *appQuickLaunchModel);

public slots:

private:
    WebServicesTools webServiceTools;
    AppModel *appModel;
    AppQuickLaunchModel *appQuickLaunchModel;
};

#endif // APPTOOLS_H
