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

private slots:
     QString authenticateUser(const QJsonObject &request);

private:
    void updateHistory(QJsonObject& history_log);
    QString formDetails(QJsonObject& qJO);
    QJsonArray historyArray;
    QString formHistory(QJsonObject& qJO);
    QHttpServer _m_httpServer;
    SocketServer _m_socketServer;
    QStringList m_rfids;
    QJsonObject  m_users;
    void        setupHTTPServer();
    void        setupSocketServer();
    QJsonObject loadJsonFromFile(const QString &fileName);
    QMap<QString, QString> m_userRFIDs;
    QMap<QString, QString> m_userPasswords;
    QJsonArray readHistory(const QString &fileName);
    void loadDataFromJson(const QString &filePath);

};

#endif // HTTPSERVER_H
