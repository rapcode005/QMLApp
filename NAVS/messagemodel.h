#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QList>
#include <QtQml>
#include "messageinfo.h"

class MessageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(MessageModel)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QQmlListProperty<MessageInfo> content READ content)
    Q_CLASSINFO("DefaultProperty", "content")


public:
    explicit MessageModel(QObject *parent = nullptr);

    static void registerTypes(const char *uri);

    QQmlListProperty<MessageInfo> content();
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

    Q_INVOKABLE void append(MessageInfo* o);
    Q_INVOKABLE void insert(MessageInfo* o, int i);
    Q_INVOKABLE MessageInfo *get(int i);
    int rowCount(const QModelIndex &p) const override;
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void setCount(int count);
    static void dataObjectAppend(QQmlListProperty<MessageInfo> *list, MessageInfo *e);
    static int dataObjectCount(QQmlListProperty<MessageInfo> *list);
    static MessageInfo *dataObjectAt(QQmlListProperty<MessageInfo> *list, int i);
    static void dataObjectClear(QQmlListProperty<MessageInfo> *list);

signals:
    void countChanged(int cnt);

private:
    int m_count;
    int m_columnCount;
    QList<MessageInfo*> m_dataM;
};
#endif // MESSAGEMODEL_H
