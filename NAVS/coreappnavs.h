#ifndef COREAPPNAVS_H
#define COREAPPNAVS_H

#include <QAbstractNativeEventFilter>
#include <QObject>
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
class CoreAppNavs :  public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit CoreAppNavs(QObject *parent = nullptr);
    //virtual BOOL ExitWindowsEx(UINT uFlags, DWORD dwReason);

protected:
    //void aboutToQuit();
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override ;
};

#endif // COREAPPNAVS_H



