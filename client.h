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

    //QString extractedField;
    QNetworkReply *reply;
    void connect();
    QString read_response();
    void processJson(const QString& jsonStr);
    QString read_field();




private:

    QStringList string_array;
    QByteArray responseData;
    QString strReplyEdit;
    //QString extractedField;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QEventLoop loop;

    std::string server_;
    std::string path_;
    boost::asio::io_service io_service_;
    boost::asio::ssl::context context_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
};

#endif // CLIENT_H

