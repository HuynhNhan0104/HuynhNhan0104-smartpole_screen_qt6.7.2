#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <cstdlib>
#include <QTimer>

class HttpHandler: public QObject
{
    Q_OBJECT
    // QML_ELEMENT


public:

    HttpHandler(QObject* parent = nullptr, QString api = "", int id = -1,int stream_id = 1, int maxTimeout = 5);
    ~HttpHandler();
    // add Q_INVOKABLE to call in qml
    Q_INVOKABLE void sendRequest();

    int getId() const;
    void setId(int id);
    int getStreamId() const;
    void setStreamId(int id);
    void sendRequestToAPI(QString api);

    void relyRequest(QNetworkReply *reply);
    void getUrlFromStreamId(int stream_id);

    void periodicConnect();

signals:
    void receiveLinkFromRequest(const QByteArray &message);
    void requestTimeout();
private:
    QString api = "";
    int id;
    int stream_id;
    int count_timeout;
    bool timeout = false;
    int maxTimeout;
    int reconnectTime = 30000;// 30s
    QTimer* timer = nullptr;
    QNetworkAccessManager* manager;
};

#endif // HTTPHANDLER_H
