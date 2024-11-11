#include "videoController.h"

// {"ID": [1, 2, 3], "link":"https://www.twitch.tv/rivers_gg_waitingroom"}

VideoController::VideoController(QObject *parent,int id, QString defaultLink):
    QObject( parent ),
    id(id),
    defaultLink(defaultLink)

{
    const std::string defaultStdLink = defaultLink.toStdString();
    // setLink("http://192.168.196.76:5000/video_feed");
    // setLink("http://192.168.196.76:5000/video_feed");

    // setMediaPlayerLink(defaultStdLink);
    setLink(defaultLink);
    // QString currentQuality = getQuality();
    // QString newM3u8DefaultLink = parseM3u8Url(defaultStdLink , currentQuality.toStdString());

    // setLink(newM3u8DefaultLink );

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

void VideoController::setLink(const std::string &newLink)
{
    QString newQString= QString::fromStdString(newLink);
    if (m_link == newQString)
        return;
    m_link = newQString;
    emit LinkChanged();
}

void VideoController::setMediaPlayerLink(const std::string &newLink)
{
    if (newLink.substr(0, 4) == "file") {
        qDebug() << "link is local \n";
        setLink(newLink);
    } else {
        qDebug() << "link is an stream \n";
        QString currentQuality = getQuality();
        QString newM3u8Link = parseM3u8Url(newLink, currentQuality.toStdString());
        setLink(newM3u8Link);
    }
}

// void VideoController::setMediaPlayerLink(const QString &link)
// {
//     const std::string newLink = link.toStdString();
//     if (newLink.substr(0, 4) == "file") {
//         qDebug() << "link is local \n";
//         setLink(newLink);
//     } else {
//         qDebug() << "link is an stream \n";
//         QString currentQuality = getQuality();
//         QString newM3u8Link = parseM3u8Url(newLink, currentQuality.toStdString());
//         setLink(newM3u8Link);
//     }
// }

void VideoController::setQuality(const char* newQuality){
    quality = QString(newQuality);
}
void VideoController::setQuality(const QString &newQuality){
    quality = newQuality;
}
QString VideoController::getQuality(){
    return quality;
}

int VideoController::getId() const {
    return this->id;
}
void VideoController::setId(int id){
    this->id = id;
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

void VideoController::onReceiveLinkFromHttp(const QByteArray &message){
    QJsonDocument doc = QJsonDocument::fromJson(message);
    QJsonObject response = doc.object();
    QString link = response["link"].toString();
    qDebug() << "last link:" << link;
    // setMediaPlayerLink(link.toStdString());
    setLink(link);
}
void VideoController::onReceiveLinkFromMqtt(const QString& link){
    setLink(link);
}


void VideoController::onRequestTimeout(){
    setLink(this->defaultLink);
}
