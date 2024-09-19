#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <QObject>
#include <QMqttClient>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <cstdlib>

class mqttHandler: public QObject
{
    Q_OBJECT
public:
    explicit mqttHandler(
        QObject* parent = nullptr,
        const char* hostName = nullptr,
        uint16_t port = 1880,
        const char* userName = nullptr,
        const char* password = nullptr
    );

    ~mqttHandler();

    void onConnected();
    void onMessageRecieved(const QByteArray &message, const QMqttTopicName &topic);
    // void onSubcribed(QMqttSubscription::SubscriptionState state);
    void subscribeAllTopic();




    void addTopic(const char* topicName);
    void addSubcriptionPointer(QMqttSubscription* newSubcription);
    void addTopicList(const char** topicNameList, const int &size);


    void clearSubscriptionsManager();

private:
    QList<QMqttTopicFilter> topics;
    QList<QMqttSubscription*> subcriptionsManager;
    QMqttClient* client = nullptr;
};

#endif // MQTTHANDLER_H
