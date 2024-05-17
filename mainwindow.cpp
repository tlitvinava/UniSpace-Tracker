#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <iostream>
#include<QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //textEdit = new QTextEdit(this); // Инициализация textEdit
    //textEdit->setGeometry(QRect(QPoint(100, 200), QSize(500, 300)));
    //client = new Client("iis.bsuir.by", "/api/v1/schedule?studentGroup=353504");//ЕСЛИ ТЫ УБЕРЕШЬ ЭТУ СТРОКУ ВСЕ УЛЕТИТ
    client = new Client("iis.bsuir.by", "/api/v1/auditories");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}


void MainWindow::on_startButton_clicked()
{
    client->connect();
    client->connect_groups();
    client->connect_group_schedule();
    client->createFinalSchedule();
}


void MainWindow::on_findButton_clicked() {
    QString auditoriumNumber = ui->lineEdit->text();
    QDate date = ui->dateEdit->date();

    // Проверка на воскресенье
    if (date.dayOfWeek() == 7) {
        this->ui->textEdit->setText("Университет не работает в воскресенье");
        return;
    }

    QString schedule = client->findScheduleTime(client->finalSchedule, auditoriumNumber, date);
    ui->textEdit->setText(schedule.isEmpty() ? "Расписание не найдено" : schedule);
}

