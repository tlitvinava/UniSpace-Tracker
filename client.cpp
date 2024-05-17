#include "client.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/ssl.hpp>

static QStringList daysOfWeek = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};


Client::Client(const std::string& server, const std::string& path) : server_(server), path_(path), io_service_(), context_(boost::asio::ssl::context::sslv23), socket_(io_service_, context_) {

}

void Client::connect() {//ПОЛУЧЕНИЕ ВСЕХ АУДИТОРИЙ

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
    //processJson(strReplyEdit);
    qDebug()<<"Получен ответ на список аудиторий";
}

/*void Client::processJson(const QString& jsonStr) {//СОРТИРОВКА АУДИТОРИЙ ПО КОРПУСАМ
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
        //qDebug() << "Building: " << i.key() << ", Rooms: " << i.value().join(", ");
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

void Client::processJsonGroup(const QString& jsonStr) {// ПОЛУЧЕНИЕ ПОЛЯ НОМЕР ГРУППЫ И СОХРАНЕНИЕ В МАССИВ
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonArray array = doc.array();

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        QJsonValue needed_field = object.value("name");
        QString groupNumber = needed_field.toString();
        // Проверка, что номер группы не начинается с нуля
        if (!groupNumber.startsWith('0')) {
            string_array_group.append(groupNumber);
        }
    }
    qDebug()<<"Группа сохранена в массив";

}

void Client::connect_group_schedule() {//ПОЛУЧЕНИЕ РАСПИСАНИЯ КАЖДОЙ ГРУППЫ

    // Проходим по всем группам в массиве string_array_group
    for (const QString& groupNumber : string_array_group) {
        // Формируем URL для запроса, добавляя номер группы к базовому URL
        QString url = QString("https://iis.bsuir.by/api/v1/schedule?studentGroup=%1").arg(groupNumber);
        request.setUrl(QUrl(url));

        qDebug() << "Начало:" << QDateTime::currentDateTime().toMSecsSinceEpoch();

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

        qDebug() << "Конец_:" << QDateTime::currentDateTime().toMSecsSinceEpoch();

        qDebug()<<"Получено расписание группы";


        // Сохраняем данные ответа в отдельный массив
        processGroupSchedule(QString::fromUtf8(responseData));
        qDebug()<<"Получены поля из расписания группы";
    }
    qDebug()<<"Получен список, сортирующийся по дням недели";

}

void Client::processGroupSchedule(const QString& jsonStr) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonObject object = doc.object();
    QJsonValue schedules_field = object.value("schedules");

    if (schedules_field.isObject()) {
        QJsonObject schedules_object = schedules_field.toObject();
        //QMap<QString, QStringList> daySchedules;
        QSet<QString> uniqueSchedules; // Добавлено для проверки уникальности
        //QStringList daysOfWeek = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

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

                        /*QString scheduleString = QString("\"%1\"+\"%2\"+\"%3\"+\"%4\"")
                                                     .arg(auditorium)
                                                     .arg(startLessonTime)
                                                     .arg(endLessonTime)
                                                     .arg(weekNumbersString);*/

                        QString scheduleString = auditorium + "+" + startLessonTime + "+" + endLessonTime + "+" + weekNumbersString;
                        // Проверка на уникальность перед добавлением
                        if (!uniqueSchedules.contains(scheduleString)) {//mutex
                            uniqueSchedules.insert(scheduleString);
                            daySchedules[day].append(scheduleString);
                        }
                    }
                }
            }
        }

        /*for (const QString& day : daysOfWeek) {
            if (!daySchedules[day].isEmpty()) {
                qDebug() << day << " :";
                for (const QString& schedule : daySchedules[day]) {
                    qDebug() << "    " << schedule;
                }
            }
        }*/
    }
}

void Client::createFinalSchedule() {//долго сортирует
    //QMap<QString, QSet<QString>> finalSchedule;

    for (const QString& day : daysOfWeek) {
        for (const QString& schedule : daySchedules[day]) {
            QStringList scheduleDetails = schedule.split("+");
            QString auditorium = scheduleDetails.at(0);
            QString startLessonTime = scheduleDetails.at(1);
            QString endLessonTime = scheduleDetails.at(2);
            QString timeSlot = startLessonTime + "+" + endLessonTime;
            QString key = auditorium + "+" + day;//////////////////////////////////////////////////////////////////////////////////

            // Проверка на уникальность временного слота перед добавлением
            if (!finalSchedule[key].contains(timeSlot)) {
                finalSchedule[key].insert(timeSlot);
            }
        }
    }

    // Вывод итогового расписания
    qDebug() << "Аудитория\\День | " + daysOfWeek.join(" | ");
    for (const QString& key : finalSchedule.keys()) {
        QString line = key + " | ";
        for (const QString& day : daysOfWeek) {
                QString dayKey = key.split("+").at(0) + "+" + day;
            if (finalSchedule.contains(dayKey)) {
                line += QStringList(finalSchedule[dayKey].begin(), finalSchedule[dayKey].end()).join(", ") + " | ";
            } else {
                line += " - | ";
            }
        }
        qDebug() << line;
    }
    qDebug()<<"Создана финальная карта";

}

