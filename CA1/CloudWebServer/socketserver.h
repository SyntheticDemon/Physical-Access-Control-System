#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>

class SocketServer: public QWebSocketServer
{
    Q_OBJECT

public:
    explicit SocketServer(quint16 port, QObject *parent = nullptr);

private slots:
    void handleNewConnection();
    void onTextMessageReceived(const QString &message);
};

#endif // SOCKET_SERVER_H
