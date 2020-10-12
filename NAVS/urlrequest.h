#ifndef URLREQUEST_H
#define URLREQUEST_H

#include <QObject>
#include <QWebEngineUrlRequestInterceptor>

class UrlRequest : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
public:
    explicit UrlRequest(QObject *p = Q_NULLPTR);
    void interceptRequest(QWebEngineUrlRequestInfo &info);
    QString sessionKey;
};

#endif // URLREQUEST_H
