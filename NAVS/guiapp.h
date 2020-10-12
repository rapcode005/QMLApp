#ifndef GUIAPP_H
#define GUIAPP_H

#include <QGuiApplication>
#include <QObject>
#include <QSessionManager>

class GuiApp : public QObject
{
    Q_OBJECT

public:
    explicit GuiApp(QObject *parent = nullptr);

public slots:
    void commitDataRequest(QSessionManager &manager);
};

#endif // GUIAPP_H
