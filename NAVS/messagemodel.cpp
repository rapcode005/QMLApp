#include "messagemodel.h"
#include <QDebug>


MessageModel::MessageModel(QObject *parent):
    QAbstractListModel(parent)
{
    m_columnCount = 3;
}

void MessageModel::registerTypes(const char *uri)
{
    qmlRegisterType<MessageModel>(uri, 1, 0, "Message");
}

QQmlListProperty<MessageInfo> MessageModel::content()
{
    return QQmlListProperty<MessageInfo>(this, nullptr,
                                         &MessageModel::dataObjectAppend,
                                         &MessageModel::dataObjectCount,
                                         &MessageModel::dataObjectAt,
                                         &MessageModel::dataObjectClear);
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    static QHash<int, QByteArray> *roles;
    if (!roles) {
         roles = new QHash<int, QByteArray>;
         (*roles)[Qt::UserRole + 1] = "dataObject";
    }
    return *roles;
}

int MessageModel::count() const
{
    return m_count;
}

QModelIndex MessageModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, (void*)&m_dataM[row])
        : QModelIndex();
}

QModelIndex MessageModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int MessageModel::columnCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)
   return m_columnCount;
}

void MessageModel::append(MessageInfo *o)
{
    int i = m_dataM.size();
    beginInsertRows(QModelIndex(), i, i);

        m_dataM.append(o);

        //Emit changed signals
        emit countChanged(count());

    endInsertRows();
}

void MessageModel::insert(MessageInfo *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_dataM.insert(i, o);

    // Emit changed signals
    emit countChanged(count());

    endInsertRows();
}

MessageInfo *MessageModel::get(int i)
{
    Q_ASSERT(i >= 0 && i <= m_dataM.count());
    return m_dataM[i];
}

int MessageModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_dataM.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    return QVariant::fromValue(m_dataM[index.row()]);
}

void MessageModel::setCount(int cnt)
{
    if (m_count == cnt)
        return;

    m_count = cnt;
    emit countChanged(m_count);
}

void MessageModel::dataObjectAppend(QQmlListProperty<MessageInfo> *list, MessageInfo *e)
{
    MessageModel *om = qobject_cast<MessageModel*>(list->object);
    if (om && e) {
        om->append(e);
    }
}

int MessageModel::dataObjectCount(QQmlListProperty<MessageInfo> *list)
{
    MessageModel *om = qobject_cast<MessageModel*>(list->object);
    if (om) {
        return om->m_dataM.count();
    }
    return 0;
}

MessageInfo* MessageModel::dataObjectAt(QQmlListProperty<MessageInfo> *list, int i)
{
    MessageModel *om = qobject_cast<MessageModel*>(list->object);
    if (om) {
        return om->get(i);
    }
    return nullptr;
}

void MessageModel::dataObjectClear(QQmlListProperty<MessageInfo> *list)
{
   MessageModel *om = qobject_cast<MessageModel*>(list->object);
   if (om) {
       om->m_dataM.clear();
   }
}
