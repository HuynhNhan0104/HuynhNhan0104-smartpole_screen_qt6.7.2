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

class HttpHandler: public QObject
{
    Q_OBJECT
    // QML_ELEMENT


public:
    QString api = "";
    QNetworkAccessManager* manager;
    HttpHandler(QObject* parent = nullptr, QString api = "");
    ~HttpHandler();
    // add Q_INVOKABLE to call in qml
    Q_INVOKABLE void sendRequest();
    void sendRequestToAPI(QString api);

    void relyRequest(QNetworkReply *reply);
signals:
    void receiveLinkFromRequest(const QByteArray &message);
};

#endif // HTTPHANDLER_H
