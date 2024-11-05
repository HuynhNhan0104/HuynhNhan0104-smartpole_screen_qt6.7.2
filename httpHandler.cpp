#include "httpHandler.h"


HttpHandler::HttpHandler(QObject *parent, QString api, int id, int stream_id):
    api(api),
    id(id),
    stream_id(stream_id)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HttpHandler::relyRequest);

}



HttpHandler::~HttpHandler()
{
    delete manager;
}

void HttpHandler::sendRequest(){
    QString api_get = api + "?stream=" + QString::number(stream_id);
    qDebug() << api_get;
    QNetworkRequest request;
    request.setUrl(QUrl(api_get));
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("ngrok-skip-browser-warning", "true");
    manager->get(request);
}

int HttpHandler::getId() const
{
    return this->id;
}

void HttpHandler::setId(int id)
{
    this->id = id;
}

int HttpHandler::getStreamId() const
{
    return this->id;
}

void HttpHandler::setStreamId(int id)
{
    this->id = id;
}


void HttpHandler::sendRequestToAPI(QString api)
{
    QString api_get = api + "?stream=" + QString::number(stream_id);
    qDebug() << api_get;
    QNetworkRequest request;
    request.setUrl(QUrl(api_get));
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("ngrok-skip-browser-warning", "true");
    manager->get(request);
}

void HttpHandler::relyRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug()<< responseData ;

        emit receiveLinkFromRequest(responseData);
    } else {
        qDebug() << "Error occurred:" << reply->errorString();
    }
    reply->deleteLater();

}

void HttpHandler::getUrlFromStreamId(int stream_id){
    setStreamId(stream_id);
    QString api_get = api + "?stream=" + QString::number(stream_id);
    qDebug() << "request to API: " << api_get;
    sendRequestToAPI(api_get);
}

