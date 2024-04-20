#ifndef BIDIRECTIONALCONNECTION_H
#define BIDIRECTIONALCONNECTION_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>//

class BidirectionalConnection : public QObject
{
    Q_OBJECT

public:
    explicit BidirectionalConnection(QObject *parent = nullptr);
    void connect(const QUrl &url);
    void login(const QString &username, const QString &password);

private:
    QWebSocket webSocket;

private slots:
    void onConnected();
    void onTextMessageReceived(const QString &message);
};

#endif // BIDIRECTIONALCONNECTION_H
