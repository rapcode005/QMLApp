#include "organizationModel.h"
#include <QDebug>


OrganizationModel::OrganizationModel(QObject *parent):
    QAbstractListModel(parent)
{
    m_columnCount = 3;
}

void OrganizationModel::registerTypes(const char *uri)
{
    qmlRegisterType<OrganizationModel>(uri, 1, 0, "Organization");
}

QQmlListProperty<OrganizationInfo> OrganizationModel::content()
{
    return QQmlListProperty<OrganizationInfo>(this, nullptr,
                                         &OrganizationModel::dataObjectAppend,
                                         &OrganizationModel::dataObjectCount,
                                         &OrganizationModel::dataObjectAt,
                                         &OrganizationModel::dataObjectClear);
}

QHash<int, QByteArray> OrganizationModel::roleNames() const
{
    static QHash<int, QByteArray> *roles;
    if (!roles) {
         roles = new QHash<int, QByteArray>;
         (*roles)[Qt::UserRole + 1] = "dataObject";
    }
    return *roles;
}

int OrganizationModel::count() const
{
    return m_count;
}

QModelIndex OrganizationModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, (void*)&m_data[row])
        : QModelIndex();
}

QModelIndex OrganizationModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int OrganizationModel::columnCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)
   return m_columnCount;
}

void OrganizationModel::append(OrganizationInfo *o)
{
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);

        m_data.prepend(o);

        //Emit changed signals
        emit countChanged(count());

    endInsertRows();
}

void OrganizationModel::insert(OrganizationInfo *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_data.insert(i, o);

    // Emit changed signals
    emit countChanged(count());

    endInsertRows();
}

OrganizationInfo *OrganizationModel::get(int i)
{
    Q_ASSERT(i >= 0 && i <= m_data.count());
    return m_data[i];
}

int OrganizationModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_data.size();
}

QVariant OrganizationModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    return QVariant::fromValue(m_data[index.row()]);
}

void OrganizationModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(m_count);
}

void OrganizationModel::dataObjectAppend(QQmlListProperty<OrganizationInfo> *list, OrganizationInfo *e)
{
    OrganizationModel *om = qobject_cast<OrganizationModel*>(list->object);
    if (om && e) {
        om->append(e);
    }
}

int OrganizationModel::dataObjectCount(QQmlListProperty<OrganizationInfo> *list)
{
    OrganizationModel *om = qobject_cast<OrganizationModel*>(list->object);
    if (om) {
        return om->m_data.count();
    }
    return 0;
}

OrganizationInfo* OrganizationModel::dataObjectAt(QQmlListProperty<OrganizationInfo> *list, int i)
{
    OrganizationModel *om = qobject_cast<OrganizationModel*>(list->object);
    if (om) {
        return om->get(i);
    }
    return nullptr;
}

void OrganizationModel::dataObjectClear(QQmlListProperty<OrganizationInfo> *list)
{
   OrganizationModel *om = qobject_cast<OrganizationModel*>(list->object);
   if (om) {
       om->m_data.clear();
   }
}
