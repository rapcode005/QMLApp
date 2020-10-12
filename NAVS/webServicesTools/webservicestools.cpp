#include "webservicestools.h"
#include <QDebug>
#include <QFile>
#include <io.h>
#include <QSettings>

WebServicesTools::WebServicesTools(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(html(QString)), this, SLOT(handleHtml(QString)));
    httpStatus.clear();
    httpStatus.append({200, "OK"});
    httpStatus.append({201, "The entity was successfully created"});
    httpStatus.append({204, "No Content, entry was deleted successully"});
    httpStatus.append({404, "Return for none existing entry"});
    httpStatus.append({403, "User has lack of permission"});
    httpStatus.append({401, "Use has invalid credentials"});
    httpStatus.append({422, "The data validation fails"});
    httpStatus.append({503, "Internal Server Error"});
}


QString WebServicesTools::searchErrorMessage(const int &code, QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::AuthenticationRequiredError)
        return "NAVS Session is no longer valid.";
    else if(code == QNetworkReply::UnknownNetworkError ||
            code == QNetworkReply::TemporaryNetworkFailureError ||
            code == QNetworkReply::NetworkSessionFailedError ||
            code == QNetworkReply::ProxyTimeoutError ||
            code == QNetworkReply::ProxyNotFoundError ||
            code == QNetworkReply::ProxyConnectionClosedError ||
            code == QNetworkReply::ProxyAuthenticationRequiredError) {
        return "Network connection issue.";
    }
    else {
        foreach(httpResult r, httpStatus) {
            if(r.code == code)
                return r.message;
        }
    }

    return QStringLiteral("");
}

