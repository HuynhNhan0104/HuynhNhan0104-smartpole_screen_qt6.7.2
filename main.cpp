#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "mqttHandler.h"
#include "httpHandler.h"
#include "dashboardController.h"
#include "videoController.h"
#include <QLoggingCategory>
#include<QtMultimedia/qmediaplayer.h>
#include<QtMultimedia/qmediadevices.h>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QStringList args = app.arguments();
    // argument containt
    // [0] name of file
    // [1] mqtt broker
    // [2] mqtt port
    // [3] mqtt username
    // [4] token mqtt
    // [5] topic to subcribe link
    // [6] serial bitrate
    // [7] serial mode(test or run)
    // [8] api error request
    int id; //= 0;
    QString broker; // = "io.adafruit.com";
    int port;// 1883;
    QString user;// = "NhanHuynh";
    QString token; // = "";
    QString subTopic; //= "NhanHuynh/feeds/link";
    QString pubTopic; //= "NhanHuynh/feeds/link";
    int bitrate;// = 9600;
    int mode;// = 0;
    QString api;// = "https://io.adafruit.com/api/v2/NhanHuynh/feeds/mask";


    if (args.count() < 2)
    {
        std::cerr << "argument required" << endl;
        // return 1;
    } else {
        QFile file(args[1]);
        // QFile file("../../config.json");
        // qDebug() << "Current working directory:" << QDir::currentPath();
        qDebug() << "file config: " << args[1];
        if (file.open(QIODevice::ReadOnly)) {
            // file.open(QIODevice::ReadOnly);
            QByteArray data = file.readAll();
            file.close();

            // Phân tích dữ liệu JSON
            QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
            QJsonObject jsonObj = jsonDoc.object();

            id      =   jsonObj["id"].toInt();
            broker  =   jsonObj["broker"].toString();
            port    =   jsonObj["port"].toInt();
            user    =   jsonObj["user"].toString();
            token   =   jsonObj["token"].toString();
            subTopic   =   jsonObj["subTopic"].toString();
            pubTopic   =   jsonObj["pubTopic"].toString();
            bitrate =   jsonObj["bitrate"].toInt();
            mode    =   jsonObj["mode"].toInt();
            api     =   jsonObj["api"].toString();
            // qDebug() << "id: " << id;
            // qDebug() << "broker:"  << broker;
            // qDebug() << "user: "  << user;
            // qDebug() << "port: " << port;
            // qDebug() << "token: "<< token;
            // qDebug() << "subTopic: " << subTopic;
        } else {
            qDebug() << "Couldn't open file, using default config";
        }
    }



    QQmlApplicationEngine engine;
    // fist solution: load c++ object to qml file and use, you need add marcro QML_ELEMEMT on constructor of your object
    // qmlRegisterType<DashboardController>("DashboardController", 1, 0, "DashboardController");
    // second solution: create c++ object and offer its context for the qml engine(it manager all qml), next just use c++ object on qml file


    // create mqtt handler
    MqttHandler* mqttHandler  =  new MqttHandler(&app,broker,port,user,token);
    mqttHandler->addTopic(subTopic);
    mqttHandler->setPublishTopic(pubTopic);



    // supply  DashboardController object context to QML context
    // run in "TEST" OR "RUN" Mode
    DashboardController* dashboardController =  new DashboardController(&app,bitrate,mode ? (DashboardController::RUN):(DashboardController::TEST));
    engine.rootContext()->setContextProperty("DashboardController", dashboardController);

    // supply VideoController  object context to QML context
    // default link when starting is : "https://live4.thvli.vn/Ao-O3eV678ehY8Riwr6BTg/1721556483/thvli/thvl1-abr/thvl111220/thvl1-480p/chunks.m3u8"
    VideoController* videoController =  new VideoController(&app);
    //add topic want to subcribe here
    // videoController->addTopic("NhanHuynh/feeds/link");
    engine.rootContext()->setContextProperty("VideoController", videoController);

    // create http handler
    // supply  HttpHandler object context to QML context
    HttpHandler* httpHandler = new HttpHandler(&app,api);
    engine.rootContext()->setContextProperty("HttpHandler", httpHandler);

    // create connection between mqtt handler and videoController to revcieve message and update link
    QObject::connect(mqttHandler,&MqttHandler::recieveFromLinkTopic, videoController, &VideoController::onReceiveLinkFromMqtt);
    // create connection between mqtt handler and dashboardController to publist message of sensor to topic
    QObject::connect(dashboardController,&DashboardController::publishDataToTopic, mqttHandler, &MqttHandler::publishSensorData);

    // create connection between http handler and videoController to publist message of sensor to topic
    QObject::connect(httpHandler,&HttpHandler::receiveLinkFromRequest, videoController, &VideoController::onReceiveLinkFromHttp);


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
