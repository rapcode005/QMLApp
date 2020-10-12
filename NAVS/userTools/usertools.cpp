#include "usertools.h"
#include "organizationinfo.h"
#include <QJsonObject>


UserTools::UserTools(QObject *parent) : QObject(parent)
{
    connect(&webServiceTools, &WebServicesTools::networkError, this,
            &UserTools::requestErrorOccurred);
}

void UserTools::requestErrorOccurred(QNetworkReply::NetworkError code)
{
    //qDebug() << "User Tools Caught Network error. Sending on.";

    emit networkError(code);
}

int UserTools::getLoginTypeID(const QJsonObject &user) const
{
    if(!user["user"].isUndefined() &&
            !user["user"]["organizationUser"].isUndefined() &&
            !user["user"]["organizationUser"]["user_type_id"].isUndefined() &&
            !user["user"]["organizationUser"]["user_type_id"].isNull()){
        return user["user"]["organizationUser"]["user_type_id"].toInt();
    }else{
        return 1; //For CU Northwest Admin
    }
}

void UserTools::loadMeData(const QString &sessionKey)
{
    const char error[22] = "meData was not loaded";

    try {

        const QString &url = webServiceTools.getApiUrlNavs() + "api/me";
        webServiceTools.computeJsonValueGet(url, sessionKey);
        const QJsonObject &jsonResult = webServiceTools.getJsonValue();

        if(jsonResult.length() > 0){
            emit meDataLoaded(jsonResult);
            //Sorry Jon I don't know the format Name, log source or etc.. value;
            //loggingTools->createLogs();

        }else{
            emit loadMeDataErrorLoaded(error);
        }

    } catch (...) {
      //  QJsonObject jsonResult;
        emit loadMeDataErrorLoaded(error);
    }
}

void UserTools::loadOrganizationByID(const int organizationID, const QString &sessionKey)
{
    const QString &url = webServiceTools.getApiUrlNavs() + "api/organizations/" + QVariant(organizationID).toString();
    webServiceTools.computeJsonValueGet(url, sessionKey);
    const QJsonObject &jsonResult = webServiceTools.getJsonValue();

    if(jsonResult.length() > 0){
        checkOrganization(jsonResult["dns_key"].toString().replace(".navsnow.com",""));
        //Sorry Jon I don't know the format Name, log source or etc.. value;
        //loggingTools->createLogs();

    }else{
        emit organizationByIDErrorLoaded("Check Organization feiled");
    }
}

void UserTools::checkOrganization(const QString &s_organizationDnsKey)
{
    try {

        const QString &orgNameDnsKey = s_organizationDnsKey.isEmpty() ? "s" : s_organizationDnsKey;

        bool organizationDnsKeyStatus = false;
        int count = 0;
        const QString &url = webServiceTools.getApiUrlNavs() + "api/organization-dns?dns_key=" +
               orgNameDnsKey + ".navsnow.com";

        qDebug() << url;

        webServiceTools.computeJsonValueGet(url);

       jsonOrganization = webServiceTools.getJsonValue();

       //Logging------------------------------------------
   //    QJsonDocument doc(jsonOrganization);
   //    QString strJson(doc.toJson(QJsonDocument::Compact));
   //    emit writeLogItem(strJson);
       //---------------------------------------------------

       //Check if Empty
       if (jsonOrganization.size() > 0){

            //Load Organization Data
            if (jsonOrganization.contains("name") == true && jsonOrganization.contains("id") == true) {
               const QString &organizationName = jsonOrganization["name"].toString();
               const int &organizationID = jsonOrganization["id"].toInt();
               const QString &organizationDnsKey = jsonOrganization["dns_key"].toString();
               const QJsonArray &organizationArray = jsonOrganization["organization_locations"].toArray();
               const QString &orgLogo = jsonOrganization["logo"].toString();
               const QString &orgIcon = jsonOrganization["icon"].toString();


               OrganizationModel *item = new OrganizationModel();

               foreach(const QJsonValue &value, organizationArray) {

                   //qDebug()<< "org name = " << organizationName;

                   item->append(new OrganizationInfo(organizationName,
                                                   value.toObject().find("name")->toString(),
                                                   organizationArray.size(),
                                                   value.toObject().find("id")->toInt(),
                                                   organizationID,
                                                   organizationDnsKey,
                                                   orgLogo,
                                                   orgIcon,
                                                   value.toObject()
                                                   ));


                   count += 1;
               }

               item->setCount(count);

               if(count == 0){
                    emit organizationDnsKeyStatusLoaded(false);
                    qDebug() << "Organization is not valid. It has no locations.";
               }else{
                   organizationDnsKeyStatus = true;
                   emit organizationDataLoaded(item);
               }

               //item->destroyed(this);

               //Sorry Jon I don't know the format Name, log source or etc.. value;
               //loggingTools->createLogs();
           }
       }
       else {

           emit organizationDNSKeyErrorLoaded("organizationDataLoaded Failed. API error.");

       }

       emit organizationDnsKeyStatusLoaded(organizationDnsKeyStatus);

    } catch (...) {
        emit organizationDnsKeyStatusLoaded(false);
        emit organizationDNSKeyErrorLoaded("organizationDataLoaded Failed.");
    }
}


void UserTools::loginWindowsUsername()
{
    const QString &installationKey = installationTools.getInstallationKeyFile();
    const QString &ipAddress = installationTools.getIPAddress();

    const QString &url = webServiceTools.getApiUrlNavs() + "api/users/login?"
            "login_type_id=1&key=" + installationKey +
            "&ip=" + ipAddress + "&windows_username=" + webServiceTools.getWindowsUsername();

    webServiceTools.computeJsonValuePost(url);

    const QJsonObject &jsonResult = webServiceTools.getJsonValue();

    if(jsonResult.length() > 0){
        emit loginWindowsUsernameLoaded(jsonResult);
        //Sorry Jon I don't know the format Name, log source or etc.. value;
        //loggingTools->createLogs();
    }else{
        emit loginWindowsUsernameErrorLoaded("Operation Failed.");
    }
}