void WebServicesTools::computeJsonValueGet(const QString &url, QString sessionKey)
{
    QEventLoop eventLoop;

    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    //Quit if Finished
    connect(this, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    connect(mgr, &QNetworkAccessManager::finished, &eventLoop, [=](QNetworkReply *reply){

        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
           status_code.isValid();{
               // send the status code
               const int &status = status_code.toInt();
               m_statusCode = status;
           }

        emit finished(reply);
    });

    QNetworkRequest req;
    req.setUrl(QUrl(url));

    if (!sessionKey.isEmpty()) {
        req.setRawHeader(QByteArray("NAVS-SESSION"), sessionKey.toUtf8());
    }

    QNetworkReply *reply = mgr->get(req);
    eventLoop.exec();

    //mgr->destroyed(this);

    if (reply->error() != QNetworkReply::NoError) {
        requestErrorOccurred(reply->error());
    }

    const QString &html = static_cast<QString>(reply->readAll());
    if(m_statusCode == 200) {
        m_jsonValue = ObjectFromString(html);
    }
    else if (m_statusCode == 503) {
        //Unknown issue
        const QString &error = searchErrorMessage(m_statusCode, reply);
        m_jsonValue = *new QJsonObject();
        createLogsInFile(url, error);
    }
    else {
        //For wrong validation something like that.
        m_jsonValue = ObjectFromString(html);
        const QString &error = searchErrorMessage(m_statusCode, reply);
        createLogsInFile(url, error);
    }

    reply->abort();
    //reply->destroyed(this);
}

void WebServicesTools::computeJsonValuePost(const QString &url, QString sessionKey)
{
    QEventLoop eventLoop;

    QUrlQuery postData;
    if (sessionKey != "") {
        postData.addQueryItem("NAVS-SESSION", sessionKey);
    }
    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    connect(this, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    connect(mgr, &QNetworkAccessManager::finished, &eventLoop, [=](QNetworkReply *reply){

        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
           status_code.isValid();{
               // send the status code
                const int &status = status_code.toInt();
                //qDebug() << status;
                m_statusCode = status;
           }

        emit finished(reply);
    });


    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (sessionKey != "") {
        req.setRawHeader(QByteArray("NAVS-SESSION"), sessionKey.toUtf8());
    }
    QNetworkReply *reply = mgr->post(req, postData.toString(QUrl::FullyEncoded).toUtf8());

    eventLoop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        requestErrorOccurred(reply->error());
    }

    const QString &html = static_cast<QString>(reply->readAll());
    if(m_statusCode == 200 || m_statusCode == 201) {
        m_jsonValue = ObjectFromString(html);
        //createLogsInFile(url);
    }
    else if (m_statusCode == 503) {
        //Unknown issue
        const QString &error = searchErrorMessage(m_statusCode, reply);
        m_jsonValue = *new QJsonObject();
        createLogsInFile(url, error);
    }
    else {
        //For wrong validation something like that.
        m_jsonValue = ObjectFromString(html);
        const QString &error = searchErrorMessage(m_statusCode, reply);
        createLogsInFile(url, error);
    }


    reply->abort();
    //reply->destroyed(this);
}

void WebServicesTools::computeJsonValuePost(const QString &url, const QJsonObject &jsonData)
{
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    //Quit if Finished
    connect(this, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    connect(mgr, &QNetworkAccessManager::finished, &eventLoop, [=](QNetworkReply *reply){

        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
           status_code.isValid();{
               // send the status code
                const int &status = status_code.toInt();
                m_statusCode = status;
               //qDebug() << status;
           }
        //If finished
        emit finished(reply);
    });


    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = mgr->post(req, QJsonDocument(jsonData).toJson());
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {

        QNetworkRequest req2;
        req2.setUrl(reply->url());
        QNetworkReply *reply2 = mgr->get(req2);
        eventLoop.exec();


        if (reply2->error() == QNetworkReply::NoError) {
            const QString &html = static_cast<QString>(reply2->readAll());
            m_jsonValue = ObjectFromString(html);
            //createLogsInFile(url);
            reply2->deleteLater();
        }else{
            requestErrorOccurred(reply2->error());
            const QString &error = searchErrorMessage(m_statusCode, reply);
            m_jsonValue = *new QJsonObject();
            createLogsInFile(url, error);
        }
    }else{
        requestErrorOccurred(reply->error());
        const QString &error = searchErrorMessage(m_statusCode, reply);
        m_jsonValue = *new QJsonObject();
        createLogsInFile(url, error);
    }

    //mgr->destroyed(this);
    reply->abort();
    //reply->destroyed(this);
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
    }else{
        requestErrorOccurred(reply->error());
        const QString &error = searchErrorMessage(m_statusCode, reply);
        createLogsInFile(url, error);
    }
    reply->abort();
    //mgr->destroyed(this);
}

void WebServicesTools::deleteJsonValue(const QString &url, QString sessionKey)
{
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    //Quit if Finished
    connect(this, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    connect(mgr, &QNetworkAccessManager::finished, &eventLoop, [=](QNetworkReply *reply){

        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
           status_code.isValid();{
               // send the status code
                const int &status = status_code.toInt();
                m_statusCode = status;
               //qDebug() << status;
           }

        emit finished(reply);
    });


    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (sessionKey != "") {
        req.setRawHeader(QByteArray("NAVS-SESSION"), sessionKey.toUtf8());
    }

    QNetworkReply *reply = mgr->deleteResource(req);
    eventLoop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        requestErrorOccurred(reply->error());
    }

    const QString &html = static_cast<QString>(reply->readAll());
    m_jsonValue = ObjectFromString(html);
    if(m_statusCode == 200 || m_statusCode == 201) {
        m_jsonValue = ObjectFromString(html);
        //createLogsInFile(url);
    }
    else {
        //If not successfully
        const QString &error = searchErrorMessage(m_statusCode, reply);
        m_jsonValue = *new QJsonObject();
        createLogsInFile(url, error);
    }

    reply->abort();
    //mgr->destroyed(this);
    //reply->destroyed(this);
}

bool WebServicesTools::downloadURL(const QString &url, const QString &sessionKey)
{
    QSaveFile file;
    bool alreadyCreate = false, errorCreateFile = false;
    const QString &dirPath = "C:\\Users\\Public\\Downloads";
    QNetworkAccessManager manager;
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    if (sessionKey != "") {
        req.setRawHeader(QByteArray("NAVS-SESSION"), sessionKey.toUtf8());
    }

    //QNetworkReply *response = manager.get(req);
    responseDownload = manager.get(req);
    QEventLoop event;

    connect(responseDownload, &QNetworkReply::downloadProgress, this, &WebServicesTools::downloadProgress);
    connect(responseDownload, SIGNAL(finished()), &event, SLOT(quit()));

    connect(responseDownload, &QNetworkReply::readyRead, [&](){
          if (!alreadyCreate) {

              QString end;
              //Creation of the file.
              if(responseDownload->rawHeader("Content-Disposition").isEmpty()
                      || responseDownload->rawHeader("Content-Disposition").isNull()) {
                  end = responseDownload->request().url().fileName();
              }
              else {
                  const QString &str = responseDownload->rawHeader("Content-Disposition");
                  end = str.mid(21);
              }

              setFileName(end);
              //Use filename as a name without extension file.
              std::size_t lastIndex = end.toStdString().find_last_of(".");
              std::string stdFileName = end.toStdString().substr(0, lastIndex);
              setFolderName(QString::fromUtf8(stdFileName.c_str()));
              createFolder(getFolderName());

              file.setFileName(dirPath + "\\" + getFolderName() + "\\" + getfileName());

              if(!file.open(QFile::WriteOnly)) {
                  createLogsInFile(url, "Unable to Download.");
                  errorCreateFile = true;
              }
              alreadyCreate = true;
          }
          else {
              QByteArray data= responseDownload->readAll();
              //qDebug() << "received data of size: " << data.size();
              file.write(data);
          }
    });

    connect(responseDownload, &QNetworkReply::finished, [&]{
        qDebug() << "finished downloading";
        QByteArray data= responseDownload->readAll();
        file.write(data);
        file.commit();
        responseDownload->deleteLater();
    });

    event.exec();

    if (errorCreateFile)
        return false;

    if (responseDownload->error() == QNetworkReply::NoError)
    {
        return true;
    }else{
        requestErrorOccurred(responseDownload->error());
        const QString &error = searchErrorMessage(m_statusCode, responseDownload);
        createLogsInFile(url, error);
    }
    return false;
}

bool WebServicesTools::downloadURL(const QString &url, const QString &sessionKey, const QString &destination)
{
    bool alreadyCreate = false, errorCreateFile = false;
    QNetworkAccessManager manager;
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    if (!sessionKey.isEmpty()) {
        req.setRawHeader(QByteArray("NAVS-SESSION"), sessionKey.toUtf8());
    }

    responseDownload = manager.get(req);
    //QNetworkReply *response = manager.get(req);
    QEventLoop event;
    QSaveFile file;
    qDebug() << "Start";

    connect(responseDownload, SIGNAL(finished()), &event, SLOT(quit()));
    connect(responseDownload, &QNetworkReply::downloadProgress, this, &WebServicesTools::downloadProgress);

    connect(responseDownload, &QNetworkReply::readyRead, [&](){
        //this will be called every time a chunk of data is received
        if (!alreadyCreate) {

            QString end;
            //Creation of the file.
            if(responseDownload->rawHeader("Content-Disposition").isEmpty()
                    || responseDownload->rawHeader("Content-Disposition").isNull()) {
                end = responseDownload->request().url().fileName();
            }
            else {
                const QString &str = responseDownload->rawHeader("Content-Disposition");
                end = str.mid(21);
            }

            setFileName(end);

            //QFile file(destination + "/" + getfileName());
            file.setFileName(destination + "/" + getfileName());

            if(!file.open(QFile::WriteOnly)) {
                createLogsInFile(url, "Unable to Download.");
                errorCreateFile = true;
            }
            alreadyCreate = true;
        }
        else {
            QByteArray data= responseDownload->readAll();
            //qDebug() << "received data of size: " << data.size();
            file.write(data);
        }

    });

    connect(responseDownload, &QNetworkReply::finished, [&]{
        qDebug() << "finished downloading";
        QByteArray data= responseDownload->readAll();
        file.write(data);
        file.commit();
        responseDownload->deleteLater();
    });

    event.exec();

    if (errorCreateFile)
        return false;

    if (responseDownload->error() == QNetworkReply::NoError)
    {
        return true;
    }else{
        requestErrorOccurred(responseDownload->error());
        const QString &error = searchErrorMessage(m_statusCode, responseDownload);
        createLogsInFile(url, error);
    }
    return false;
}

bool WebServicesTools::downloadURL2(const QString &url, const QString &destination, const QString &sessionKey)
{
    QNetworkAccessManager manager;
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    if (!sessionKey.isEmpty()) {
        req.setRawHeader(QByteArray("NAVS-SESSION"), sessionKey.toUtf8());
    }

    QNetworkReply *response = manager.get(req);
    if (response->error() == QNetworkReply::NoError)
    {
        QString end;
        //Creation of the file.
        if(response->rawHeader("Content-Disposition").isEmpty() || response->rawHeader("Content-Disposition").isNull()) {
            end = response->request().url().fileName();
        }
        else {
            const QString &str = response->rawHeader("Content-Disposition");
            end = str.mid(21);
        }

        qDebug() << end;

        setFileName(end);

        //return true;
    }else{
        requestErrorOccurred(response->error());
        const QString &error = searchErrorMessage(m_statusCode, response);
        createLogsInFile(url, error);
        return false;
    }

    QSaveFile file(destination + "/" + getfileName());

    qDebug() << "Start";

    connect(response, &QNetworkReply::readyRead, this, [&](){
        //this will be called every time a chunk of data is received
        QByteArray data= response->readAll();
        qDebug() << "received data of size: " << data.size();
        file.write(data);
    });
    //connect(response, &QNetworkReply::downloadProgress, this, &WebServicesTools::downloadProgress);
    connect(response, &QNetworkReply::finished, this, [&](){
        qDebug() << "finished downloading";
        //QByteArray data= response->readAll();

        if(!file.open(QFile::WriteOnly)) {
            createLogsInFile(url, "Unable to Download.");
            return false;
        }
        else {
            file.write(response->readAll());
            file.commit();
        }
        response->deleteLater();
        return true;
    });

    //return false;
}

void WebServicesTools::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
   emit sendDownloadProgress(bytesReceived, bytesTotal);
}

