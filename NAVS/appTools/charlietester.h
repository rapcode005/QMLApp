#ifndef CHARLIETESTER_H
#define CHARLIETESTER_H

#include <QObject>

#include <windows.h>

class CharlieTester : public QObject
{
    Q_OBJECT
public:
    explicit CharlieTester(QObject *parent = nullptr);
    //void checkCheckCharlie2();
    void checkCheckCharlie1();

private:
    bool checkProcessName(QString processName);
    void startCharlie1();
    //void startCharlie2();
    bool checkProcessMatch(DWORD processID, QString processName);

signals:

public slots:

};

#endif // CHARLIETESTER_H
