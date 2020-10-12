#include "appmodel.h"

AppModel::AppModel(QObject *parent):
    QAbstractListModel(parent)
{
    m_columnCount = 3;
}

void AppModel::registerTypes(const char *uri)
{
    qmlRegisterType<AppModel>(uri, 1, 0, "AppsModel");
}

QQmlListProperty<AppInfo> AppModel::content()
{
    return QQmlListProperty<AppInfo>(this, nullptr,
                                         &AppModel::dataObjectAppend,
                                         &AppModel::dataObjectCount,
                                         &AppModel::dataObjectAt,
                                         &AppModel::dataObjectClear);
}

QHash<int, QByteArray> AppModel::roleNames() const
{
    static QHash<int, QByteArray> *roles;
    if (!roles) {
         roles = new QHash<int, QByteArray>;
         (*roles)[Qt::UserRole + 1] = "dataObject";
    }
    return *roles;
}

int AppModel::count() const
{
    return m_count;
}

QModelIndex AppModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, (void*)&m_data[row])
        : QModelIndex();
}

QModelIndex AppModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int AppModel::columnCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)
   return m_columnCount;
}

void AppModel::append(AppInfo *o)
{
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);

        m_data.prepend(o);

        //Emit changed signals
        emit countChanged(count());

    endInsertRows();
}

void AppModel::insert(AppInfo *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_data.insert(i, o);

    // Emit changed signals
    emit countChanged(count());

    endInsertRows();
}

AppInfo *AppModel::get(int i)
{
    Q_ASSERT(i >= 0 && i <= m_data.count());
    return m_data[i];
}

int AppModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_data.size();
}

QVariant AppModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    return QVariant::fromValue(m_data[index.row()]);
}

void AppModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(m_count);
}

void AppModel::dataObjectAppend(QQmlListProperty<AppInfo> *list, AppInfo *e)
{
    AppModel *om = qobject_cast<AppModel*>(list->object);
    if (om && e) {
        om->append(e);
    }
}

int AppModel::dataObjectCount(QQmlListProperty<AppInfo> *list)
{
    AppModel *om = qobject_cast<AppModel*>(list->object);
    if (om) {
        return om->m_data.count();
    }
    return 0;
}

AppInfo* AppModel::dataObjectAt(QQmlListProperty<AppInfo> *list, int i)
{
    AppModel *om = qobject_cast<AppModel*>(list->object);
    if (om) {
        return om->get(i);
    }
    return nullptr;
}

void AppModel::dataObjectClear(QQmlListProperty<AppInfo> *list)
{
   AppModel *om = qobject_cast<AppModel*>(list->object);
   if (om) {
       om->m_data.clear();
   }
}

