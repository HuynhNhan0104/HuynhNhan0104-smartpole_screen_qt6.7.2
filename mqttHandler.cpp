#include "mqttHandler.h"



MqttHandler::MqttHandler(QObject *parent, const char *hostName, uint16_t port, const char *userName, const char *password):
    QObject( parent ),
    client( new QMqttClient(this))
{
    client->setHostname(QString(hostName));
    client->setPort(quint16(port));
    client->setUsername(QString(userName));
    client->setPassword(QString(password));
    connect(client,&QMqttClient::connected,this,&MqttHandler::onConnected);
    connect(client,&QMqttClient::messageReceived,this,&MqttHandler::onMessageRecieved);
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
    emit recieveFromLinkTopic(message);


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

void MqttHandler::clearSubscriptionsManager()
{
    foreach(const QMqttSubscription* item, subcriptionsManager){
        delete item;
    }
}