QString Client::getDayOfWeekString(const QDate& date) {
    QMap<int, QString> daysOfWeek = {
        {1, "Понедельник"},
        {2, "Вторник"},
        {3, "Среда"},
        {4, "Четверг"},
        {5, "Пятница"},
        {6, "Суббота"},
        {7, "Воскресенье"}
    };
    return daysOfWeek[date.dayOfWeek()];
}

QString Client::findScheduleTime(const QMap<QString, QSet<QString>>& finalSchedule, const QString& auditorium, const QDate& date) {
    QString dayOfWeek = getDayOfWeekString(date);

    // Проверка, является ли день воскресеньем
    if (dayOfWeek == "Воскресенье") {
        qDebug() << "Университет не работает в воскресенье";
        return ""; // Возвращаем пустую строку, чтобы не использовать это значение в дальнейшем коде
    }

    QString key = auditorium + "+" + dayOfWeek;
    if (finalSchedule.contains(key)) {
        return QStringList(finalSchedule[key].begin(), finalSchedule[key].end()).join(", ");
    }
    return "Расписание не найдено";
}



// Функция для определения дня недели по дате
/*QString getDayOfWeek(const QDate& date) {
    switch (date.dayOfWeek()) {
    case 1: return "Понедельник";
    case 2: return "Вторник";
    case 3: return "Среда";
    case 4: return "Четверг";
    case 5: return "Пятница";
    case 6: return "Суббота";
    case 7: return "Воскресенье";
    default: return "";
    }
}*/

// Функция для поиска временных слотов в карте
/*QStringList findTimeSlots(const QMap<QString, QSet<QString>>& scheduleMap, const QString& auditorium, const QDate& date) {
    QString dayOfWeek = getDayOfWeek(date);
    QString key = auditorium + "+" + dayOfWeek;
    if (scheduleMap.contains(key)) {
        return QStringList(scheduleMap[key].begin(), scheduleMap[key].end());
    }
    return QStringList();
}*/

// Функция для форматирования времени в строку
QString formatTimeSlots(const QStringList& timeSlots) {
    return timeSlots.join(", ");
}

/*QString Client::searchSchedule(const QString& auditoriumNumber, const QDate& date) {////////////////////////////////////////////////

    switch (date.dayOfWeek()) {
    case 1: return "Понедельник";
    case 2: return "Вторник";
    case 3: return "Среда";
    case 4: return "Четверг";
    case 5: return "Пятница";
    case 6: return "Суббота";
    case 7: return "Воскресенье";
    default: return "";
    }

        //            QString key = auditorium + "+" + day;

        QString key_map = finalSchedule.key(auditoriumNumber "+" +date);
        QList<QString> valuesList = finalSchedule.values(); // get a list of all the values


}*/

// Пример использования функций
/*void Client::searchSchedule() {
    QString auditorium;
    QDate date;

    // Предположим, что пользователь ввел данные
    qDebug() << "Введите номер аудитории:";
    // ... пользователь вводит номер аудитории ...
    qDebug() << "Введите дату (гггг-мм-дд):";
    // ... пользователь вводит дату ...

    // Вызов функции поиска
    QStringList timeSlots = findTimeSlots(finalSchedule, auditorium, date);

    // Форматирование и вывод времени
    QString formattedTime = formatTimeSlots(timeSlots);
    qDebug() << "Временные слоты для аудитории " << auditorium << " на " << getDayOfWeek(date) << ": " << formattedTime;

    // Для вывода в виджет QT можно использовать QString напрямую
    // Например:
    // ui->timeSlotsLabel->setText(formattedTime);
}*/



/*void Client::processGroupSchedule(const QString& jsonStr) {
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
}*/


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

/*void Client::processJsonGroup(const QString& jsonStr) {//ПОЛУЧЕНИЕ ПОЛЯ НОМЕР ГРУППЫ И СОХРАНЕНИЕ В МАССИВ
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonArray array = doc.array();

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject object = array[i].toObject();
        QJsonValue needed_field = object.value("name");
        string_array_group.append(needed_field.toString());
    }
}*/


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

//"Аудитория"+"День недели"+"Начало занятий"+"Конец занятий"
//"Аудитория"+"День недели"+"Начало занятий"+"Конец занятий"
//"Аудитория"+"День недели"+"Начало занятий"+"Конец занятий"
//"Аудитория"+"День недели"+"Начало занятий"+"Конец занятий"
//"Аудитория"+"День недели"+"Начало занятий"+"Конец занятий"

