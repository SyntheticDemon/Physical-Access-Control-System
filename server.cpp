// HttpServer.cpp
#include "Server.h"
#include <QCoreApplication>
#include <QtHttpServer/QHttpServer>
#include <QtHttpServer/QHttpServerResponse>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QJsonValue>
#define SCHEME "http"
#define HOST "127.0.0.1"
#define  SOCKET_SERVER_PORT 8086
#define  HTTP_SERVER_PORT  8087
#include <QtHttpServer/QHttpServer>
#include <QtHttpServer/QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
QString findUsernameByRFID(const QMap<QString, QString>& map, const QString& rfid) {
    for (auto it = map.begin(); it != map.end(); ++it) {
        if (it.value() == rfid) {
            return it.key();
        }
    }
    return QString();
}
void Server::setupHTTPServer(){
    _m_httpServer.route("/checkRFID", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) -> QHttpServerResponse {
        if (request.url().path() == "/checkRFID") {
            QJsonDocument doc = QJsonDocument::fromJson(request.body());
            QJsonObject obj = doc.object();
            QString rfid = obj["rfid"].toString();
            QJsonObject jsonResponse;
            QDateTime now = QDateTime::currentDateTime();

            bool rfidExistence = m_userRFIDs.values().contains(rfid);


            jsonResponse["exists"] = m_userRFIDs.values().contains(rfid);
            jsonResponse["message"] = m_userRFIDs.values().contains(rfid) ? "RFID found." : "RFID not found.";
            jsonResponse["date"] = now.date().toString(Qt::ISODate);
            jsonResponse["time"] = now.time().toString(Qt::ISODate);
            jsonResponse["username"] = findUsernameByRFID(m_userRFIDs,rfid);
            QJsonDocument respDoc(jsonResponse);
            jsonResponse["type"] = "history";

            if (jsonResponse["exists"]== true){

                QString historyLog(this->formHistory(jsonResponse));
                qDebug() << "Emitting History Log" << historyLog ;
                this->_m_socketServer.sendMessageToClient(historyLog);

                // Append historyLog to JSON file
                qDebug() <<"aaa"<<endl;
                QFile file("history.json");
                qDebug() <<"bbb"<<endl;
                historyArray.append(historyLog);
                qDebug() <<"ccc"<<endl;
                QJsonDocument jsonWriteDoc(historyArray);
                qDebug() <<"dddd"<<endl;
                if (file.open(QIODevice::ReadWrite)) {
                    qDebug() <<"eee"<<endl;
                    file.write(jsonWriteDoc.toJson());
                    file.close();

                } else {

                    qCritical() << "Failed to open JSON file for appending.";
                }

            }

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
    loadDataFromJson("users.json");
    connect(&_m_socketServer, &SocketServer::sendAuthSignal, this, &Server::authenticateUser);
    historyArray = this->readHistory("history.json");
    Server::setupHTTPServer();
}

QJsonObject Server::loadJsonFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open the file.";
        return {};
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

void Server::loadDataFromJson(const QString &filePath)
{

    QJsonObject json = loadJsonFromFile(filePath);
    QJsonArray usersArray = json["users"].toArray();
    for (const QJsonValue &userValue : usersArray) {
        QJsonObject userObj = userValue.toObject();
        QString username = userObj["username"].toString();
        QString rfid = userObj["rfid"].toString();
        QString password = userObj["password"].toString();
        m_userRFIDs[username] = rfid;
        m_userPasswords[username]=password;
    }
}

QString Server::authenticateUser(const QJsonObject &request) {
    qDebug() << "Authenticating User with request" << request;
    QString username = request["body"].toObject()["username"].toString();
    QString password = request["body"].toObject()["password"].toString();
    QJsonObject responseObj;

    if (m_userPasswords.contains(username) && m_userPasswords[username] == password) {
        QString rfid = m_userRFIDs[username];
        QDateTime now = QDateTime::currentDateTime();
        responseObj["code"] = 200;
        responseObj["date"] = now.date().toString(Qt::ISODate);
        responseObj["type"] = "login_response";
        responseObj["time"] = now.time().toString(Qt::ISODate);
        responseObj["rfid"] = rfid;

        for (const auto& historyEntry : this->historyArray) {
            QJsonObject historyObj = historyEntry.toObject();
            QString historyLog = this->formHistory(historyObj);
            this->_m_socketServer.sendMessageToClient(historyLog);
        }

    } else {
        responseObj["code"] = 403;
        responseObj["type"] = "login_response";
        responseObj["message"] = "Authentication Failed";
        this->_m_socketServer.kickout();
    }
    QJsonDocument responseDoc(responseObj);
    QString responseJsonString = QString::fromUtf8(responseDoc.toJson(QJsonDocument::Compact));
    this->_m_socketServer.sendMessageToClient(responseJsonString);
    return responseJsonString;
}

QJsonArray Server::readHistory(const QString &fileName)
{
    QJsonObject json = loadJsonFromFile(fileName);
    QJsonArray historyArray ;
    for (auto it = json.constBegin(); it != json.constEnd(); ++it) {
        historyArray.append(it.value());
    }
    //= QJsonDocument(json).toArray();
    return historyArray;
    // QFile file(fileName);
    // if (!file.open(QIODevice::ReadOnly)) {
    //     qWarning() << "Couldn't open the file.";
    //     return QJsonArray();
    // }

    // QByteArray fileData = file.readAll();
    // file.close();

    // QJsonDocument doc = QJsonDocument::fromJson(fileData);
    // if (!doc.isArray()) {
    //     qWarning() << "The file does not contain a JSON array.";
    //     return QJsonArray();
    // }

    // return doc.array();
}



QString Server::formHistory(QJsonObject& qJO){
    QJsonDocument responseDoc(qJO);
    QString responseJsonString = QString::fromUtf8(responseDoc.toJson(QJsonDocument::Compact));
    return responseJsonString;
}


