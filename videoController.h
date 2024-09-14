#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QMqttClient>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <cstdlib>



class VideoController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString link READ getLink WRITE setLink NOTIFY LinkChanged FINAL)
public:


    explicit VideoController(QObject* parent = nullptr,
                const char* hostName = nullptr,
                uint16_t port = 1880,
                const char* userName = nullptr,
                const char* password = nullptr
    );
    ~VideoController();

    void onConnected();
    void onMessageRecieved(const QByteArray &message, const QMqttTopicName &topic);
    // void onSubcribed(QMqttSubscription::SubscriptionState state);
    void subscribeAllTopic();




    void addTopic(const char* topicName);
    void addSubcriptionPointer(QMqttSubscription* newSubcription);
    void addTopicList(const char** topicNameList, const int &size);


    void clearSubscriptionsManager();


    QString getLink() const;
    void setLink(const QString &newLink);
    void setLink(const char* newLink);
    QString  getLastLink();
    QString parseM3u8Url(const std::string &url,const std::string &type);

    void setQuality(const char* newQuality);
    void setQuality(const QString &newQuality);
    QString getQuality();

    void setDefaultLink(const char* newDefaultLink);
    void setDefaultLink(const QString &newDefaultLink);
    QString getDefaultLink();


signals:
    void LinkChanged();

private:

    // quality in twitch is "160p", "360p", "480p" , "720p", "1080p", "worst" and "best"
    QString quality= QString("best");
    QList<QMqttTopicFilter> topics;
    QList<QMqttSubscription*> subcriptionsManager;
    QMqttClient* client = nullptr;
    QString defaultLink = QString("https://www.thvli.vn/live/thvl1-hd");
    QString m_link;
};

#endif // VIDEOCONTROLLER_H
