#ifndef ORGANIZATIONMODEL_H
#define ORGANIZATIONMODEL_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QList>
#include <QtQml>
#include "organizationinfo.h"

class OrganizationModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(OrganizationModel)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QQmlListProperty<OrganizationInfo> content READ content)
    Q_CLASSINFO("DefaultProperty", "content")


public:
    explicit OrganizationModel(QObject *parent = nullptr);

    static void registerTypes(const char *uri);

    QQmlListProperty<OrganizationInfo> content();
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

    Q_INVOKABLE void append(OrganizationInfo* o);
    Q_INVOKABLE void insert(OrganizationInfo* o, int i);
    Q_INVOKABLE OrganizationInfo *get(int i);
    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void setCount(int count);
    static void dataObjectAppend(QQmlListProperty<OrganizationInfo> *list, OrganizationInfo *e);
    static int dataObjectCount(QQmlListProperty<OrganizationInfo> *list);
    static OrganizationInfo *dataObjectAt(QQmlListProperty<OrganizationInfo> *list, int i);
    static void dataObjectClear(QQmlListProperty<OrganizationInfo> *list);

signals:
    void countChanged(int count);

private:
    int m_count;
    int m_columnCount;
    QList<OrganizationInfo*> m_data;
};
#endif // ORGANIZATIONMODEL_H
