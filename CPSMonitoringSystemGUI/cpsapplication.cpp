  #include "cpsapplication.h"
#include "BidirectionalConnection.h"
#include <qurl.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace CPS {

Application::Application(QObject *parent)
    : QObject{parent},
    _window(new MainWindow),
    _history(new HistoryWindow)
{
    setWindowsThemeToDark<MainWindow>(*_window);
    QObject::connect(_window, &MainWindow::connectBtnClicked, this, &Application::setupWebSocket);
    QObject::connect(_window, &MainWindow::historyuBtnClicked, this, &Application::showHistoryWindow);

}

Application::~Application()
{
    delete this->_window;
    delete this->_history;
    delete this->_connection;
}

void Application::setupWebSocket(const QString &url, const QString &username, const QString &password)
{
    qDebug() << "Setting up WebSocket" << url;
    QUrl websocketUrl(url);
    _connection = new BidirectionalConnection(websocketUrl, this);
    connect(_connection, &BidirectionalConnection::connectionChanged, this, [this, username, password]() {
        _connection->login(username, password);
    });
    QObject::connect(this->_connection, &BidirectionalConnection::connectionChanged,this->_window, &MainWindow::changeRightPanelEnabled);
    _connection->startConnection();
    QObject::connect(_connection, &BidirectionalConnection::emitHistory, this, &Application::receiveHistory);
    QObject::connect(_connection, &BidirectionalConnection::emitDetails, _window, &MainWindow::showUserDetails);

}

void Application::show()
{
    this->_window->show();
}

void Application::receiveHistory(const QJsonObject &history){
    this->_historyArray.append(history);
}
void Application::showHistoryWindow()
{
    setWindowsThemeToDark<HistoryWindow>(*_history);
    // TODO:
    /*
         * fetch data from server and show it in history window.
         * your data must be in QJsonArray format.
         * something like this:
         *
         * [
         *     {
         *          username: string,
         *          date: string,
         *          time: string,
         *     },
         *
         *     {
         *          username: string,
         *          date: string,
         *          time: string,
         *     }
         * ]
         *
         *  below is an example of how to create a QJsonArray from QVariantList: (beginer level)
         *  please erase this horrible example and implement your own logic.
         *  you must fetch a json from server
         *
         * */

    // QJsonObject obj1;
    // QJsonObject obj2;
    // QJsonObject obj3;

    // obj1["username"] = "1234567890";
    // obj1["date"] = "12/12/2024";
    // obj1["time"] = "10:00";

    // obj2["username"] = "0987654321";
    // obj2["date"] = "03/28/2024";
    // obj2["time"] = "12:00";

    // obj3["username"] = "5432167890";
    // obj3["date"] = "09/08/2024";
    // obj3["time"] = "14:00";

    // QVariantList list;
    // list.append(obj1);
    // list.append(obj2);
    // list.append(obj3);

    QJsonArray data = this->_historyArray;

    _history->show(data);
}

} // end of CPS
