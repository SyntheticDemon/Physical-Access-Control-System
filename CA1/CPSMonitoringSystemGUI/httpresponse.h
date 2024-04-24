#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class HttpResponse {
public:
    HttpResponse();
    bool fromJsonString(const QString& jsonString);
    QString getBody() const;
    int getStatus() const;

private:
    QJsonObject body;
    int status;
};

#endif
