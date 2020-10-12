#include "usertools.h"
#include "organizationinfo.h"
#include <QJsonObject>


UserTools::UserTools(QObject *parent) : QObject(parent)
{
}

void UserTools::loadUser(){
    //Call the userTools functions to get the user data.
    QJsonObject tempUserObject
    {
        {"property1", 1},
        {"property2", 2}
    };
    emit userDataLoaded(tempUserObject);
}

int UserTools::getLoginTypeID()
{
    QString url = webServiceTools.getApiUrlNavs() + "api/releases/1";
    QJsonObject jsonLoginTypeID;

    webServiceTools.computeJsonValueGet(url);

    jsonLoginTypeID = webServiceTools.getJsonValue();

    if(jsonLoginTypeID.size() > 0)
    {
        return jsonLoginTypeID["id"].toInt();
    }

    return 1;
}

/*
QObject *UserTools::fromJson(cohttp://ec2-44-225-216-138.us-west-2.compute.amazonaws.com/api/login-types/{id}nst QMetaObject &meta, QString &json)
{
    auto jsonObject = QJsonDocument::fromJson(json.toLatin1()).object();
    return fromJson(&meta, jsonObject);
}

QObject *UserTools::fromJson(const QMetaObject *meta, QJsonObject &jsonObject)
{
   QObject *object = meta->newInstance();

   int propertyStart = QObject::staticMetaObject.propertyCount();
   for (int i = propertyStart; i < meta->propertyCount(); ++i) {
       QMetaProperty property = meta->property(i);
       if (!property.isWritable())
           continue;
       if (!jsonObject.contains(property.name()))
           continue;

       auto value = jsonValueToProperty(object, property, jsonObject.value(property.name()));
       property.write(object, value);
   }

   return object;
}

QVariant UserTools::jsonValueToProperty(QObject *object, QMetaProperty &property, QJsonValue value)
{
    auto type = property.userType();
    auto typeName = QString(property.typeName());
    if (value.isArray()) {
        //todo array
        return value;
    } else if (value.isObject()) {
        switch (type) {
        default:
            auto jsonObject = value.toObject();
            return QVariant::fromValue(fromJson(QMetaType::metaObjectForType(type), jsonObject));
        }
        //todo other meta object types
    } else {
        //primitive types
        return value.toVariant();
    }
}
*/

void UserTools::loadOrganization(QString s_organization)
{
    QString organizationName = "Empty", organizationDnsKey = "Empty";
    int organizationID = -1;

    if (s_organization.isEmpty())
        s_organization = "s";

    /*
    QString url = webServiceTools.getApiUrlNavs() + "api/organizations?"
                  "limit=50&search_text=" + s_organization;
    */
    QString url = webServiceTools.getApiUrlNavs() + "api/organization-dns?dns_key=" +
            s_organization;

    webServiceTools.computeJsonValueGet(url);

    jsonOrganization = webServiceTools.getJsonValue();

    if (!jsonOrganization["name"].isNull() && !jsonOrganization["id"].isNull()) {
        organizationName = jsonOrganization["name"].toString();
        organizationID = jsonOrganization["id"].toInt();
        organizationDnsKey = jsonOrganization["dns_key"].toString();
    }

    QJsonArray organizationArray = jsonOrganization["organization_locations"].toArray();
    int count = 0;
    if (organizationArray.size() > 0) {
        foreach(const QJsonValue &value, organizationArray) {
            item = new OrganizationModel();
            item->append(new OrganizationInfo(organizationName,
                                            value.toObject().find("name")->toString(),
                                            organizationArray.size(),
                                            value.toObject().find("id")->toInt(),
                                            organizationID,
                                            organizationDnsKey));
            count += 1;
        }
    }
    else {
        item = new OrganizationModel();
        item->append(new OrganizationInfo(organizationName, "Empty", -1, -1, organizationID,
                                            organizationDnsKey));
        count += 1;
    }
    item->setCount(count);
    /*
    QJsonArray organizationArray = jsonOrganization["organization_locations"].toArray();

    //Check if Empty
    if (organizationArray.size() > 0) {

        foreach (const QJsonValue &nameValue, organizationArray) {
            organizationName = nameValue.toObject().find("name")->toString();
            organizationID = nameValue.toObject().find("id")->toInt();
        }

        //Get Location info
        url =  webServiceTools.getApiUrlNavs() + "api/organization-locations?limit=50"
                                                  "&organization_id=" + QString::number(organizationID);

        webServiceTools.computeJsonValueGet(url);

        QJsonObject json = webServiceTools.getJsonValue();

        QJsonArray locationArray = json["data"].toArray();

        if (locationArray.size() > 0) {

            foreach (const QJsonValue &locationValue, locationArray) {
                item = new OrganizationModel();
                item->append(new OrganizationInfo(organizationName,
                                                  locationValue.toObject().find("name")->toString(),
                                                  locationArray.size(),
                                                  locationValue.toObject().find("id")->toInt(),
                                                  organizationID));
            }

        }
        else {
            item = new OrganizationModel();
            item->append(new OrganizationInfo(organizationName, "Empty", -1, -1, organizationID));
        }

    }
    else {
        item = new OrganizationModel();
        item->append(new OrganizationInfo("Empty", "Empty", -1, -1, -1));
    }
    */
    emit organizationDataLoaded(item);
}

