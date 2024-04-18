#include <QCoreApplication>
#include <QtHttpServer/QHttpServer>
#include <qfile.h>
#define SCHEME "http"
#define HOST "127.0.0.1"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

QJsonObject loadJsonFromFile(const QString &fileName) {
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
QStringList loadRFIDsFromJson(const QString &filePath) {
    QStringList rfids;
    QJsonObject json = loadJsonFromFile(filePath);

    if (!json.isEmpty()) {
        qDebug() << "JSON loaded successfully!";

        // Assuming RFIDs are stored in a JSON array under the key "rfids"
        QJsonArray rfidArray = json.value("rfids").toArray();
        for (const QJsonValue &value : rfidArray) {
            rfids.append(value.toString());
        }
    } else {
        qDebug() << "Failed to load or parse JSON.";
    }

    return rfids;
}

void setupHTTPServer(QStringList rfids){
    static QHttpServer httpServer;
    httpServer.route("/checkRFID", QHttpServerRequest::Method::Post, [&rfids](const QHttpServerRequest &request) {
        QJsonDocument doc = QJsonDocument::fromJson(request.body());
        QJsonObject obj = doc.object();
        QString rfid = obj["rfid"].toString();
        qCritical() <<"Received RFID" << rfid;
        qCritical() <<"RFIDS :  rfids" << rfids;
        QJsonObject response;
        response["exists"] = rfids.contains(rfid);
        response["message"] = rfids.contains(rfid) ? "RFID found." : "RFID not found.";

        QJsonDocument respDoc(response);
        return QHttpServerResponse("application/json", respDoc.toJson());
    });

    if (!httpServer.listen(QHostAddress::Any, 8088)) {
        qCritical() << "Failed to open HTTP server on port 8088";
    }
    qInfo() << "Started HTTP server on port 8088" ;

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList rfids =loadRFIDsFromJson("rfids.json");
    setupHTTPServer(rfids);

    return a.exec();
}
