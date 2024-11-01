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
    // void setMediaPlayerLink(const QString &link);
    QString  getLastLink();
    QString parseM3u8Url(const std::string &url,const std::string &type);

    void setQuality(const char* newQuality);
    void setQuality(const QString &newQuality);
    QString getQuality();

    void setDefaultLink(const char* newDefaultLink);
    void setDefaultLink(const QString &newDefaultLink);
    QString getDefaultLink();


    void onReceiveLinkFromMqtt(const QByteArray &message);
    void onReceiveLinkFromHttp(const QByteArray &message);

signals:
    void LinkChanged();

private:

    // quality in twitch is "160p", "360p", "480p" , "720p", "1080p", "worst" and "best"
    QString quality= QString("best");
    QString defaultLink = QString("https://manifest.googlevideo.com/api/manifest/hls_playlist/expire/1730474625/ei/IJ4kZ_usPMqZ4t4Po9b8kAk/ip/125.235.233.151/id/rKn4EQ3-Ns0.1/itag/96/source/yt_live_broadcast/requiressl/yes/ratebypass/yes/live/1/sgoap/gir%3Dyes%3Bitag%3D140/sgovp/gir%3Dyes%3Bitag%3D137/rqh/1/hdlc/1/hls_chunk_host/rr4---sn-8pxuuxa-nboz6.googlevideo.com/xpc/EgVo2aDSNQ%3D%3D/playlist_duration/30/manifest_duration/30/spc/qtApASZCxPg22c16kS2ApR_WG9eE8sc0j5fS251UWbOkd-Ou7hd5uaWUuw/vprv/1/playlist_type/DVR/initcwndbps/1527500/met/1730453025,/mh/me/mm/44/mn/sn-8pxuuxa-nboz6/ms/lva/mv/m/mvi/4/pcm2cms/yes/pl/23/rms/lva,lva/dover/11/pacing/0/keepalive/yes/fexp/51312688,51326932,51331021/mt/1730452587/sparams/expire,ei,ip,id,itag,source,requiressl,ratebypass,live,sgoap,sgovp,rqh,hdlc,xpc,playlist_duration,manifest_duration,spc,vprv,playlist_type/sig/AJfQdSswRAIgBN-Mkh52_JTbYGMCW6_kcLuSjTFt0uu0swb4ti8joY0CIANgp0ykAdUychh9Q0_yW_4L8zBz17WsfMwnt7yOuFKW/lsparams/hls_chunk_host,initcwndbps,met,mh,mm,mn,ms,mv,mvi,pcm2cms,pl,rms/lsig/ACJ0pHgwRQIgEyIbrW6P6DdAhnKd_8azoo6QMU40O4SyKtyCLrklDjQCIQD6NjIhCRrwgSV-lfxCY2Mjh0OrSGfb_igM6ZHWT4wSMA%3D%3D/playlist/index.m3u8");
    // QString defaultLink = QString("https://www.youtube.com/watch?v=rKn4EQ3-Ns0");
    // {"ID": [1, 2, 3], "link":"udp://@192.168.196.76:1234?pkt_size=1316"}
    QString m_link;
};

#endif // VIDEOCONTROLLER_H
