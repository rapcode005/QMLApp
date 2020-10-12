#include "guiapp.h"

GuiApp::GuiApp(QObject *parent) :
    QObject(parent)
{

}

void GuiApp::commitDataRequest(QSessionManager &manager)
{
     if (manager.allowsInteraction()) {
         QGuiApplication::quit();
     }

}
