// HttpServer.cpp
#include "Server.h"
#include <QCoreApplication>
#include <QtHttpServer/QHttpServer>
#include <QtHttpServer/QHttpServerResponse>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#define SCHEME "http"
#define HOST "127.0.0.1"
#define  SOCKET_SERVER_PORT 8086
#define  HTTP_SERVER_PORT  8087
#include <QtHttpServer/QHttpServer>
#include <QtHttpServer/QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>

void Server::setupHTTPServer(){
    _m_httpServer.route("/checkRFID", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse {
        if (request.url().path() == "/checkRFID") {
            QJsonDocument doc = QJsonDocument::fromJson(request.body());
            QJsonObject obj = doc.object();
            QString rfid = obj["rfid"].toString();

            QJsonObject jsonResponse;
            jsonResponse["exists"] = m_rfids.contains(rfid);
            jsonResponse["message"] = m_rfids.contains(rfid) ? "RFID found." : "RFID not found.";

            QJsonDocument respDoc(jsonResponse);
            return QHttpServerResponse("application/json", respDoc.toJson());
        } else {
            return QHttpServerResponse("application/text", "Not Found");
        }
    });

    if (!_m_httpServer.listen(QHostAddress::Any, HTTP_SERVER_PORT)) {
        qCritical() << "Failed to open HTTP server on port" << HTTP_SERVER_PORT;
    } else {
        qInfo() << "Started HTTP server on port" << HTTP_SERVER_PORT;
    }
}

Server::Server(QObject *parent)
    : QObject(parent), _m_socketServer(SOCKET_SERVER_PORT, this)
{
    loadRFIDsFromJson("rfids.json");
    loadUsersFromJson("users.json");
    Server::setupHTTPServer();
}

QJsonObject Server::loadJsonFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open the file.";
        return {};  // Return an empty JSON object if the file doesn't open
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (!doc.isObject()) {
        qWarning() << "The file does not contain a JSON object.";
        return {};
    }

    return doc.object();
}

void Server::loadRFIDsFromJson(const QString &filePath)
{
    QStringList rfids;
    QJsonObject json = loadJsonFromFile(filePath);
    if (!json.isEmpty()) {
        qDebug() << "JSON loaded successfully!";
        QJsonArray rfidArray = json.value("rfids").toArray();
        for (const QJsonValue &value : rfidArray) {
            rfids.append(value.toString());
        }
    } else {
        qDebug() << "Failed to load or parse RFID JSON.";
    }
    this->m_rfids = rfids;
}

void Server::loadUsersFromJson(const QString &filePath)
{
    QJsonObject json = loadJsonFromFile(filePath);
    if (!json.isEmpty()) {
        qDebug() << "JSON loaded successfully!";
        this->m_users = json;
    } else {
        qDebug() << "Failed to load or parse USER JSON.";
    }
}
