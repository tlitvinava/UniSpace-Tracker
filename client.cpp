#include "client.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/ssl.hpp>

Client::Client(const std::string& server, const std::string& path) : server_(server), path_(path), io_service_(), context_(boost::asio::ssl::context::sslv23), socket_(io_service_, context_) {

}

void Client::connect() {
    // указываем URL для запроса
    request.setUrl(QUrl("https://iis.bsuir.by/api/v1/schedule?studentGroup=353504"));

    // отправляем GET-запрос и получаем ответ
    reply = manager.get(request);

    // ожидаем завершения запроса
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    // выводим результат запроса
    //qDebug() << reply->readAll();

    responseData = reply->readAll();
    qDebug() << responseData;

    QString strReply = QString::fromUtf8(responseData);
    strReplyEdit = strReply;
       processJson(strReplyEdit);
}

QString Client::read_response() {
    return strReplyEdit;
}

void Client::processJson(const QString& jsonStr) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject data = doc.object();

    // Извлечение нужного поля
    QJsonValue needed_field = data.value("endDate");

    // Удаление остальных полей
    QStringList keys = data.keys();
    for (const QString &key : keys) {
        if (key != "endDate") {
            data.remove(key);
        }
    }

    // Сохранение нужного поля в динамический массив
    /*QJsonArray dynamic_array;
    dynamic_array.append(needed_field);

    // Вывод динамического массива
    qDebug() << dynamic_array;
    //qDebug()<<needed_field;*/

    //QStringList string_array;
    string_array.append(needed_field.toString());

    // Вывод динамического массива
    qDebug() << string_array;
}

QString Client::read_field() {
    return string_array.join("\n");
}
