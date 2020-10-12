QT += qml quick
#CONFIG+=no_keywords
CONFIG+=c++11
#CONFIG += console

QT += gui
QT += core
QT += network
QT += webengine
QT += webchannel


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

RC_ICONS = Images/NAVS_N.ico

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        appTools/apptools.cpp \
        appTools/charlietester.cpp \
        appfilterproxy.cpp \
        appinfo.cpp \
        appmodel.cpp \
        backend.cpp \
        coreappnavs.cpp \
        errormessage.cpp \
        installationTools/installationtools.cpp \
        loggingTools/loggingtools.cpp \
        main.cpp \
        messageinfo.cpp \
        messagemodel.cpp \
        navsobject.cpp \
        notificationinfo.cpp \
        notificationsmodel.cpp \
        organizationinfo.cpp \
        organizationmodel.cpp \
        socket/internal/sio_client_impl.cpp \
        socket/internal/sio_packet.cpp \
        socket/sio_client.cpp \
        socket/sio_socket.cpp \
        urlrequest.cpp \
        userTools/usertools.cpp \
        webServicesTools/webservicestools.cpp \
        navseventfilter.cpp \
        workerthread.cpp

RESOURCES += qml.qrc \
    contet.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
0
include(./fontawesome.pri)


#Raf's Config
#INCLUDEPATH += C:\socket.io-client-cpp\lib\websocketpp
#INCLUDEPATH += C:\socket.io-client-cpp\lib\rapidjson\include
#INCLUDEPATH += C:\boostv1_67\include\boost-1_67
#LIBS += -LC:\socket.io-client-cpp\build\lib\Release -lsioclient
#LIBS += -LC:\socket.io-client-cpp\build\lib\Release -lsioclient_tls
#LIBS += -LC:\boostv1_67\lib -llibboost_date_time-vc141-mt-x64-1_67
#LIBS += -LC:\boostv1_67\lib -llibboost_random-vc141-mt-x64-1_67
#LIBS += -LC:\boostv1_67\lib -llibboost_system-vc141-mt-x64-1_67

#Rap's OpenSSL Library
#INCLUDEPATH += $$quote("C:\OpenSSL-Win64\include")
#LIBS += -L$$quote("C:\OpenSSL-Win64\lib") -lopenssl
#LIBS += -L$$quote("C:\OpenSSL-Win64\lib") -llibcrypto
#LIBS += -L$$quote("C:\OpenSSL-Win64\lib") -llibssl

#Jon's config
INCLUDEPATH += C:\NAVS\websocketpp
INCLUDEPATH += C:\NAVS\rapidjson\include
INCLUDEPATH += C:\NAVS\boost-1_67
LIBS += -LC:\NAVS -lsioclient
LIBS += -LC:\NAVS -lsioclient_tls
LIBS += -LC:\NAVS -llibboost_date_time-vc141-mt-x64-1_67
LIBS += -LC:\NAVS -llibboost_random-vc141-mt-x64-1_67
LIBS += -LC:\NAVS -llibboost_system-vc141-mt-x64-1_67

INCLUDEPATH += $$quote("C:\openssl\include")
LIBS += -L$$quote("C:\openssl\lib") -lopenssl
LIBS += -L$$quote("C:\openssl\lib") -llibcrypto
LIBS += -L$$quote("C:\openssl\lib") -llibssl

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    appTools/apptools.h \
    appTools/charlietester.h \
    appfilterproxy.h \
    appinfo.h \
    appmodel.h \
    backend.h \
    coreappnavs.h \
    errormessage.h \
    installationTools/installationtools.h \
    loggingTools/loggingtools.h \
    messageinfo.h \
    messagemodel.h \
    navsObject.h \
    notificationinfo.h \
    notificationsmodel.h \
    organizationinfo.h \
    organizationmodel.h \
    socket/internal/sio_client_impl.h \
    socket/internal/sio_packet.h \
    socket/sio_client.h \
    socket/sio_message.h \
    socket/sio_socket.h \
    urlrequest.h \
    userTools/usertools.h \
    webServicesTools/webservicestools.h \
    navseventfilter.h \
    workerThread.h

FORMS +=

DISTFILES +=
