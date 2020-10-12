
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtWebEngine>
#include "backend.h"
#include "organizationModel.h"
#include "organizationinfo.h"
#include "notificationsmodel.h"
#include "notificationinfo.h"
#include "appfilterproxy.h"
#include "navseventfilter.h"
#include "urlrequest.h"
#include "coreappnavs.h"
#include <Windows.h>
#include <QCoreApplication>
#include <QSystemSemaphore>//added 4.13.2020 for application single instance
#include <QSharedMemory>//added 4.13.2020 for application single instance
#include  "errormessage.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Navs");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QtWebEngine::initialize();

    //QApplication app(argc, argv);
    QGuiApplication app(argc, argv);

    //4.13.2020 start single instance of the application
    QSystemSemaphore semaphore("Navs Semaphore", 1);
    semaphore.acquire();

    //for 64_bit windows OS machines
#ifndef Q_OS_WIN64
    QSharedMemory nix_fix_shared_memory("Navs Shared Memory");
    if(nix_fix_shared_memory.attach()){
        nix_fix_shared_memory.detach();
    }
#endif

    QSharedMemory sharedMemory("Navs Shared Memory");
    bool is_running;
    if(sharedMemory.attach()){
        is_running = true;
    }else{
        sharedMemory.create(1);
        is_running = false;
    }
    semaphore.release();

    if(is_running){
        //qDebug() << "the application is already running";
        return 1;
    }else{
        //qDebug() << "the application first run";
    }
    //4.13.2020 end single instance of the application

    qmlRegisterType<Backend>("backend", 1, 5, "BackEnd");

    qmlRegisterType<NotificationsModel>("notificationsmodel", 1, 0, "NotificationsModel");
    qmlRegisterType<AppFilterProxy>("appfilterproxy", 1, 0, "AppFilterProxy");

    NavsEventFilter eventFilter;
    CoreAppNavs coreAppNavs;

    QQmlApplicationEngine engine;
    app.installEventFilter(&eventFilter);
    app.installNativeEventFilter(&coreAppNavs);

    engine.addImportPath("qrc:///");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    QObject::connect(&eventFilter,
                         SIGNAL(windowStateChange()),
                         engine.rootObjects().takeFirst(),
                         SLOT(windowStateChange()));

    return app.exec();

}

