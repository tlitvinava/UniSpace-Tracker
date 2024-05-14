#include "client.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/ssl.hpp>

Client::Client(const std::string& server, const std::string& path) : server_(server), path_(path), io_service_(), context_(boost::asio::ssl::context::sslv23), socket_(io_service_, context_) {

}

/*void Client::connect() {//ПОЛУЧЕНИЕ ВСЕХ АУДИТОРИЙ

    request.setUrl(QUrl("https://iis.bsuir.by/api/v1/auditories"));

    // отправляем GET-запрос и получаем ответ
    reply = manager.get(request);

    // ожидаем завершения запроса
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    responseData = reply->readAll();
    //qDebug() << responseData;

    QString strReply = QString::fromUtf8(responseData);
    strReplyEdit = strReply;
    processJson(strReplyEdit);
}

void Client::processJson(const QString& jsonStr) {//СОРТИРОВКА АУДИТОРИЙ ПО КОРПУСАМ
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonArray array = doc.array();

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        QJsonValue name_field = object.value("name");
        QJsonValue building_field = object["buildingNumber"].toObject().value("name");

        buildingRooms[building_field.toString()].append(name_field.toString());
    }

    QMapIterator<QString, QStringList> i(buildingRooms);
    while (i.hasNext()) {
        i.next();
        qDebug() << "Building: " << i.key() << ", Rooms: " << i.value().join(", ");
    }
}*/

void Client::connect_groups() {//ПОЛУЧЕНИЕ ВСЕХ ГРУПП В УНИВЕРЕ
    //request.setUrl(QUrl("https://iis.bsuir.by/api/v1/schedule?studentGroup=353504"));
    request.setUrl(QUrl("https://iis.bsuir.by/api/v1/student-groups"));

    // отправляем GET-запрос и получаем ответ
    reply = manager.get(request);

    // ожидаем завершения запроса
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    responseData = reply->readAll();
    //qDebug() << responseData;

    QString strReply = QString::fromUtf8(responseData);
    strReplyEditGroup = strReply;
    processJsonGroup(strReplyEditGroup);
}

void Client::processJsonGroup(const QString& jsonStr) {//ПОЛУЧЕНИЕ ПОЛЯ НОМЕР ГРУППЫ И СОХРАНЕНИЕ В МАССИВ
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonArray array = doc.array();

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        QJsonValue needed_field = object.value("name");
        string_array_group.append(needed_field.toString());
    }
}

void Client::connect_group_schedule() {//ПОЛУЧЕНИЕ РАСПИСАНИЯ КАЖДОЙ ГРУППЫ

    // Проходим по всем группам в массиве string_array_group
    for (const QString& groupNumber : string_array_group) {
        // Формируем URL для запроса, добавляя номер группы к базовому URL
        QString url = QString("https://iis.bsuir.by/api/v1/schedule?studentGroup=%1").arg(groupNumber);
        request.setUrl(QUrl(url));

        // Отправляем GET-запрос и получаем ответ
        reply = manager.get(request);

        // Ожидаем завершения запроса
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        // Проверяем, была ли ошибка в запросе
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Ошибка: " << reply->errorString();
            continue;
        }

        // Получаем данные ответа
        responseData = reply->readAll();

        // Сохраняем данные ответа в отдельный массив
        processGroupSchedule(QString::fromUtf8(responseData));
    }
}

void Client::processGroupSchedule(const QString& jsonStr) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject object = doc.object();
    QJsonValue schedules_field = object.value("schedules");

    if (schedules_field.isObject()) {
        QJsonObject schedules_object = schedules_field.toObject();
        //QMap<QString, QStringList> daySchedules;
        QStringList daysOfWeek = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

        for (const QString& day : daysOfWeek) {
            QJsonValue day_field = schedules_object.value(day);
            if (day_field.isArray()) {
                QJsonArray day_array = day_field.toArray();

                for (const QJsonValue& dayValue : day_array) {
                    if (dayValue.isObject()) {
                        QJsonObject dayObject = dayValue.toObject();
                        QString auditorium = dayObject.value("auditories").toArray().first().toString();
                        QString startLessonTime = QTime::fromString(dayObject.value("startLessonTime").toString(), "HH:mm").toString("HH:mm:ss");
                        QString endLessonTime = QTime::fromString(dayObject.value("endLessonTime").toString(), "HH:mm").toString("HH:mm:ss");
                        QJsonArray weekNumberArray = dayObject.value("weekNumber").toArray();
                        QStringList weekNumbers;
                        for (const QJsonValue& value : weekNumberArray) {
                            weekNumbers.append(QString::number(value.toInt()));
                        }
                        QString weekNumbersString = weekNumbers.join(", ");

                        QString scheduleString = QString("\"%1\"+\"%2\"+\"%3\"+\"%4\"")
                                                     .arg(auditorium)
                                                     .arg(startLessonTime)
                                                     .arg(endLessonTime)
                                                     .arg(weekNumbersString);
                        daySchedules[day].append(scheduleString);
                    }
                }
            }
        }

        for (const QString& day : daysOfWeek) {
            if (!daySchedules[day].isEmpty()) {
                qDebug() << day << " :";
                for (const QString& schedule : daySchedules[day]) {
                    qDebug() << "    " << schedule;
                }
            }
        }
    }
}

QString Client::findScheduleAsString(const QString& auditoriumNumber, const QDate& date) {
    QString result;
    QString dayOfWeek = QLocale(QLocale::Russian).dayName(date.dayOfWeek());
    QStringList schedulesForDay = this->daySchedules[dayOfWeek];

    for (const QString& schedule : schedulesForDay) {
        if (schedule.contains(auditoriumNumber)) {
            result = schedule;
            break; // Выходим из цикла после нахождения первого совпадения
        }
    }
    return result; // Возвращаем найденное расписание как строку
}


QString Client::read_response() {//ВЫВОД АУДИТОРИЙ В ОКНО
    return strReplyEdit;
}

QString Client::read_field() {//ВЫВОД ЧЕГО ТО ТАМ
    return string_array.join("\n");
}

void Client::onLineEditTextChanged(const QString& text) {//СВЯЗЫВАНИЕ КОРПУСА ИЗ ВИДЖЕТА С МАССИВОМ АУДИТОРИЙ ПО КОРПУСАМ
    if (buildingRooms.contains(text)) { // Проверка, есть ли такой корпус в QMap
        QStringList rooms = buildingRooms[text]; // Получение списка аудиторий для данного корпуса
        qDebug() << "Building: " << text << ", Rooms: " << rooms.join(", ");
    } else {
        qDebug() << "Building: " << text << " does not exist.";
    }
}


QString Client::read_response_groups() {//ВЫВОД ГРУПП В ОКНО
    return strReplyEditGroup;
}


QString Client::read_group() {//ВЫВОД НОМЕРОВ ГРУПП В ОКНО
    return string_array_group.join("\n");
}


QStringList Client::read_group_schedule() {//ВЫВОД МАССИВА С ДАННЫМИ РАСПИСАНИЯ ДЛЯ КАЖДОЙ ГРУППЫ
    // Возвращаем массив с данными расписания для каждой группы
    return groupScheduleData;
}

QStringList Client::read_auditoriums() {
    // Возвращаем QStringList с аудиториями
    return auditoriums;
}

/*Неделя:
    День недели:
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
    День недели:
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
        "Аудитория"+"Начало занятий"+"Конец занятий"+"Недели"
*/


