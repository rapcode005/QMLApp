#include "notificationsmodel.h"

NotificationsModel::NotificationsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_columnCount = 9;
}

void NotificationsModel::registerTypes(const char *uri)
{
    qmlRegisterType<NotificationsModel>(uri, 1, 0, "Notification");
}

QQmlListProperty<NotificationInfo> NotificationsModel::content()
{
    return QQmlListProperty<NotificationInfo>(this, nullptr,
                                         &NotificationsModel::dataObjectAppend,
                                         &NotificationsModel::dataObjectCount,
                                         &NotificationsModel::dataObjectAt,
                                         &NotificationsModel::dataObjectClear);
}

QHash<int, QByteArray> NotificationsModel::roleNames() const
{
    static QHash<int, QByteArray> *roles;
    if (!roles) {
         roles = new QHash<int, QByteArray>;
         (*roles)[Qt::UserRole + 1] = "dataObject";
    }
    return *roles;
}

int NotificationsModel::count() const
{
    return m_count;
}

QModelIndex NotificationsModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, (void*)&m_dataM[row])
        : QModelIndex();
}

QModelIndex NotificationsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int NotificationsModel::columnCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)
   return m_columnCount;
}

void NotificationsModel::deleteItem(const int i)
{
    beginRemoveRows(QModelIndex(), i, i);
        m_dataM.takeAt(i);

        setCount(count()-1);
        //Emit changed signals
        emit countChanged(count());
    endRemoveRows();
}

void NotificationsModel::deleteAll()
{
    const int preCount = count();
    if(preCount > 0){
        for(int i = preCount-1; i>=0; i--){
            deleteItem(i);
        }
    }
}

void NotificationsModel::append(NotificationInfo *o)
{
    int i = m_dataM.size();
    beginInsertRows(QModelIndex(), i, i);

        m_dataM.append(o);

        setCount(count()+1);
        //Emit changed signals
        emit countChanged(count());

    endInsertRows();
}

void NotificationsModel::insert(NotificationInfo *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);

        m_dataM.insert(i, o);

        setCount(count()+1);
        // Emit changed signals
        emit countChanged(count());

    endInsertRows();
}

NotificationInfo *NotificationsModel::get(int i)
{
    Q_ASSERT(i >= 0 && i <= m_dataM.count());
    return m_dataM[i];
}

int NotificationsModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_dataM.size();
}

QVariant NotificationsModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    return QVariant::fromValue(m_dataM[index.row()]);
}

void NotificationsModel::setCount(int cnt)
{
    if (m_count == cnt)
        return;

    m_count = cnt;
    emit countChanged(m_count);
}

void NotificationsModel::dataObjectAppend(QQmlListProperty<NotificationInfo> *list, NotificationInfo *e)
{
    NotificationsModel *om = qobject_cast<NotificationsModel*>(list->object);
    if (om && e) {
        om->append(e);
    }
}

int NotificationsModel::dataObjectCount(QQmlListProperty<NotificationInfo> *list)
{
    NotificationsModel *om = qobject_cast<NotificationsModel*>(list->object);
    if (om) {
        return om->m_dataM.count();
    }
    return 0;
}

NotificationInfo* NotificationsModel::dataObjectAt(QQmlListProperty<NotificationInfo> *list, int i)
{
    NotificationsModel *om = qobject_cast<NotificationsModel*>(list->object);
    if (om) {
        return om->get(i);
    }
    return nullptr;
}

void NotificationsModel::dataObjectClear(QQmlListProperty<NotificationInfo> *list)
{
   NotificationsModel *om = qobject_cast<NotificationsModel*>(list->object);
   if (om) {
       om->m_dataM.clear();
   }
}
