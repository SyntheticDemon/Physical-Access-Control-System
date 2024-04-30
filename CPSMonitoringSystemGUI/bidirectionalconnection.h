#ifndef BIDIRECTIONALCONNECTION_H
#define BIDIRECTIONALCONNECTION_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QUrl>

class BidirectionalConnection : public QObject
{
    Q_OBJECT

public:

    explicit BidirectionalConnection(const QUrl& url,QObject *parent = nullptr);
    void startConnection();
    void login(const QString &username, const QString &password);
signals:
    void connectionChanged(bool &enabled);
    void emitHistory(const QJsonObject& historyResponse);
    void emitDetails(const QString &username, const QString &date, const QString &time);
private:
    void handleSocketResponse(const QJsonObject& historyResponse);
    QWebSocket webSocket;
    void sendMessage(const QString &message);
    QUrl m_url;
private slots:
    void onConnected();
    void onError();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
};

#endif // BIDIRECTIONALCONNECTION_H
