#include "httprequest.h"

HttpRequest::HttpRequest() {}

void HttpRequest::setUrl(const QString& url) {
    this->url = url;
}

void HttpRequest::setMethod(const QString& method) {
    this->method = method;
}

void HttpRequest::setBody(const QJsonObject& body) {
    this->body = body;
}

QString HttpRequest::toJsonString() const {
    QJsonObject json;
    json["url"] = url;
    json["method"] = method;
    json["body"] = body;
    QJsonDocument doc(json);
    return QString(doc.toJson(QJsonDocument::Compact));
}
