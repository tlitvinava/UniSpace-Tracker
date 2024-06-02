#ifndef APIMANAGER_H
#define APIMANAGER_H

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
#include <QMutex>
#include <QMutexLocker>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

class ApiManager{
public:
    ApiManager(const std::string& server, const std::string& path);

    //QMap<QString, QSet<std::pair<QTime, QTime>>> finalSchedule;

    QSet<QString> uniqueSchedules;

    QMap<QString, QSet<QString>> finalSchedule;
    QMap<QString, QStringList> daySchedules;
    QNetworkReply *reply;
    void connect();
    QString read_response();
    //void processJson(const QString& jsonStr);
    QString read_field();
    void onLineEditTextChanged(const QString& text);
    void connect_groups();
    QString read_response_groups();
    void processJsonGroup(const QString& jsonStr);
    QString read_group();
    void connect_group_schedule();
    QStringList read_group_schedule();
    void processGroupSchedule(const QString& jsonStr){
         QThread::sleep(1);
    }
    void processDayObject(const QJsonObject& dayObject, const QString& day);
    QStringList read_auditoriums();
    QString findScheduleAsString(const QString& auditoriumNumber, const QDate& date);
    void createFinalSchedule();
    QStringList findTimeSlots(const QMap<QString, QSet<QString>>& scheduleMap, const QString& auditorium, const QDate& date);
    QString formatTimeSlots(const QStringList& timeSlots);
    QString searchSchedule(const QString& auditoriumNumber, const QDate& date);
    QString getDayOfWeekString(const QDate& date);
    QString findScheduleTime(const QMap<QString, QSet<QString>>& finalSchedule, const QString& auditorium, const QDate& date);
    QStringList findFreeTimeSlots(const QMap<QString, QSet<QString>>& finalSchedule, const QString& auditorium, const QDate& date);
    //QStringList findFreeTimeSlots(const QMap<QString, QSet<QPair<QTime, QTime>>>& finalSchedule, const QString& auditorium, const QDate& date);
    static bool timeCompare(const QPair<QTime, QTime>& a, const QPair<QTime, QTime>& b);




    //void searchSchedule();

private:

    //QMap<QString, QStringList> daySchedules;
    QMutex mutex;
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

signals:
    void progressUpdated(int value);
    void finished();

public slots:
    void startProcess();

};

#endif // APIMANAGER_H

