#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apimanager.h"
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QProgressDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int width = 850; // ширина в пикселях
    int height = 650;
    QPixmap bkgnd("/home/taisa/Изображения/photo_2024-06-03_00-42-19.jpg");

    bkgnd = bkgnd.scaled(width, height, Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    apimanager = new ApiManager("iis.bsuir.by", "/api/v1/auditories");
    apimanager->apimanager=apimanager;

    on_startButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete apimanager;
}

void MainWindow::on_startButton_clicked()
{
    qDebug()<<"lrglsorjhdlnvd;nb";
    apimanager->connect_groups();
    apimanager->connect_group_schedule();
    apimanager->createFinalSchedule();
}

void MainWindow::on_findButton_clicked() {
    qDebug()<<"lrglsorjhdlnvd;nb";
    QString auditoriumNumber = ui->lineEdit->text();
    QDate date = ui->dateEdit->date();

    // Проверка на воскресенье
    if (date.dayOfWeek() == 7) {
        this->ui->textEdit->setText("Университет не работает в воскресенье");
        return;
    }

    QString check = apimanager->checkDate(date);
    if (!check.isEmpty()) {
        ui->textEdit->setText(check);
        return;
    }

    QStringList freeTimeSlots = apimanager->findFreeTimeSlots(apimanager->finalSchedule, auditoriumNumber, date);

    QString result = freeTimeSlots.isEmpty() ? "Свободное время не найдено" : freeTimeSlots.join("\n");

    ui->textEdit->setText(result);
}

