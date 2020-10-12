#ifndef APPFILTERPROXY_H
#define APPFILTERPROXY_H

#include <QObject>
#include <QSortFilterProxyModel>

class AppFilterProxy : public QSortFilterProxyModel
{
 Q_OBJECT
public:

AppFilterProxy(QObject* parent = 0);

~AppFilterProxy();

Q_INVOKABLE void setFilterString(QString string);

Q_INVOKABLE void setSortOrder(bool checked);
};

#endif // APPFILTERPROXY_H
