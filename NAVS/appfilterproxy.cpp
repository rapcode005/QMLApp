#include "appfilterproxy.h"

AppFilterProxy::AppFilterProxy(QObject *parent)
 : QSortFilterProxyModel(parent)
{
 setSortOrder(false);
}

AppFilterProxy::~AppFilterProxy(){

}

void AppFilterProxy::setFilterString(QString string)
{
    this->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->setFilterFixedString(string);
}

void AppFilterProxy::setSortOrder(bool checked)
{
    Q_UNUSED(checked)
    this->sort(0, Qt::AscendingOrder);
}
