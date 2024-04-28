#include <QWebSocketServer>
#include <QWebSocket>
#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

class SocketServer : public QWebSocketServer {
    Q_OBJECT

public:
    explicit SocketServer(quint16 port, QObject *parent = nullptr);
    ~SocketServer();
    void sendMessageToClient(const QString &message); // Method to send a message to the client
    void kickout();
signals:
    void sendAuthSignal(const QJsonObject &authDetails);
private slots:
    void onTextMessageReceived(const QString &message);
    void onClientDisconnected();

private:
    QWebSocket *m_client;
    void handleNewConnection();
    void handleSocketRequest(const QJsonObject &request,const QString &path);
};
#endif // SOCKETSERVER_H
