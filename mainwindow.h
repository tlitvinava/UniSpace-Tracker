#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "apimanager.h"
#include <QTextEdit>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTextEdit *textEdit;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    //void on_lineEdit_textChanged(const QString &arg1);

    //void on_dateEdit_userDateChanged(const QDate &date);

    void on_findButton_clicked();

private:
    Ui::MainWindow *ui;
    ApiManager *apimanager;
    //QTextEdit *textEdit;
};

#endif // MAINWINDOW_H