QString WebServicesTools::getApiUrlNavs() const
{
    QSettings settings;
    settings.beginGroup("Servers");
    const QVariant value = settings.value("beta1_url", "https://api.navsnow.com/");
    settings.endGroup();
    const QString res = value.toString();
    return res;
}

QString WebServicesTools::getBetaUrl() const
{
    return getApiUrlNavs();
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

QString WebServicesTools::getfileName() const
{
    return m_fileName;
}

void WebServicesTools::setFileName(const QString &name)
{
    m_fileName = name;
}

QString WebServicesTools::getFolderName() const
{
    return m_folderName;
}

void WebServicesTools::setFolderName(const QString &name)
{
    m_folderName = name;
}

QString WebServicesTools::adjustAppName(const QString &curApp) const
{
    const QString &str = curApp;
    const int &len = str.length();
    QString newStr = curApp;
    if (str.indexOf(" ") != -1) {
        //get all words put them in array
        const QStringList &words = str.split(" ");
        QString temp = "", newTempStr = "";
        foreach(const QString &word, words) {
            temp = temp + word;
            if (temp.length() + 1 > 12) {
                newTempStr = newTempStr + "\n" + word;
                temp = word;
            }
            else {
                newTempStr = newTempStr + " " + word;
                temp = newTempStr;
            }
        }
        newStr = newTempStr.trimmed();
    }
    else {
        //If more than 12 letters.
        const int &count = len / 12;
        int c = 0;
        QString tempStr = "";
        for(int i = 0; i < count; i++) {
            if (i == 0)
                tempStr = str.mid(c, 12);
            else
                tempStr = tempStr + "\n" + str.mid(c, 12);

            c += 12;
        }
        newStr = tempStr.trimmed();
    }
    return newStr;
}

int WebServicesTools::getOrganizationID() const
{
    QString inst = decryptInstallation();
    return inst.length() < 1? 0: inst.split(" ")[1].toInt();
}

int WebServicesTools::getLocationID() const
{
    QString inst = decryptInstallation();
    return inst.length() < 1? 0: inst.split(" ")[2].toInt();
}

QString WebServicesTools::decryptInstallation() const
{
    QFile file(QStringLiteral("C:\\Users\\Public\\Documents\\n@v$12"));
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        return "";
    }
    QByteArray encodeText = file.readAll();
    return  QString::fromStdString(encodeText.toStdString());
}

