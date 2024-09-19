#include "videoController.h"

// {"ID": [1, 2, 3], "link":"https://www.twitch.tv/rivers_gg_waitingroom"}

VideoController::VideoController(QObject *parent):
    QObject( parent )

{
    const std::string defaultStdLink = defaultLink.toStdString();
    QString currentQuality = getQuality();
    QString newM3u8DefaultLink = parseM3u8Url(defaultStdLink , currentQuality.toStdString());

    setLink(newM3u8DefaultLink );

}

VideoController::~VideoController(){
    qDebug()<< "video controller Deleted \n";

}


QString VideoController::parseM3u8Url(const std::string  &url,const std::string &type)
{

    //if use yt-dlp: 'yt-dlp -f "480p" --get-url https://www.twitch.tv/kaicenat'
    std::string command = "streamlink --stream-url " + url +" "+ type;
    char buffer[225];
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        qDebug() << "streamlink failed!\n";

    }
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.cend());
    // qDebug()<<result;
    return QString::fromStdString(result);

}

QString VideoController::getLink() const
{
    return m_link;
}

void VideoController::setLink(const QString &newLink)
{
    if (m_link == newLink)
        return;
    m_link = newLink;
    emit LinkChanged();
}

void VideoController::setLink(const char* newLink)
{
    QString newQString(newLink);
    if (m_link == newQString)
        return;
    m_link = newQString;
    emit LinkChanged();
}


void VideoController::setQuality(const char* newQuality){
    quality = QString(newQuality);
}
void VideoController::setQuality(const QString &newQuality){
    quality = newQuality;
}
QString VideoController::getQuality(){
    return quality;
}



void VideoController::setDefaultLink(const char* newDefaultLink){
    defaultLink = QString(newDefaultLink);
}
void VideoController::setDefaultLink(const QString &newDefaultLink){
    defaultLink = newDefaultLink;
}

QString VideoController::getDefaultLink(){
    return defaultLink;
}

void VideoController::onRecieveLinnk(const QByteArray &message)
{
    QJsonDocument messageJsonDoc = QJsonDocument::fromJson(message);
    if (messageJsonDoc.isObject()) {
        // convert document to Oject
        QJsonObject messageJsonObj = messageJsonDoc.object();

        // Get json value from key
        QJsonValue recievedIDList = messageJsonObj["ID"];
        QJsonValue recievedLink = messageJsonObj["link"];

        // Convert type from json to appropriated value
        QJsonArray IDList = recievedIDList.toArray();
        QString qlink = recievedLink.toString();

        // parse m3u8 file
        const std::string stdLink = qlink.toStdString();
        QString currentQuality = getQuality();
        QString newM3u8Link = parseM3u8Url(stdLink, currentQuality.toStdString());
        setLink(newM3u8Link);
    }
    else{
        qWarning() << "[Warning] data recieved is not json \n" ;
    }
}
