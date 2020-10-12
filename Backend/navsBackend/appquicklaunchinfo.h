#ifndef APPQUICKLAUNCHINFO_H
#define APPQUICKLAUNCHINFO_H

#include <QObject>

class AppQuickLaunchInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int appId READ appId WRITE setAppId NOTIFY appIdChanged)

public:
    AppQuickLaunchInfo(const QString &newName, const int &newAppId);

    Q_INVOKABLE QString name() const;
    Q_INVOKABLE void setName(const QString &newName);

    Q_INVOKABLE int appId() const;
    Q_INVOKABLE void setAppId(const int &newAppId);

signals:
    void nameChanged();
    void appIdChanged();

private:
    QString m_name;
    int m_appId;
};

#endif // APPQUICKLAUNCHINFO_H
