#include "httpHandler.h"


HttpHandler::HttpHandler(QObject *parent, QString api, int id):
    api(api),
    id(id)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HttpHandler::relyRequest);

}



HttpHandler::~HttpHandler()
{
    delete manager;
}

void HttpHandler::sendRequest()
{
    manager->get(QNetworkRequest(QUrl(api)));
}

int HttpHandler::getId() const
{
    return this->id;
}

void HttpHandler::setId(int id)
{
    this->id = id;
}

void HttpHandler::sendRequestToAPI(QString api)
{
    manager->get(QNetworkRequest(QUrl(api)));
}

void HttpHandler::relyRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();

        emit receiveLinkFromRequest(responseData);
    } else {
        qDebug() << "Error occurred:" << reply->errorString();
    }
    reply->deleteLater();

}

