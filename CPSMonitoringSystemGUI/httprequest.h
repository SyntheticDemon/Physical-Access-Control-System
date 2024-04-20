#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class HttpRequest {
public:
    HttpRequest();
    void setUrl(const QString& url);
    void setMethod(const QString& method);
    void setBody(const QJsonObject& body);
    QString toJsonString() const;

private:
    QString url;
    QString method;
    QJsonObject body;
};

#endif
