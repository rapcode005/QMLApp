#include "navseventfilter.h"
#include <QEvent>
//#include <QDebug>

NavsEventFilter::NavsEventFilter(QObject *parent) :
    QObject(parent)
{
}

bool NavsEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange){
            //qDebug() << Q_FUNC_INFO << " QEvent::WindowStateChange ";
            emit windowStateChange();
            return true;
    }
//    else if (event->type() == QEvent::Close){
//        qDebug() << Q_FUNC_INFO << " QEvent::Close ";
//        return false;
//    }
    else{
        return QObject::eventFilter(obj, event);
    }
}
