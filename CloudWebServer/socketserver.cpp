#include "socketserver.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

SocketServer::SocketServer(quint16 port, QObject *parent)
    : QWebSocketServer(QStringLiteral("Bidirectional Connection Server"), NonSecureMode, parent),
    m_client(nullptr) {
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSocket server listening on port" << port;
        connect(this, &QWebSocketServer::newConnection, this, &SocketServer::handleNewConnection);
    } else {
        qWarning() << "Failed to start WebSocket server";
    }
}
SocketServer::~SocketServer() {
    if (m_client) {
        m_client->deleteLater();
    }
}

void SocketServer::kickout(){
    qDebug() << "Kickout called";
    if (m_client != nullptr) {
        qDebug() << "detatching client";
        m_client->disconnect(this);
        m_client->close();
        m_client->deleteLater();
        m_client = nullptr;
    }

}

void SocketServer::handleNewConnection() {
    if (m_client != nullptr) {
        qDebug() << "Dismissing old Client, attaching new client";
        m_client->disconnect(this);
        m_client->close();
        m_client->deleteLater();
        m_client = nullptr;
    }

    m_client = nextPendingConnection();
    if (m_client) {
        connect(m_client, &QWebSocket::textMessageReceived, this, &SocketServer::onTextMessageReceived);
        connect(m_client, &QWebSocket::disconnected, this, &SocketServer::onClientDisconnected);
    } else {
        qDebug() << "Failed to establish a new connection properly.";
    }
}



void SocketServer::onTextMessageReceived(const QString &message)
{
    qDebug() << "Text Message Received from Client" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject request;
    if (!doc.isNull() && doc.isObject()) {
        request= doc.object();
    }
    QJsonObject body = request["body"].toObject();
    QString path = request["url"].toString();
    sendMessageToClient("Received your " + path + " Request");
    handleSocketRequest(request,path);
}

void SocketServer::handleSocketRequest(const QJsonObject &request,const QString &path){
    qDebug() << "Processing " + path + " Request";
    if (path == "/login"){
        emit sendAuthSignal(request);
    }
    else{
        QJsonObject responseObj;
        responseObj["message"]="Not Found";
        responseObj["code"]="404";
        QJsonDocument responseDoc(responseObj);
        QString responseJsonString = QString::fromUtf8(responseDoc.toJson(QJsonDocument::Compact));
        sendMessageToClient(responseJsonString);
        this->kickout();
    }
}

void SocketServer::onClientDisconnected() {
    if (m_client == sender()) {
        m_client = nullptr;
    }
}

void SocketServer::sendMessageToClient(const QString &message) {
    if (m_client && m_client->state() == QAbstractSocket::ConnectedState) {
        m_client->sendTextMessage(message);
    } else {
        qDebug() << "No client connected or client is not in a valid state.";
    }
}
