#ifndef TESTER_H
#define TESTER_H

#include <QObject>

#include <windows.h>

class tester : public QObject
{
    Q_OBJECT
public:
    explicit tester(QObject *parent = nullptr);

private:
    bool IsNAVS(DWORD processID);

signals:

public slots:
    bool runCheck( void );
    void startNAVS();
    bool isUpdating();
};


#endif // TESTER_H
