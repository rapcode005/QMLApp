#include "messageinfo.h"

MessageInfo::MessageInfo(QString newUsername, QString newMessage)
{
    setChatMessage(newMessage);
    setUsername(newUsername);
}

QString MessageInfo::username() const
{
    return m_username;
}

void MessageInfo::setUsername(const QString &username)
{
    m_username = username;
    emit usernameChanged();
}

QString MessageInfo::chatMessage() const
{
    return m_message;
}

void MessageInfo::setChatMessage(const QString &message)
{
    m_message = message;
    emit chatMessageChanged();
}
