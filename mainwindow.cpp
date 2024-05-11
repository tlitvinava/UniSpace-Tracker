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
    client = new Client("iis.bsuir.by", "/api/v1/schedule?studentGroup=353504");//ЕСЛИ ТЫ УБЕРЕШЬ ЭТУ СТРОКУ ВСЕ УЛЕТИТ К ЧЕРТЯМ СОБАЧЬИМ
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}


void MainWindow::on_pushButton_2_clicked()
{
    client->connect();
    this->textEdit->setText(client->read_field());
}

