#include "webservicestools.h"

WebServicesTools::WebServicesTools(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(html(QString)), this, SLOT(handleHtml(QString)));
}


void WebServicesTools::computeJsonValueGet(const QString &url)
{
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl(url));
    //req.setHeader(K, headerValue)
    QNetworkReply *reply = mgr->get(req);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QString html =static_cast<QString>(reply->readAll());
        m_jsonValue = ObjectFromString(html);
        reply->abort();
    }

    //eventLoop.exit();
}

void WebServicesTools::computeJsonValuePost(const QString &url,
                                            const QString &headerName,
                                            const QString &headerValue)
{
    QEventLoop eventLoop;

    QUrlQuery postData;
    postData.addQueryItem(headerName, headerValue);

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = mgr->post(req, postData.toString(QUrl::FullyEncoded).toUtf8());
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QString html = static_cast<QString>(reply->readAll());
        m_jsonValue = ObjectFromString(html);
        reply->abort();
    }

}

void WebServicesTools::computeJsonValuePost(const QString &url, const QJsonObject &jsonData)
{
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = mgr->post(req, QJsonDocument(jsonData).toJson());
    eventLoop.exec();

    //connect(reply, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onRedirected(const QUrl&));

    if (reply->error() == QNetworkReply::NoError) {

        QNetworkRequest req2;
        req2.setUrl(reply->url());
        QNetworkReply *reply2 = mgr->get(req2);
        eventLoop.exec();

        if (reply2->error() == QNetworkReply::NoError) {
            QString html = static_cast<QString>(reply2->readAll());
            m_jsonValue = ObjectFromString(html);
            reply2->deleteLater();
        }

        /*QString html = reply->readAll();
        qDebug() << html;
        m_jsonValue = ObjectFromString(html);
        reply->deleteLater();*/

    }


}

void WebServicesTools::computeGetUrlParameter(const QString &url, const QString &parameterName)
{
    QEventLoop eventLoop;
    m_parameterValue = "";

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req;
    req.setUrl(QUrl(url));
    //req.setHeader(K, headerValue)
    QNetworkReply *reply = mgr->get(req);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QString parameter = "";
        QString url = reply->url().toString();
        if(url.split("?").length() > 1)
        {
            parameter = url.split("?")[1];
            QStringList parameterList = parameter.split("&");

            //Get value from parameter
            for(int i = 0; i < parameterList.length(); i++)
            {
                //Check if parameter value from given parameter
                QStringList div = parameterList[i].split("=");
                if(div.length() > 1 && parameterName == div[0])
                {
                    m_parameterValue = div[1];
                }
            }

        }
        reply->abort();
    }
}


QString WebServicesTools::getApiUrlNavs() const
{
    return "http://ec2-44-225-216-138.us-west-2.compute.amazonaws.com/";
}

QString WebServicesTools::getParameterValue() const
{
    return m_parameterValue;
}

QString WebServicesTools::getWindowsUsername() const
{
    QString name = qgetenv("USER");
    if (name.isEmpty())
        return qgetenv("USERNAME");
    else
        return name;
}

QJsonObject WebServicesTools::ObjectFromString(const QString &in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    obj = doc.object();

    return obj;
}

QJsonObject WebServicesTools::getJsonValue()
{
    return m_jsonValue;
}

void WebServicesTools::handleHtml(QString sHtml)
{
    m_jsonValue = ObjectFromString(sHtml);
}
