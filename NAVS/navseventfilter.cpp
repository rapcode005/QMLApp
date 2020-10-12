#include "navseventfilter.h"
#include <QEvent>
#include <QDebug>

NavsEventFilter::NavsEventFilter(QObject *parent) :
    QObject(parent)
{
}

bool NavsEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange){
            emit windowStateChange();
            return true;
    }else{
        return QObject::eventFilter(obj, event);
    }
}
