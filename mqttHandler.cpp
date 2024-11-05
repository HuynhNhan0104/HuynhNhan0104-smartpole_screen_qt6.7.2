#include "mqttHandler.h"



MqttHandler::MqttHandler(QObject *parent, QString hostName, uint16_t port, QString  userName,QString password,int id):
    QObject( parent ),
    id(id),
    client( new QMqttClient(this))
{
    client->setHostname(hostName);
    client->setPort(quint16(port));
    client->setUsername(userName);
    client->setPassword(password);
    connect(client,&QMqttClient::connected,this,&MqttHandler::onConnected);
    // connect(client,&QMqttClient::messageReceived,this,&MqttHandler::onMessageRecieved);
    connect(client,&QMqttClient::messageReceived,this,&MqttHandler::onStreamIdRecieved);
    connect(client,&QMqttClient::errorChanged,this, [](QMqttClient::ClientError error) {
        qDebug()<<"[Error] "<< error << "\n";}
    );
    client->connectToHost();
}

MqttHandler::~MqttHandler()
{
    qDebug()<< "MQTT Deleted \n";
    delete client;
    clearSubscriptionsManager();
}

void MqttHandler::onConnected()
{
    subscribeAllTopic();
}

void MqttHandler::onMessageRecieved(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug()<<"[Recieve]:[from: "<< topic <<"]:[Data: "<< message <<"] \n";
    //json format {"ID": [1, 2, 3], "link":"file:///home/nhan/workspace/img/wave.mp4"}
    // or {"ID": [1, 2, 3], "link":"file:///home/nhan/workspace/img/sea.mp4"}
    // emit onStreamIdRecieved(message);
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
        qDebug() << "ID LIST " << IDList;
        // parse m3u8 file
        qDebug() << "link received: " << qlink;
        // setMediaPlayerLink(stdLink);
        // if not contain id just ignore
        if( IDList.contains(QJsonValue(this->getId()))){
            emit recieveLinkFromTopic(qlink);

        } else {
            qDebug() << "Ignore, none of this business";
        }
    } else {
        qWarning() << "[Warning] data recieved is not json \n" ;
    }



}
void MqttHandler::onStreamIdRecieved(const QByteArray &message, const QMqttTopicName &topic){
    qDebug()<<"[Recieve]:[from: "<< topic <<"]:[Data: "<< message <<"] \n";
    QJsonDocument messageJsonDoc = QJsonDocument::fromJson(message);
    if (messageJsonDoc.isObject()) {
        QJsonObject messageJsonObj = messageJsonDoc.object();
        QJsonValue recievedIDList = messageJsonObj["data"];
        QJsonArray IDList = recievedIDList.toArray();
        int stream_id = 0;
        for (const QJsonValue &value : IDList) {
            QJsonObject obj = value.toObject();
            qDebug() << obj;
            if (obj.contains("Pole_ID") && obj["Pole_ID"].toInt() == this->id) {
                stream_id = obj["Stream_ID"].toInt();

                break; // Exit loop if match is found
            }
        }
        if(stream_id){
            emit recieveStreamIdFromTopic(stream_id);
        }

    } else {
        qWarning() << "[Warning] data recieved is not json \n" ;
    }
}

void MqttHandler::subscribeAllTopic()
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

void MqttHandler::addTopic(const char *topicName)
{
    if(topicName != nullptr){
        topics.append(QMqttTopicFilter(topicName));
    }
}
void MqttHandler::addTopic(const QString& topicName)
{
    if(topicName != ""){
        topics.append(QMqttTopicFilter(topicName));
    }
}

void MqttHandler::addSubcriptionPointer(QMqttSubscription *newSubcription)
{
    subcriptionsManager.append(newSubcription);
}

void MqttHandler::addTopicList(const char **topicNameList, const int &size)
{
    if(topicNameList != nullptr){
        for(int i = 0; i < size; ++i){
            addTopic(topicNameList[i]);
        }
    }
}

void MqttHandler::setPublishTopic(const QString& pubtopic)
{
    this->publishTopic = pubtopic;
}

void MqttHandler::clearSubscriptionsManager()
{
    foreach(const QMqttSubscription* item, subcriptionsManager){
        delete item;
    }
}

int MqttHandler::publish(const QString &topic, const QByteArray &message, int qos, bool retain)
{
    return client->publish(QMqttTopicName(topic), message, qos, retain);
}

void MqttHandler::publishSensorData(const QByteArray &message)
{
    if( !publish( publishTopic, message, 0, true ) ){
        qDebug() <<"PUBLISH SUCCESS" << message;
    }
    else{
        qDebug() <<"PUBLISH FAIL" << message;
    }

}

int MqttHandler::getId() const
{
    return this->id;
}

void MqttHandler::setId(int id)
{
    this->id = id;
}
