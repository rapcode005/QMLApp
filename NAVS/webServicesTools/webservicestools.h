#ifndef WEBSERVICESTOOLS_H
#define WEBSERVICESTOOLS_H

#include <QObject>
#include <QJsonObject>
#include <QtWebEngineCore>

class WebServicesTools: public QObject
{
    Q_OBJECT
public:
    explicit WebServicesTools(QObject *parent = nullptr);

    void computeJsonValueGet(const QString &url, QString sessionKey = "");
    void computeJsonValuePost(const QString &url, QString sessionKey = "");
    void computeJsonValuePost(const QString &url, const QJsonObject &jsonData);
    void computeGetUrlParameter(const QString &url, const QString &parameterName);
    void deleteJsonValue(const QString &url, QString sessionKey = "");
    bool downloadURL(const QString &url, const QString &sessionKey);
    bool downloadURL(const QString &url, const QString &destination, const QString &sessionKey);
    bool downloadURL2(const QString &url, const QString &destination, const QString &sessionKey);//added 7.13.2020
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    QJsonObject getJsonValue();
    QString getApiUrlNavs() const;
    QString getBetaUrl() const;
    QString getParameterValue() const;
    QString getWindowsUsername() const;
    QString getfileName() const;
    void setFileName(const QString &name);
    QString getFolderName() const;
    void setFolderName(const QString &name);
    QNetworkReply *responseDownload;
    QString adjustAppName(const QString &curApp) const;

signals:
    void html(QString sHtml);
    void finished(QNetworkReply *reply);
    void networkError(QNetworkReply::NetworkError code);
    void sendDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void handleHtml(QString sHtml);

private:
    QJsonObject ObjectFromString(const QString &in);
    QJsonObject m_jsonValue;
    QString m_parameterValue;
    QString m_fileName;
    QString m_folderName;
    int m_statusCode;
    void createFolder(const QString &name);
    void requestErrorOccurred(QNetworkReply::NetworkError code);
    int getOrganizationID() const;
    int getLocationID() const;
    QString decryptInstallation() const;
    void createLogsInFile(const QString &url, QString errorMessage = "");
    void createColumn(const QString &str, QFile *file);
    QString searchErrorMessage(const int &code, QNetworkReply *reply);
    struct httpResult {
        int code;
        QString message;
    };

    QList<httpResult> httpStatus;
};

#endif // WEBSERVICESTOOLS_H
