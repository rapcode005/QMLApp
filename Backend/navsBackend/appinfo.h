#ifndef APPINFOMODEL_H
#define APPINFOMODEL_H

#include <QObject>

class AppInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString iconApp READ iconApp WRITE setIconApp NOTIFY iconAppChanged)
    Q_PROPERTY(QString startUrl READ startUrl WRITE setStartUrl NOTIFY startUrlChanged)
    Q_PROPERTY(int appId READ appId
               WRITE setAppId
               NOTIFY appIdChanged)
    Q_PROPERTY(int organizationId READ organizationId
               WRITE setOrganizationId
               NOTIFY organizationIdChanged)
    Q_PROPERTY(int appQuickLaunchId READ appQuickLaunchId
               WRITE setAppQuickLaunchId
               NOTIFY appQuickLaunchIdChanged)
    Q_PROPERTY(int appCategoryId READ appCategoryId
               WRITE setAppCategoryId
               NOTIFY appCategoryIdChanged)
public:
    AppInfo(const QString &newName, const QString &newIconApp, const QString &newStartUrl,
                const int &newAppID, const int &newOrganizationId, const int &newAppQuickLaunchId,
            const int &newAppCategoryId);

    Q_INVOKABLE QString name() const;
    Q_INVOKABLE void setName(const QString &newName);

    Q_INVOKABLE QString iconApp() const;
    Q_INVOKABLE void setIconApp(const QString &newIconApp);

    Q_INVOKABLE QString startUrl() const;
    Q_INVOKABLE void setStartUrl(const QString &newStartUrl);

    Q_INVOKABLE int appId() const;
    Q_INVOKABLE void setAppId(const int &newAppId);

    Q_INVOKABLE int organizationId() const;
    Q_INVOKABLE void setOrganizationId(const int &newOrganizationId);

    Q_INVOKABLE int appQuickLaunchId() const;
    Q_INVOKABLE void setAppQuickLaunchId(const int &newAppQuickLaunchId);

    Q_INVOKABLE int appCategoryId() const;
    Q_INVOKABLE void setAppCategoryId(const int &newAppCategoryId);

signals:
    void nameChanged();
    void appIdChanged();
    void organizationIdChanged();
    void iconAppChanged();
    void appQuickLaunchIdChanged();
    void startUrlChanged();
    void appCategoryIdChanged();

private:
    QString m_name;
    QString m_iconApp;
    QString m_startUrl;
    int m_appId;
    int m_organizationId;
    int m_appQuickLaunchId;
    int m_appCategoryId;
};

#endif // APPINFOMODEL_H
