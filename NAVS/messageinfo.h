#ifndef MESSAGEINFO_H
#define MESSAGEINFO_H

#include <QObject>

class MessageInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString chatMessage READ chatMessage WRITE setChatMessage NOTIFY chatMessageChanged)

public:
    MessageInfo(QString newUsername, QString newMessage);

    Q_INVOKABLE QString username() const;
    Q_INVOKABLE void setUsername(const QString &username);

    Q_INVOKABLE QString chatMessage() const;
    Q_INVOKABLE void setChatMessage(const QString &message);


signals:
    void usernameChanged();
    void chatMessageChanged();

private:
    QString m_username;
    QString m_message;
};

#endif // MESSAGEINFO_H