void WebServicesTools::createLogsInFile(const QString &url, QString errorMessage)
{
    QFile *file = new QFile();
    file->setFileName("Logs");
    if(file->open(QFile::ReadWrite | QFile::Text)) {

        QByteArray current = file->readAll();

        QTextStream logs(file);
        QStringList data;
        QDate cd = QDate::currentDate();
        QTime ct = QTime::currentTime();
        data << cd.toString() << ct.toString() << getWindowsUsername()
             << "Organization ID: " + QString::number(getOrganizationID())
             << "Location ID: " + QString::number(getLocationID())
             << "(Web Request Error)"
             << url;
        if (!errorMessage.isEmpty())
            data << errorMessage;

        //logs << current;

        foreach(QString str, data) {
            logs.setPadChar(' ');

            logs.setFieldWidth(qMax(2, str.length()));

            logs << str;

            logs.setFieldWidth(1);

            logs << " ";
        }

        logs << "\n";

        file->flush();
        file->close();
    }
}

void WebServicesTools::createColumn(const QString &str, QFile *file)
{
    Q_UNUSED(str)
    QTextStream ts(file);
}

QJsonObject WebServicesTools::ObjectFromString(const QString &in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    obj = doc.object();

    return obj;
}

void WebServicesTools::createFolder(const QString &name)
{
    const QString &dirPath = "C:\\Users\\Public\\Downloads";
    QDir dir(dirPath);
    qDebug() << "Current path: " + dir.path();
    if (dir.mkdir(name))
        qDebug() << "sucesss: " + dirPath + "\\" + name;
    else
        qDebug() << "failed: " + name;
}

void WebServicesTools::requestErrorOccurred(QNetworkReply::NetworkError code)
{
    //Q_UNUSED(code)

    //qDebug() << "Send network error: " << code;

    emit networkError(code);
}

QJsonObject WebServicesTools::getJsonValue()
{
    return m_jsonValue;
}

void WebServicesTools::handleHtml(QString sHtml)
{
    m_jsonValue = ObjectFromString(sHtml);
}
