#include "mqttHandler.h"



mqttHandler::mqttHandler(QObject *parent, const char *hostName, uint16_t port, const char *userName, const char *password):
    QObject( parent ),
    client( new QMqttClient(this))
{
    client->setHostname(QString(hostName));
    client->setPort(quint16(port));
    client->setUsername(QString(userName));
    client->setPassword(QString(password));
    connect(client,&QMqttClient::connected,this,&mqttHandler::onConnected);
    connect(client,&QMqttClient::messageReceived,this,&mqttHandler::onMessageRecieved);
    connect(client,&QMqttClient::errorChanged,this, [](QMqttClient::ClientError error) {
        qDebug()<<"[Error] "<< error << "\n";}
    );
    client->connectToHost();
}

mqttHandler::~mqttHandler()
{
    qDebug()<< "MQTT Deleted \n";
    delete client;
    clearSubscriptionsManager();
}

void mqttHandler::onConnected()
{
    subscribeAllTopic();
}

void mqttHandler::onMessageRecieved(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug()<<"[Recieve]:[from: "<< topic <<"]:[Data: "<< message <<"] \n";
    //json format {"ID": [1, 2, 3], "link":"file:///home/nhan/workspace/img/wave.mp4"}
    // or {"ID": [1, 2, 3], "link":"file:///home/nhan/workspace/img/sea.mp4"}

    QJsonDocument messageJsonDoc = QJsonDocument::fromJson(message);
    if (messageJsonDoc.isObject()) {
        // convert document to Oject
        QJsonObject messageJsonObj = messageJsonDoc.object();

        // Get json value from key
        QJsonValue recievedIDList = messageJsonObj["ID"];
        QJsonValue recievedLink = messageJsonObj["link"];

        // Convert type from json to appropriated value
        QJsonArray IDList = recievedIDList.toArray();
        QString qlink = recievedLink.toString();

        // parse m3u8 file
        const std::string stdLink = qlink.toStdString();
        // QString currentQuality = getQuality();
        // QString newM3u8Link = parseM3u8Url(stdLink, currentQuality.toStdString());
        // setLink(newM3u8Link);
    }
    else{
        qWarning() << "[Warning] data recieved is not json \n" ;
    }
}

void mqttHandler::subscribeAllTopic()
{
    foreach(const QMqttTopicFilter &topic, topics){
        QMqttTopicFilter newTopic(topic);
        QMqttSubscription* subscription = client->subscribe(newTopic, 0);
        if (subscription != nullptr) {
            qDebug() << "[Subcription] Subscribed to topic:" << topic.filter();
            addSubcriptionPointer(subscription);

        } else {
            qWarning() << "[Warning] Failed to subscribe to topic: " << topic.filter();
        }

    }
}

void mqttHandler::addTopic(const char *topicName)
{
    if(topicName != nullptr){
        topics.append(QMqttTopicFilter(topicName));
    }
}

void mqttHandler::addSubcriptionPointer(QMqttSubscription *newSubcription)
{
    subcriptionsManager.append(newSubcription);
}

void mqttHandler::addTopicList(const char **topicNameList, const int &size)
{
    if(topicNameList != nullptr){
        for(int i = 0; i < size; ++i){
            addTopic(topicNameList[i]);
        }
    }
}

void mqttHandler::clearSubscriptionsManager()
{
    foreach(const QMqttSubscription* item, subcriptionsManager){
        delete item;
    }
}
