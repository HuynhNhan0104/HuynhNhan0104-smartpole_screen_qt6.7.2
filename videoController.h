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
#include "mqttHandler.h"



class VideoController: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString link READ getLink WRITE setLink NOTIFY LinkChanged FINAL)
public:


    explicit VideoController(
        QObject* parent = nullptr
    );
    ~VideoController();

    QString getLink() const;
    void setLink(const QString &newLink);
    void setLink(const char* newLink);
    void setLink(const std::string &newLink);

    void setMediaPlayerLink(const std::string &newLink);
    QString  getLastLink();
    QString parseM3u8Url(const std::string &url,const std::string &type);

    void setQuality(const char* newQuality);
    void setQuality(const QString &newQuality);
    QString getQuality();

    void setDefaultLink(const char* newDefaultLink);
    void setDefaultLink(const QString &newDefaultLink);
    QString getDefaultLink();


    void onRecieveLinnk(const QByteArray &message);

signals:
    void LinkChanged();

private:

    // quality in twitch is "160p", "360p", "480p" , "720p", "1080p", "worst" and "best"
    QString quality= QString("best");
    QString defaultLink = QString("https://www.youtube.com/watch?v=rKn4EQ3-Ns0");
    QString m_link;
};

#endif // VIDEOCONTROLLER_H
