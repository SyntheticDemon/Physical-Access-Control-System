#include "BidirectionalConnection.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "httprequest.h"
BidirectionalConnection::BidirectionalConnection(const QUrl& url, QObject *parent)
    : QObject(parent), m_url(url)
{
    QObject::connect(&webSocket, &QWebSocket::connected, this, &BidirectionalConnection::onConnected);
    // QObject::connect(&webSocket, &QWebSocket::error, this, &BidirectionalConnection::onError);
    QObject::connect(&webSocket, &QWebSocket::disconnected, this, &BidirectionalConnection::onDisconnected);
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
    bool a=true;
    emit connectionChanged(a);
}

void BidirectionalConnection::onConnected()
{
    bool a =false;
    qDebug() << "WebSocket connected!";
    emit connectionChanged(a);

}


void BidirectionalConnection::onDisconnected()
{
    bool a =true;
    qDebug() << "WebSocket disconnected!";
    emit connectionChanged(a);

}

void BidirectionalConnection::handleSocketResponse(const QJsonObject& historyResponse){
    if (historyResponse["type"] == "history"){
        emit emitHistory(historyResponse);
    }
    else{
        qDebug() << "Response did not require handling";
    }
}
void BidirectionalConnection::onTextMessageReceived(const QString &message)
{
    qDebug() << "Text Message Received from Server" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject response;
    if (!doc.isNull() && doc.isObject()) {
        response= doc.object();
    }
    QJsonObject body = response;
    handleSocketResponse(response);

}
