#include "errormessage.h"


QString ErrorMessage::appDataError() const
{
    return m_appDataError;
}

void ErrorMessage::setAppDataError(const QString &data)
{
    m_appDataError = data;
    emit appDataErrorChanged();
}

QString ErrorMessage::quickAppDataError() const
{
    return m_quickAppDataError;
}

void ErrorMessage::setQuickAppDataError(const QString &data)
{
    m_quickAppDataError = data;
    emit quickAppDataErrorChanged();
}

QString ErrorMessage::welcomeDataError() const
{
    return m_welcomeDataError;
}

void ErrorMessage::setWelcomeDataError(const QString &data)
{
    m_welcomeDataError = data;
    emit welcomeDataErrorChanged();
}

QString ErrorMessage::addQuickAppDataError() const
{
    return m_addQuickAppDataError;
}

void ErrorMessage::setaddQuickAppDataError(const QString &data)
{
    m_addQuickAppDataError = data;
    emit addQuickAppDataErrorChanged();
}

QString ErrorMessage::deleteQuickAppDataError() const
{
    return m_deleteQuickAppDataError;
}

void ErrorMessage::setDeleteQuickAppDataError(const QString &data)
{
    m_deleteQuickAppDataError = data;
    emit deleteQuickAppDataErrorChanged();
}

QString ErrorMessage::loginUsernamePasswordDataError() const
{
    return m_loginUsernamePasswordDataError;
}

void ErrorMessage::setLoginUsernamePasswordDataError(const QString &data)
{
    m_loginUsernamePasswordDataError = data;
    emit loginUsernamePasswordDataErrorChanged();
}

QString ErrorMessage::loginWindowsUsernameDataError() const
{
    return m_loginWindowsUsernameDataError;
}

void ErrorMessage::setLoginWindowsUsernameDataError(const QString &data)
{
    m_loginWindowsUsernameDataError = data;
    emit loginWindowsUsernameDataErrorChanged();
}

QString ErrorMessage::loadMeDataError() const
{
    return m_loadMeDataError;
}

void ErrorMessage::setLoadMeDataError(const QString &data)
{
    m_loadMeDataError = data;
    emit loadMeDataErrorChanged();
}

QString ErrorMessage::organizationDNSKeyError() const
{
    return m_organizationDNSKeyError;
}

void ErrorMessage::setOrganizationDNSKeyError(const QString &data)
{
    m_organizationDNSKeyError = data;
    emit organizationDNSKeyErrorChanged();
}

QString ErrorMessage::organizationByIDError() const
{
    return m_organizationByIDError;
}

void ErrorMessage::setOrganizationByIDError(const QString &data)
{
    m_organizationByIDError = data;
    emit organizationByIDErrorChanged();
}

QString ErrorMessage::networkError() const
{
    return m_networkError;
}

void ErrorMessage::setNetworkError(const QString &data)
{
    m_networkError = data;
    emit networkErrorChanged();
}
