#include "client.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/ssl.hpp>

Client::Client(const std::string& server, const std::string& path) : server_(server), path_(path), io_service_(), context_(boost::asio::ssl::context::sslv23), socket_(io_service_, context_) {}

/*void Client::connect() {
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(server_, "https");
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::connect(socket_.lowest_layer(), endpoint_iterator);
    socket_.handshake(boost::asio::ssl::stream_base::client);
}*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/*void Client::connect() {
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    // manager->setParent(this); // Удалите эту строку
    QNetworkRequest request;
    request.setUrl(QUrl("https://yourserver.com"));
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Client::onFinished);
    // Добавьте следующую строку, чтобы удалить manager после завершения запроса
    connect(reply, &QNetworkReply::finished, manager, &QNetworkAccessManager::deleteLater);
}


void Client::onFinished() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QString response = QString::fromUtf8(responseData);
            qDebug() << "Response: " << response;
            // Дальнейшая обработка ответа
        } else {
            qDebug() << "Error: " << reply->errorString();
            // Обработка ошибки
        }
        reply->deleteLater();
    }
}
*/

void Client::connect() {
    // указываем URL для запроса
    request.setUrl(QUrl("https://iis.bsuir.by/api/v1/schedule?studentGroup=353504"));

    // отправляем GET-запрос и получаем ответ
    QNetworkReply *reply = manager.get(request);

    // ожидаем завершения запроса
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    // выводим результат запроса
    qDebug() << reply->readAll();

}

/*void Client::send_request() {
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path_ << " HTTPS/1.1\r\n";
    request_stream << "Host: " << server_ << "\r\n";
    request_stream << "Accept: application/json\r\n";
    request_stream << "Connection: close\r\n\r\n";
    boost::asio::write(socket_, request);
}

std::string Client::read_response() {
    boost::asio::streambuf response;
    boost::asio::read_until(socket_, response, "\r\n");
    std::istream response_stream(&response);
    std::string https_version;
    response_stream >> https_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);

    if (!response_stream || https_version.substr(0, 5) != "HTTP/") {
        return "Invalid response";
    }

    if (status_code >= 300 && status_code < 400) {
        // Обработка перенаправлений здесь
    }

    std::ostringstream oss;
    oss << &response;
    std::string str_response = oss.str();

    // Парсинг JSON
    std::istringstream iss(str_response);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(iss, pt);

    // Используйте pt для доступа к значениям JSON
    // Например, если JSON содержит {"key": "value"}, вы можете получить значение следующим образом:
    // std::string value = pt.get<std::string>("key");

    return str_response;
}
*/

