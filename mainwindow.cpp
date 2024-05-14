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
    textEdit = new QTextEdit(this); // Инициализация textEdit
    textEdit->setGeometry(QRect(QPoint(100, 200), QSize(500, 300)));
    //client = new Client("iis.bsuir.by", "/api/v1/schedule?studentGroup=353504");//ЕСЛИ ТЫ УБЕРЕШЬ ЭТУ СТРОКУ ВСЕ УЛЕТИТ К ЧЕРТЯМ СОБАЧЬИМ
    client = new Client("iis.bsuir.by", "/api/v1/auditories");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}


void MainWindow::on_startButton_clicked()
{
    //client->connect();
    client->connect_groups();
    client->connect_group_schedule();
}

void MainWindow::on_findButton_clicked()
{
    // Получаем номер аудитории из lineEdit
    QString auditoriumNumber = ui->lineEdit->text();

    // Получаем выбранную дату из dateEdit
    QDate date = ui->dateEdit->date();

    // Вызываем функцию поиска расписания с полученными данными
    this->textEdit->setText(client->findScheduleAsString(auditoriumNumber, date));
}

