#include "HttpResponse.h"

HttpResponse::HttpResponse() : status(200) {}

bool HttpResponse::fromJsonString(const QString& jsonString) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (!doc.isObject()) return false;

    QJsonObject json = doc.object();
    if (json.contains("body") && json["body"].isObject()) {
        body = json["body"].toObject();
    }
    if (json.contains("status") && json["status"].isDouble()) {
        status = json["status"].toInt();
    }
    return true;
}

QString HttpResponse::getBody() const {
    return QString(QJsonDocument(body).toJson(QJsonDocument::Compact));
}

int HttpResponse::getStatus() const {
    return status;
}
