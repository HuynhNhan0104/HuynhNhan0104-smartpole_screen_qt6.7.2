#include "videoController.h"

// {"ID": [1, 2, 3], "link":"https://www.twitch.tv/rivers_gg_waitingroom"}

VideoController::VideoController(QObject *parent):
                                 // , const char *hostName, const uint16_t port, const char *userName, const char* password):
    QObject( parent )
    // client( new QMqttClient(this))
{
    // client->setHostname(QString(hostName));
    // client->setPort(quint16(port));
    // client->setUsername(QString(userName));
    // client->setPassword(QString(password));


    // connect(client,&QMqttClient::connected,this,&VideoController::onConnected);
    // connect(client,&QMqttClient::messageReceived,this,&VideoController::onMessageRecieved);
    // connect(client,&QMqttClient::errorChanged,this, [](QMqttClient::ClientError error) {
    //     qDebug()<<"[Error] "<< error << "\n";}
    // );



    // client->connectToHost();
    // subscribe topic
    const std::string defaultStdLink = defaultLink.toStdString();
    QString currentQuality = getQuality();
    QString newM3u8DefaultLink = parseM3u8Url(defaultStdLink , currentQuality.toStdString());

    setLink(newM3u8DefaultLink );

}

VideoController::~VideoController(){
    qDebug()<< "MQTT Deleted \n";
    // delete client;
    // clearSubscriptionsManager();
}


// void VideoController::onConnected()
// {
//     qDebug()<< "[Connection] Connect successfull \n";
//     // QMqttTopicFilter myTopic("NhanHuynh/feeds/led");
//     // QMqttSubscription* subscription = client->subscribe(myTopic, 0);
//     subscribeAllTopic();

// }

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


// void VideoController::onMessageRecieved(const QByteArray &message, const QMqttTopicName &topic)
// {
//     qDebug()<<"[Recieve]:[from: "<< topic <<"]:[Data: "<< message <<"] \n";
//     //json format {"ID": [1, 2, 3], "link":"file:///home/nhan/workspace/img/wave.mp4"}
//     // or {"ID": [1, 2, 3], "link":"file:///home/nhan/workspace/img/sea.mp4"}

//     QJsonDocument messageJsonDoc = QJsonDocument::fromJson(message);
//     if (messageJsonDoc.isObject()) {
//         // convert document to Oject
//         QJsonObject messageJsonObj = messageJsonDoc.object();

//         // Get json value from key
//         QJsonValue recievedIDList = messageJsonObj["ID"];
//         QJsonValue recievedLink = messageJsonObj["link"];

//         // Convert type from json to appropriated value
//         QJsonArray IDList = recievedIDList.toArray();
//         QString qlink = recievedLink.toString();

//         // parse m3u8 file
//         const std::string stdLink = qlink.toStdString();
//         QString currentQuality = getQuality();
//         QString newM3u8Link = parseM3u8Url(stdLink, currentQuality.toStdString());
//         setLink(newM3u8Link);
//     }
//     else{
//         qWarning() << "[Warning] data recieved is not json \n" ;
//     }
// }

// void VideoController::subscribeAllTopic()
// {
//     foreach(const QMqttTopicFilter &topic, topics){
//         QMqttTopicFilter newTopic(topic);
//         QMqttSubscription* subscription = client->subscribe(newTopic, 0);
//         if (subscription != nullptr) {
//             qDebug() << "[Subcription] Subscribed to topic:" << topic.filter();
//             addSubcriptionPointer(subscription);

//         } else {
//             qWarning() << "[Warning] Failed to subscribe to topic: " << topic.filter();
//         }

//     }
// }



// void VideoController::addSubcriptionPointer(QMqttSubscription *newSubcription)
// {
//     subcriptionsManager.append(newSubcription);
// }


// void VideoController::clearSubscriptionsManager()
// {
//     foreach(const QMqttSubscription* item, subcriptionsManager){
//         delete item;
//     }
// }


// void VideoController::addTopic(const char *topicName)
// {
//     if(topicName != nullptr){
//         topics.append(QMqttTopicFilter(topicName));
//     }
// }



// void VideoController::addTopicList(const char **topicNameList,const int &size)
// {
//     if(topicNameList != nullptr){
//         for(int i = 0; i < size; ++i){
//             addTopic(topicNameList[i]);
//         }
//     }
// }
// // std::string getM3U8Link(const std::string& twitchUrl,const std::string& type) {
// //     std::string command = "streamlink --stream-url " + twitchUrl + type;
// //     std::array<char, 128> buffer;
// //     std::string result;
// //     std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
// //     if (!pipe) throw std::runtime_error("popen() failed!");
// //     while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
// //         result += buffer.data();
// //     }
// //     return result;
// // }


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
