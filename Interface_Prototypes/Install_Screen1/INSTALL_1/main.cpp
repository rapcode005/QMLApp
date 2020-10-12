#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "navseventfilter.h"
//#include <QtWebEngine/qtwebengineglobal.h> //You should not need this. see .pro file comments.

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //QtWebEngine::initialize();  //You should not need this.

    NavsEventFilter eventFilter;

    QQmlApplicationEngine engine;

    app.installEventFilter(&eventFilter);

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
