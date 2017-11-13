#include "mopidyreader.h"
#include <QDebug>

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QJsonDocument>



MopidyReader::MopidyReader(QObject *parent) : QObject(parent)
{
    currentUpdate = 0;

    title = "-";
    artist = "-";
    state = "*";
    length = -1;
    songProgress = 0;

    this->Test();
}

void MopidyReader::Test()
{

    qDebug() << "connecting...";

    connect(&m_webSocket, &QWebSocket::connected, this, &MopidyReader::onConnected);
    //connect(&m_webSocket, &QWebSocket::disconnected, this, &MopidyReader::closed);

    QString url = "ws://raspiclock:6680/mopidy/ws";
    m_webSocket.open(QUrl(url));
}

void MopidyReader::onConnected()
{
        qDebug() << "Connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &MopidyReader::onTextMessageReceived);


    QByteArray a = this->getState();


    //m_webSocket.sendTextMessage(QString("{\"jsonrpc\": \"2.0\", \"id\": %1, \"method\": \"core.playback.next\"}").arg(Play));


    QJsonObject json;
    json["jsonrpc"] = "2.0";
    json["id"] = 1;
    json["method"] = "core.playback.get_current_track";
    QJsonDocument doc(json);
    QString jstring = doc.toJson();

 //   qDebug() << "ASDFASDFASDFASDF : " << jstring;

 //   m_webSocket.sendTextMessage(jstring);
}

void MopidyReader::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;


    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(message.toUtf8(),&jsonError);

    if (jsonError.error != QJsonParseError::NoError){
        qDebug() << "ARSCH: " << jsonError.errorString();
    }

    //qDebug() << json.toJson(QJsonDocument::Indented);

    MessageId id = (MessageId)json.object()["id"].toInt();

    if(json.object()["event"].isNull())
    {
        qDebug() << "this is not an event";

        if(id == Play)
        {
            qDebug() << "YEAH you hit play";
        }
        else
        {
            qDebug() << "Sorry cant read this";
        }
        qDebug() << "AND THE ID IS: "  << id;
    }
    else
    {

        QString event = json.object()["event"].toString();
        qDebug() << "YEAH event received: " << event;

        if( event == "playback_state_changed")
        {
            QString newState = json.object()["new_state"].toString();
            qDebug() << "new state: " << newState;

            this->state = newState;

            if(this->state == "playing")
            {
                this->state = "▶";
            }

            if(this->state == "paused")
            {
                this->state = "||";
            }

            emit DataChanged();
        }
        else if(event == "track_playback_started")
        {
            this->title = json.object()["tl_track"].toObject()["track"].toObject()["name"].toString();
            this->artist = json.object()["tl_track"].toObject()["track"].toObject()["artists"].toArray()[0].toObject()["name"].toString();
            emit DataChanged();
        }
    }






    //m_webSocket.close();
}








void MopidyReader::UpdateSong()
{
    //titleManager->post(this->getRequest(), getCurrentTrack() );
}

void MopidyReader::UpdateState()
{
    //stateManager->post(this->getRequest(), getState() );
}

void MopidyReader::UpdatePosition()
{
    //positionManager->post(this->getRequest(), getCurrentPos() );
}

QByteArray MopidyReader::getCurrentTrack()
{
    QJsonObject json;
    json["jsonrpc"] = "2.0";
    json["id"] = 1;
    json["method"] = "core.playback.get_current_track";
    QJsonDocument doc(json);

    return doc.toJson();
}

QByteArray MopidyReader::getCurrentPos()
{
    QJsonObject json;
    json["jsonrpc"] = "2.0";
    json["id"] = 1;
    json["method"] = "core.playback.get_time_position";
    QJsonDocument doc(json);

    return doc.toJson();
}

QByteArray MopidyReader::getState()
{
    QJsonObject json;
    json["jsonrpc"] = "2.0";
    json["id"] = 1;
    json["method"] = "core.playback.get_state";
    QJsonDocument doc(json);

    return doc.toJson();
}

QNetworkRequest MopidyReader::getRequest()
{
    QString host = "http://localhost:6680/mopidy/rpc";
    QNetworkRequest request;
    request.setUrl(QUrl(host));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    return request;
}
