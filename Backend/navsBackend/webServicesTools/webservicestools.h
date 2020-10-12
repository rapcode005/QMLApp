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

    void computeJsonValueGet(const QString &url);
    void computeJsonValuePost(const QString &url, const QString &headerName = "Content-Type",
                          const QString &headerValue = "application/json");
    void computeJsonValuePost(const QString &url, const QJsonObject &jsonData);
    void computeGetUrlParameter(const QString &url, const QString &parameterName);
    QJsonObject getJsonValue();
    QString getApiUrlNavs() const;
    QString getParameterValue() const;
    QString getWindowsUsername() const;


signals:
    void html(QString sHtml);

private slots:
    void handleHtml(QString sHtml);

private:
    QJsonObject ObjectFromString(const QString &in);
    QJsonObject m_jsonValue;
    QString m_parameterValue;
};

#endif // WEBSERVICESTOOLS_H
