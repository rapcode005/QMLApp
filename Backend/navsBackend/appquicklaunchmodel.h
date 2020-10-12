#ifndef APPQUICKLAUNCHMODEL_H
#define APPQUICKLAUNCHMODEL_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QList>
#include <QtQml>
#include "appquicklaunchinfo.h"

class AppQuickLaunchModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(AppQuickLaunchModel)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QQmlListProperty<AppQuickLaunchInfo> content READ content)
    Q_CLASSINFO("DefaultProperty", "content")

public:
    explicit AppQuickLaunchModel(QObject *parent = nullptr);

    static void registerTypes(const char *uri);

    QQmlListProperty<AppQuickLaunchInfo> content();
    enum ModelRoles {
        NameRole = Qt::UserRole + 1
    };
    QHash<int, QByteArray> roleNames() const override;
    int count() const;

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex &child) const override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void append(AppQuickLaunchInfo* o);
    Q_INVOKABLE void insert(AppQuickLaunchInfo* o, int i);
    Q_INVOKABLE AppQuickLaunchInfo *get(int i);
    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void setCount(int count);
    static void dataObjectAppend(QQmlListProperty<AppQuickLaunchInfo> *list, AppQuickLaunchInfo *e);
    static int dataObjectCount(QQmlListProperty<AppQuickLaunchInfo> *list);
    static AppQuickLaunchInfo *dataObjectAt(QQmlListProperty<AppQuickLaunchInfo> *list, int i);
    static void dataObjectClear(QQmlListProperty<AppQuickLaunchInfo> *list);

signals:
    void countChanged(int count);

private:
    int m_count;
    int m_columnCount;
    QList<AppQuickLaunchInfo*> m_data;
};

#endif // APPQUICKLAUNCHMODEL_H
