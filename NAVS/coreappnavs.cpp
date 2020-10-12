#include "coreappnavs.h"
#include <Windows.h>
#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QGuiApplication>



CoreAppNavs::CoreAppNavs(QObject *parent) :
    QObject(parent)
{

}

bool CoreAppNavs::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(result)
    Q_UNUSED(eventType)

    MSG *msg = static_cast<MSG*>(message);

    switch(msg->message) {
        case WM_QUERYENDSESSION:{
            ExitProcess(0);
        }
        case WM_ENDSESSION: {
            QGuiApplication::quit();
        }
    }

    return false;
}
