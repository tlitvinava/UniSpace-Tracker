/*#include "mainwindow.h"
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::string str = client->getResponse("google.com");
    QString qstr = QString::fromStdString(str);
    this->textEdit->setText(qstr);

    int n = ui->textEdit->toPlainText().toInt();
    int reversedNumber = r.reverseNumber(n);
     ui->textEdit_2->setText(QString::number(reversedNumber));
}*/

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
    client = new Client("iis.bsuir.by", "/api/v1/schedule?studentGroup=353504");//ЕСЛИ ТЫ УБЕРЕШЬ ЭТУ СТРОКУ ВСЕ УЛЕТИТ К ЧЕРТЯМ СОБАЧЬИМ
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}

void MainWindow::on_pushButton_clicked()
{
    client->connect();

    //client->send_request();
    //std::string str = client->read_response();
    //std::cout << "Response: " << str << std::endl;  // Добавьте эту строку для отладки
    //QString qstr = QString::fromStdString(str);
    //this->textEdit->setText(qstr);

    //Client client;
    //QString result = client->connect();
    //this->textEdit->setText(result);
}
