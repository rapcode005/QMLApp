#ifndef APPMODEL_H
#define APPMODEL_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QList>
#include <QtQml>
#include "appinfo.h"

class AppModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(AppModel)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QQmlListProperty<AppInfo> content READ content)
    Q_CLASSINFO("DefaultProperty", "content")

public:
    explicit AppModel(QObject *parent = nullptr);

    static void registerTypes(const char *uri);

    QQmlListProperty<AppInfo> content();

    enum ModelRoles {
        NameRole = Qt::UserRole + 1,
        CategoryRole = Qt::UserRole + 2,
    };

    QHash<int, QByteArray> roleNames() const override;

    int count() const;

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex &child) const override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void append(AppInfo* o);
    Q_INVOKABLE void insert(AppInfo* o, int i);
    Q_INVOKABLE void deleteItem(const int i);
    Q_INVOKABLE void deleteAll();
    Q_INVOKABLE AppInfo *get(int i);

    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    static void dataObjectAppend(QQmlListProperty<AppInfo> *list, AppInfo *e);
    static int dataObjectCount(QQmlListProperty<AppInfo> *list);
    static AppInfo *dataObjectAt(QQmlListProperty<AppInfo> *list, int i);
    static void dataObjectClear(QQmlListProperty<AppInfo> *list);

signals:
    void countChanged(int count);

private:
    int m_count = 0;
    int m_columnCount;
    QList<AppInfo*> m_data;
    void setCount(int count);

};

#endif // APPMODEL_H
