#include "urlrequest.h"
#include <QDebug>

UrlRequest::UrlRequest(QObject *p):
    QWebEngineUrlRequestInterceptor(p)
{

}

void UrlRequest::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    //QList<QByteArray> key, value;
    QByteArray key, value;
    key.append(QByteArray("NAVS-SESSION"));
    value.append(sessionKey.toUtf8());

    info.setHttpHeader(key, value);
}