void UserTools::loginWindowsUsername()
{
    QJsonObject jsonResult;

    int loginTypeID = getLoginTypeID();
    QString installationKey = installationTools.getInstallationKeyFile();
    //QString installationKey = "60677b2e-4acf-3051-a90c-8406f6bb75fe";
    QString ipAddress = installationTools.getIPAddress();

    QString url = webServiceTools.getApiUrlNavs() + "api/users/login?"
            "login_type_id=" + QString::number(loginTypeID) + "&key=" + installationKey +
            "&ip=" + ipAddress + "&windows_username=" + webServiceTools.getWindowsUsername();

    webServiceTools.computeJsonValuePost(url);

    jsonResult = webServiceTools.getJsonValue();

    emit loginWindowsUsernameLoaded(jsonResult);
}
/*
void UserTools::logoutOrganization(const QString &username, const QString &password)
{
    QJsonObject jsonResult;

    int loginTypeID = getLoginTypeID();
    QString installationKey = installationTools.getInstallationKeyFile();
    //QString installationKey = "60677b2e-4acf-3051-a90c-8406f6bb75fe";
    QString ipAddress = installationTools.getIPAddress();

    QString url = webServiceTools.getApiUrlNavs() + "api/users/logout?"
            "login_type_id=" + QString::number(loginTypeID) + "&key=" + installationKey +
            "&ip=" + ipAddress + "&username=" + username + "&password=" + password;

    webServiceTools.computeJsonValuePost(url);

    jsonResult = webServiceTools.getJsonValue();

    emit logoutOrganizationLoaded(jsonResult);
}
*/
void UserTools::checkOrganization(QString s_organizationDnsKey)
{
     if(s_organizationDnsKey.isEmpty())
        s_organizationDnsKey = "s";

    bool organizationDnsKeyStatus;

    /*
    QString url = webServiceTools.getApiUrlNavs() + "api/organizations?"
                  "limit=50&name=" + s_organizationName;
    */
    QString url = webServiceTools.getApiUrlNavs() + "api/organization-dns?dns_key=" +
            s_organizationDnsKey;


    WebServicesTools *webService = new WebServicesTools();

    webService->computeJsonValueGet(url);

    jsonOrganization = webService->getJsonValue();

    //QJsonArray organizationArray = jsonOrganization["data"].toArray();

    //Check if Empty
    if (jsonOrganization.size() > 0)
    {
        organizationDnsKeyStatus = true;
    }
    else
    {
        organizationDnsKeyStatus = false;
    }

    emit organizationDnsKeyStatusLoaded(organizationDnsKeyStatus);
}

