#include "BidirectionalConnection.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "httprequest.h"
BidirectionalConnection::BidirectionalConnection(const QUrl& url, QObject *parent)
    : QObject(parent), m_url(url) // Initialize m_url with the provided URL
{
    QObject::connect(&webSocket, &QWebSocket::connected, this, &BidirectionalConnection::onConnected);
    // QObject::connect(&webSocket, &QWebSocket::error, this, &BidirectionalConnection::onError);
    QObject::connect(&webSocket, &QWebSocket::textMessageReceived, this, &BidirectionalConnection::onTextMessageReceived);
}

void BidirectionalConnection::startConnection()
{
    qDebug() <<"Opening Connection to Server socket" << this->m_url;
    webSocket.open(this->m_url);

}


void BidirectionalConnection::login(const QString &username, const QString &password)
{
    if (webSocket.state() == QAbstractSocket::ConnectedState) {
        HttpRequest request;
        request.setUrl("/login");
        request.setMethod("POST");

        QJsonObject jsonBody;
        jsonBody["username"] = username;
        jsonBody["password"] = password;
        request.setBody(jsonBody);

        webSocket.sendTextMessage(request.toJsonString());
        qDebug() << "Sent Login Request";
    } else {
        qDebug() << "WebSocket is not connected, cannot send login request.";
    }
}

void BidirectionalConnection::sendMessage(const QString &message)
{
    webSocket.sendTextMessage(message);
}


void BidirectionalConnection::onError()
{
    qDebug() << "WebSocket Error!";
}


void BidirectionalConnection::onConnected()
{
    qDebug() << "WebSocket connected!";
    emit connected();

}

void BidirectionalConnection::onTextMessageReceived(const QString &message)
{
    qDebug() << "Message received:" << message;
    // Process incoming messages as needed
}
