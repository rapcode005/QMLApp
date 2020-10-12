#ifndef NOTIFICATIONSMODEL_H
#define NOTIFICATIONSMODEL_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QList>
#include <QtQml>
#include "notificationinfo.h"

class NotificationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(NotificationsModel)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QQmlListProperty<NotificationInfo> content READ content)
    Q_CLASSINFO("DefaultProperty", "content")

public:
    explicit NotificationsModel(QObject *parent = nullptr);

    static void registerTypes(const char *uri);

    QQmlListProperty<NotificationInfo> content();
    enum ModelRoles {
        NameRole = Qt::UserRole + 1,
        LocationRole,
        NumberLocationRole
    };
    QHash<int, QByteArray> roleNames() const override;
    int count() const;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void append(NotificationInfo* o);
    Q_INVOKABLE void insert(NotificationInfo* o, int i);
    Q_INVOKABLE void deleteItem(const int i);
    Q_INVOKABLE void deleteAll();
    Q_INVOKABLE NotificationInfo *get(int i);

    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void setCount(int count);
    static void dataObjectAppend(QQmlListProperty<NotificationInfo> *list, NotificationInfo *e);
    static int dataObjectCount(QQmlListProperty<NotificationInfo> *list);
    static NotificationInfo *dataObjectAt(QQmlListProperty<NotificationInfo> *list, int i);
    static void dataObjectClear(QQmlListProperty<NotificationInfo> *list);

signals:
    void countChanged(int cnt);

private:
    int m_count = 0;
    int m_columnCount;
    QList<NotificationInfo*> m_dataM;
};

#endif // NOTIFICATIONSMODEL_H
