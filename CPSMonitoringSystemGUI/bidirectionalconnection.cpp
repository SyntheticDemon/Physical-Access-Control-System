#include "BidirectionalConnection.h"
#include <QJsonDocument>
#include <QJsonObject>

#include "BidirectionalConnection.h"
#include "HttpRequest.h"
#include <QJsonDocument>
#include <QJsonObject>

BidirectionalConnection::BidirectionalConnection(QObject *parent) : QObject(parent)
{
    QObject::connect(&webSocket, &QWebSocket::connected, this, &BidirectionalConnection::onConnected);
    QObject::connect(&webSocket, &QWebSocket::textMessageReceived, this, &BidirectionalConnection::onTextMessageReceived);
}

void BidirectionalConnection::connect(const QUrl &url)
{
    webSocket.open(url);
}

void BidirectionalConnection::login(const QString &username, const QString &password)
{
    HttpRequest request;
    request.setUrl("/login");  // Assuming URL is passed here if needed, though typically not used in WebSocket
    request.setMethod("POST");

    QJsonObject jsonBody;
    jsonBody["username"] = username;
    jsonBody["password"] = password;
    request.setBody(jsonBody);

    webSocket.sendTextMessage(request.toJsonString());
}

void BidirectionalConnection::onConnected()
{
    qDebug() << "WebSocket connected!";
    // Optionally emit signal
}

void BidirectionalConnection::onTextMessageReceived(const QString &message)
{
    qDebug() << "Message received:" << message;
    // Process incoming messages as needed
}

