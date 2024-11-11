#include "httpHandler.h"


HttpHandler::HttpHandler(QObject *parent, QString api, int id, int stream_id, int maxTimeout):
    api(api),
    id(id),
    stream_id(stream_id),
    maxTimeout(maxTimeout)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HttpHandler::relyRequest);
    count_timeout = 0;
    timeout = false;
    reconnectTime = 30000;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &HttpHandler::periodicConnect);

}



HttpHandler::~HttpHandler()
{
    delete manager;
    delete timer;
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

    QNetworkRequest request;
    request.setUrl(QUrl(api));
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("ngrok-skip-browser-warning", "true");
    manager->get(request);
}

void HttpHandler::relyRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug()<< responseData ;
        this->count_timeout = 0;
        if (this->timeout) this->timer->stop();
        this->timeout = false;
        emit receiveLinkFromRequest(responseData);
    } else {
        qDebug() << "Error occurred:" << reply->errorString();
        if(this->count_timeout < this->maxTimeout){
            this->count_timeout++;
            sendRequest();
        }
        else if(this->count_timeout >= this->maxTimeout && this->timeout == false){
            this->timeout = true;
            this->timer->start(this->reconnectTime); 
            emit requestTimeout();
        }
        
    }
    reply->deleteLater();

}

void HttpHandler::getUrlFromStreamId(int stream_id){
    setStreamId(stream_id);
    QString api_get = api + "?stream=" + QString::number(stream_id);
    qDebug() << "request to API: " << api_get;
    sendRequestToAPI(api_get);
}

void HttpHandler::periodicConnect(){
    sendRequest();
}
