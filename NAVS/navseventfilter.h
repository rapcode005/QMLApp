#ifndef NAVSEVENTFILTER_H
#define NAVSEVENTFILTER_H

#include <QObject>

class NavsEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit NavsEventFilter(QObject *parent = nullptr);

protected:
     bool eventFilter(QObject *obj, QEvent *event);

signals:
  void windowStateChange();
};

#endif // NAVSEVENTFILTER_H
