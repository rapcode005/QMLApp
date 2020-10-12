#include "loggingtools.h"

LoggingTools::LoggingTools(QObject *parent) : QObject(parent)
{

}

void LoggingTools::createLogs(const QString &formatName,
                              const QString &logSource,
                              const QString &severity,
                              const QString &errorMessage,
                              const int &organizationID,
                              const int &navsID)
{
    QJsonObject jsonLogFormat = getLogFormat(formatName);

    QString url = webServiceTools.getApiUrlNavs() + "api/logs?"
                "log_source_id=" + QString::number(getLogSourceID(logSource))
                + "&format_name=" + formatName + "&severity=" + severity;

    jsonLogFormat.insert("formatName", QJsonValue::fromVariant(formatName));
    QJsonObject jsonLog =  jsonLogFormat["logData"].toObject();
    QJsonObject jsonDate = jsonLog["date"].toObject();
    QDateTime curr = QDateTime::currentDateTimeUtc();

    jsonDate["date"] = curr.toString("yyyy-MM-dd HH:mm:ss");
    jsonDate["timezone"] = "GMT";
    jsonDate["timezone_type"] = 3;
    jsonLog.remove("date");
    jsonLog.insert("date", jsonDate);

    jsonLog["organizationID"] = organizationID;
    //jsonLog["organizationUserID"] = getOrganizationUserID(organizationID, userTypeID); //Doesn't work correctly
    jsonLog["windowsUsername"] = webServiceTools.getWindowsUsername();
    jsonLog["NavsInstallationID"] = navsID;
    jsonLog["errorMessage"] = errorMessage;
    jsonLogFormat.remove("logData");
    jsonLogFormat.insert("logData", jsonLog);

    webServiceTools.computeJsonValuePost(url, jsonLogFormat);

    QJsonObject jsonResult = webServiceTools.getJsonValue();

    //Does the web service respond with anything?  If so, we need to check that it's valid.----------------  Otherwise do not emit logLoaded. Instead emit an error.

    emit logLoaded(jsonResult);
}

QJsonObject LoggingTools::getLogFormat(const QString &name)
{
    QJsonDocument doc;
    QString url = webServiceTools.getApiUrlNavs() + "api/log-formats?"
                "search_text=" + name;

    webServiceTools.computeJsonValueGet(url);

    QJsonObject jsonResult = webServiceTools.getJsonValue();

    if(jsonResult.length() < 1){
        //Emit an error message the the qml front ens can pick up.-----------------------------------------------

        return jsonResult;
    }

    QJsonArray jsonArray = jsonResult["data"].toArray();

    foreach (const QJsonValue &logformat, jsonArray) {
        doc = QJsonDocument::fromJson(logformat.toObject().
                                                    find("json_base")->toString().toUtf8());
    }

    //QJsonDocument doc = QJsonDocument::fromJson(jsonResult["JSON Base"].toString().toUtf8());


    return doc.object();
}

int LoggingTools::getLogSourceID(const QString &name)
{
    QString url = webServiceTools.getApiUrlNavs() + "api/log-sources/?"
            "search_text=" + name;

    webServiceTools.computeJsonValueGet(url);

    QJsonObject jsonResult = webServiceTools.getJsonValue();

    if(jsonResult.length() < 1){
        //Emit an error message the the qml front ens can pick up.-----------------------------------------------

        return 1;
    }

    QJsonArray jsonArray = jsonResult["data"].toArray();

    foreach (const QJsonValue &logSource, jsonArray) {

        if(logSource.toObject().find("id")->toInt() == 0)
            return 1;
        else
            return logSource.toObject().find("id")->toInt();
    }

    return 1;
}

void LoggingTools::createLogsInFile(const QString &url,
                                    QString errorMessage)
{
    QTextStream logs("logs");
    QStringList data;
    QDate cd = QDate::currentDate();
    QTime ct = QTime::currentTime();
    data << cd.toString() << ct.toString() << webServiceTools.getWindowsUsername()
         << "Organization ID: " + QString::number(installationTools.getOrganizationID())
         << "Location ID: " + QString::number(installationTools.getLocationID())
         << url;
    if (!errorMessage.isEmpty())
        data << errorMessage;

    foreach(QString str, data)
        createColumn(str);

    logs << "\n";
}

void LoggingTools::createColumn(const QString &str)
{
    QTextStream ts("logs");

    ts.setPadChar(' ');

    ts.setFieldWidth(qMax(2, str.length()));

    ts << str;

    ts.setFieldWidth(1);

    ts << " ";
}

//int LoggingTools::getOrganizationUserID(const int &organization, //-------------------------------------Not done correctly. The Me data already has this.
//                                        const int &userType)
//{
//    QString url = webServiceTools.getApiUrlNavs() + "api/organization-users?limit=50&"
//                                                    "organization_id=" + QString::number(organization) +
//                                                    "&user_type_id=" + QString::number(userType);

//    webServiceTools.computeJsonValueGet(url);

//    QJsonObject jsonResult = webServiceTools.getJsonValue();

//    QJsonArray jsonArray = jsonResult["data"].toArray();

//    foreach (const QJsonValue &value, jsonArray) {
//        if (value.toObject().find("id")->toInt() == 0)
//            return 1;
//        else
//            return value.toObject().find("id")->toInt();
//    }

//    return 1;
//}
