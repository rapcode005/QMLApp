#include "appquicklaunchmodel.h"

AppQuickLaunchModel::AppQuickLaunchModel(QObject *parent):
    QAbstractListModel(parent)
{
    m_columnCount = 3;
}

void AppQuickLaunchModel::registerTypes(const char *uri)
{
    qmlRegisterType<AppQuickLaunchModel>(uri, 1, 0, "AppQuickModel");
}

QQmlListProperty<AppQuickLaunchInfo> AppQuickLaunchModel::content()
{
    return QQmlListProperty<AppQuickLaunchInfo>(this, nullptr,
                                         &AppQuickLaunchModel::dataObjectAppend,
                                         &AppQuickLaunchModel::dataObjectCount,
                                         &AppQuickLaunchModel::dataObjectAt,
                                         &AppQuickLaunchModel::dataObjectClear);
}

QHash<int, QByteArray> AppQuickLaunchModel::roleNames() const
{
    static QHash<int, QByteArray> *roles;
    if (!roles) {
         roles = new QHash<int, QByteArray>;
         (*roles)[Qt::UserRole + 1] = "dataObject";
    }
    return *roles;
}

int AppQuickLaunchModel::count() const
{
    return m_count;
}

QModelIndex AppQuickLaunchModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, (void*)&m_data[row])
        : QModelIndex();
}

QModelIndex AppQuickLaunchModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int AppQuickLaunchModel::columnCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)
   return m_columnCount;
}

void AppQuickLaunchModel::append(AppQuickLaunchInfo *o)
{
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);

        m_data.prepend(o);

        //Emit changed signals
        emit countChanged(count());

    endInsertRows();
}

void AppQuickLaunchModel::insert(AppQuickLaunchInfo *o, int i)
{
    beginInsertRows(QModelIndex(), i, i);
    m_data.insert(i, o);

    // Emit changed signals
    emit countChanged(count());

    endInsertRows();
}

AppQuickLaunchInfo *AppQuickLaunchModel::get(int i)
{
    Q_ASSERT(i >= 0 && i <= m_data.count());
    return m_data[i];
}

int AppQuickLaunchModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_data.size();
}

QVariant AppQuickLaunchModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    return QVariant::fromValue(m_data[index.row()]);
}

void AppQuickLaunchModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(m_count);
}

void AppQuickLaunchModel::dataObjectAppend(QQmlListProperty<AppQuickLaunchInfo> *list, AppQuickLaunchInfo *e)
{
    AppQuickLaunchModel *om = qobject_cast<AppQuickLaunchModel*>(list->object);
    if (om && e) {
        om->append(e);
    }
}

int AppQuickLaunchModel::dataObjectCount(QQmlListProperty<AppQuickLaunchInfo> *list)
{
    AppQuickLaunchModel *om = qobject_cast<AppQuickLaunchModel*>(list->object);
    if (om) {
        return om->m_data.count();
    }
    return 0;
}

AppQuickLaunchInfo* AppQuickLaunchModel::dataObjectAt(QQmlListProperty<AppQuickLaunchInfo> *list, int i)
{
    AppQuickLaunchModel *om = qobject_cast<AppQuickLaunchModel*>(list->object);
    if (om) {
        return om->get(i);
    }
    return nullptr;
}

void AppQuickLaunchModel::dataObjectClear(QQmlListProperty<AppQuickLaunchInfo> *list)
{
   AppQuickLaunchModel *om = qobject_cast<AppQuickLaunchModel*>(list->object);
   if (om) {
       om->m_data.clear();
   }
}

