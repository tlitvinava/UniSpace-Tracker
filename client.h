#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

class Client {
public:
    Client(const std::string& server, const std::string& path);

    QNetworkReply *reply;
    void connect();
    QString read_response();
    void processJson(const QString& jsonStr);
    QString read_field();
    void onLineEditTextChanged(const QString& text);
    void connect_groups();
    QString read_response_groups();
    void processJsonGroup(const QString& jsonStr);
    QString read_group();
    void connect_group_schedule();
    QStringList read_group_schedule();
    void processGroupSchedule(const QString& jsonStr);
    QStringList read_auditoriums();
    QString findScheduleAsString(const QString& auditoriumNumber, const QDate& date);

    //void connect_group_schedule353504();
    //void processGroupSchedule353504(const QString& jsonStr);

private:

    QMap<QString, QStringList> daySchedules;
    QStringList auditoriums;
    QMap<QString, QStringList> buildingRooms;
    QStringList string_array;
    QStringList string_array_group;
    QByteArray responseData;
    QString strReplyEditGroup;
    QString strReplyEdit;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QEventLoop loop;
    QStringList groupScheduleData;


    std::string server_;
    std::string path_;
    boost::asio::io_service io_service_;
    boost::asio::ssl::context context_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
};

#endif // CLIENT_H

