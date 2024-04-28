#ifndef CPSAPPLICATION_H
#define CPSAPPLICATION_H

#include <QObject>
#include <QApplication>

#include "cpsmainwindow.h"
#include "cpshistorywindow.h"
#include "cpswindowsapitools.h"
#include "BidirectionalConnection.h"
namespace CPS {

class Application : public QObject
{
    Q_OBJECT

public:

    explicit Application(QObject *parent = nullptr);
    ~Application();
    QJsonArray _historyArray;
    void show();

Q_SIGNALS:

private Q_SLOTS:
    void receiveHistory(const QJsonObject &history);
    void setupWebSocket(const QString &url, const QString &username, const QString &password);
    void showHistoryWindow();

private: // methods

private: // members
    MainWindow    *_window;
    HistoryWindow *_history;
    BidirectionalConnection* _connection;
};

} // end of CPS

#endif // CPSAPPLICATION_H
