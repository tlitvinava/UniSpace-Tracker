#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apimanager.h"
#include <iostream>
#include<QLabel>
#include <QPushButton>
#include <QProgressDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //textEdit = new QTextEdit(this); // Инициализация textEdit
    //textEdit->setGeometry(QRect(QPoint(100, 200), QSize(500, 300)));
    //client = new Client("iis.bsuir.by", "/api/v1/schedule?studentGroup=353504");//ЕСЛИ ТЫ УБЕРЕШЬ ЭТУ СТРОКУ ВСЕ УЛЕТИТ
    apimanager = new ApiManager("iis.bsuir.by", "/api/v1/auditories");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete apimanager;
}


    void MainWindow::on_startButton_clicked()
{
        /*QProgressDialog progressDialog(this);
        progressDialog.setLabelText(tr("Загрузка данных..."));
        progressDialog.setCancelButtonText(tr("Отмена"));
        progressDialog.setRange(0, 100);
        progressDialog.setAutoClose(true);

        connect(apimanager, &ApiManager::progressUpdated, &progressDialog, &QProgressDialog::setValue);

        progressDialog.show();*/

        apimanager->connect_groups();
        apimanager->connect_group_schedule();
        apimanager->createFinalSchedule();

        //connect(apimanager, &ApiManager::finished, &progressDialog, &QProgressDialog::accept);

    }

void MainWindow::on_findButton_clicked() {
    QString auditoriumNumber = ui->lineEdit->text();
    QDate date = ui->dateEdit->date();

    // Проверка на воскресенье
    if (date.dayOfWeek() == 7) {
        this->ui->textEdit->setText("Университет не работает в воскресенье");
        return;
    }

    // Получение списка свободных временных интервалов
    //QStringList freeTimeSlots = apimanager->findFreeTimeSlots(apimanager->finalSchedule, auditoriumNumber, date);
    QStringList freeTimeSlots = apimanager->findFreeTimeSlots(apimanager->finalSchedule, auditoriumNumber, date);

    // Форматирование результата для отображения
    QString result = freeTimeSlots.isEmpty() ? "Свободное время не найдено" : freeTimeSlots.join("\n");

    // Отображение результата в textEdit
    ui->textEdit->setText(result);
}


