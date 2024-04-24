#include "socketserver.h"
#include <QDebug>

SocketServer::SocketServer(quint16 port, QObject *parent)
    : QWebSocketServer(QStringLiteral("Bidirectional Connection Server"), QWebSocketServer::NonSecureMode, parent)
{
    if (this->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSocket server listening on port" << port;
        connect(this, &QWebSocketServer::newConnection, this, &SocketServer::handleNewConnection);
    } else {
        qWarning() << "Failed to start WebSocket server";
    }
}

void SocketServer::handleNewConnection()
{
    QWebSocket *clientSocket = nextPendingConnection();
    qDebug() << "Received new connection" ;
    if (clientSocket) {
        connect(clientSocket, &QWebSocket::textMessageReceived, this, &SocketServer::onTextMessageReceived);
        connect(clientSocket, &QWebSocket::disconnected, clientSocket, &QWebSocket::deleteLater);
    }
}

void SocketServer::onTextMessageReceived(const QString &message)
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        qDebug() << "Message received from" << socket->peerAddress().toString() << ":" << socket->peerPort() << "-" << message;
        socket->sendTextMessage("Hello There Cunt");
    }
}
