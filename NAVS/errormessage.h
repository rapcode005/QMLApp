#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QObject>

class ErrorMessage : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QString appDataError READ appDataError
               WRITE setAppDataError NOTIFY appDataErrorChanged)

    Q_PROPERTY(QString quickAppDataError READ quickAppDataError
               WRITE setQuickAppDataError NOTIFY quickAppDataErrorChanged)

    Q_PROPERTY(QString welcomeDataError READ welcomeDataError
               WRITE setWelcomeDataError NOTIFY welcomeDataErrorChanged)

    Q_PROPERTY(QString addQuickAppDataError READ addQuickAppDataError
               WRITE setaddQuickAppDataError NOTIFY addQuickAppDataErrorChanged)

    Q_PROPERTY(QString deleteQuickAppDataError READ deleteQuickAppDataError
               WRITE setDeleteQuickAppDataError NOTIFY deleteQuickAppDataErrorChanged)

    Q_PROPERTY(QString loginUsernamePasswordDataError READ loginUsernamePasswordDataError
               WRITE setLoginUsernamePasswordDataError NOTIFY loginUsernamePasswordDataErrorChanged)

    Q_PROPERTY(QString loginWindowsUsernameDataError READ loginWindowsUsernameDataError
               WRITE setLoginWindowsUsernameDataError NOTIFY loginWindowsUsernameDataErrorChanged)

    Q_PROPERTY(QString loadMeDataError READ loadMeDataError
               WRITE setLoadMeDataError NOTIFY loadMeDataErrorChanged)

    Q_PROPERTY(QString organizationDNSKeyError READ organizationDNSKeyError
               WRITE setOrganizationDNSKeyError NOTIFY organizationDNSKeyErrorChanged)

    Q_PROPERTY(QString organizationByIDError READ organizationByIDError
               WRITE setOrganizationByIDError NOTIFY organizationByIDErrorChanged)

    Q_PROPERTY(QString networkError READ networkError
               WRITE setNetworkError NOTIFY networkErrorChanged)

public:

    Q_INVOKABLE QString appDataError() const;
    Q_INVOKABLE void setAppDataError(const QString &data);

    Q_INVOKABLE QString quickAppDataError() const;
    Q_INVOKABLE void setQuickAppDataError(const QString &data);

    Q_INVOKABLE QString welcomeDataError() const;
    Q_INVOKABLE void setWelcomeDataError(const QString &data);

    Q_INVOKABLE QString addQuickAppDataError() const;
    Q_INVOKABLE void setaddQuickAppDataError(const QString &data);

    Q_INVOKABLE QString deleteQuickAppDataError() const;
    Q_INVOKABLE void setDeleteQuickAppDataError(const QString &data);

    Q_INVOKABLE QString loginUsernamePasswordDataError() const;
    Q_INVOKABLE void setLoginUsernamePasswordDataError(const QString &data);

    Q_INVOKABLE QString loginWindowsUsernameDataError() const;
    Q_INVOKABLE void setLoginWindowsUsernameDataError(const QString &data);

    Q_INVOKABLE QString loadMeDataError() const;
    Q_INVOKABLE void setLoadMeDataError(const QString &data);

    Q_INVOKABLE QString organizationDNSKeyError() const;
    Q_INVOKABLE void setOrganizationDNSKeyError(const QString &data);

    Q_INVOKABLE QString organizationByIDError() const;
    Q_INVOKABLE void setOrganizationByIDError(const QString &data);

    Q_INVOKABLE QString networkError() const;
    Q_INVOKABLE void setNetworkError(const QString &data);

signals:
    void appDataErrorChanged();
    void quickAppDataErrorChanged();
    void welcomeDataErrorChanged();
    void loginWindowsUsernameDataErrorChanged();
    void loginUsernamePasswordDataErrorChanged();
    void deleteQuickAppDataErrorChanged();
    void addQuickAppDataErrorChanged();
    void loadMeDataErrorChanged();
    void organizationDNSKeyErrorChanged();
    void organizationByIDErrorChanged();
    void networkErrorChanged();

private:
    QString m_appDataError, m_quickAppDataError, m_addQuickAppDataError, m_deleteQuickAppDataError,
    m_welcomeDataError, m_loginUsernamePasswordDataError, m_loginWindowsUsernameDataError,
    m_loadMeDataError, m_organizationDNSKeyError, m_organizationByIDError, m_networkError;

};

#endif // ERRORMESSAGE_H
