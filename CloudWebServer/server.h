// HttpServer.h

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include "socketserver.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

private:
    QHttpServer _m_httpServer;
    SocketServer _m_socketServer;
    QStringList m_rfids;
    QJsonObject  m_users;
    void        setupHTTPServer();
    void        setupSocketServer();
    QJsonObject loadJsonFromFile(const QString &fileName);
    void        loadRFIDsFromJson(const QString &filePath);
    void        loadUsersFromJson(const QString &filePath);

signals:

};

#endif // HTTPSERVER_H
