#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "mqttHandler.h"
#include "dashboardController.h"
#include "videoController.h"
#include <QLoggingCategory>
#include<QtMultimedia/qmediaplayer.h>
#include<QtMultimedia/qmediadevices.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    // fist solution: load c++ object to qml file and use, you need add marcro QML_ELEMEMT on constructor of your object
    // qmlRegisterType<DashboardController>("DashboardController", 1, 0, "DashboardController");
    // second solution: create c++ object and offer its context for the qml engine(it manager all qml), next just use c++ object on qml file


    // create mqtt handler
    MqttHandler* mqttHandler  =  new MqttHandler(&app,"io.adafruit.com",1883,"NhanHuynh", "");


    mqttHandler->addTopic("NhanHuynh/feeds/link");


    // supply  DashboardController object context to QML context
    // run in "TEST" OR "RUN" Mode
    DashboardController* dashboardController =  new DashboardController(&app,"/dev/ttyUSB0",9600,DashboardController::TEST);
    engine.rootContext()->setContextProperty("DashboardController", dashboardController);

    // supply VideoController  object context to QML context
    // default link when starting is : "https://live4.thvli.vn/Ao-O3eV678ehY8Riwr6BTg/1721556483/thvli/thvl1-abr/thvl111220/thvl1-480p/chunks.m3u8"
    VideoController* videoController =  new VideoController(&app);
    //add topic want to subcribe here
    // videoController->addTopic("NhanHuynh/feeds/link");
    engine.rootContext()->setContextProperty("VideoController", videoController);

    // create connection between mqtt handler and videoController to revcieve message and update link
    QObject::connect(mqttHandler,&MqttHandler::recieveFromLinkTopic, videoController, &VideoController::onRecieveLinnk);
    // create connection between mqtt handler and dashboardController to publist message of sensor to topic
    QObject::connect(dashboardController,&DashboardController::publishDataToTopic, mqttHandler, &MqttHandler::publishSensorData);



    const QUrl url(QStringLiteral("qrc:/smartpole_screen/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);



    return app.exec();




}
