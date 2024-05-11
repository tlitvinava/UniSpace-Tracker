/*#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QLabel>
#include <QTextEdit>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Client* client = new Client();

private slots:
    //void on_label_linkActivated(const QString &link);

    //void on_textEdit_copyAvailable(bool b);

    //void on_pushButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    //QLabel *label;
    QTextEdit *textEdit;

};
#endif // MAINWINDOW_H
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Client *client;
    QTextEdit *textEdit;

};

#endif // MAINWINDOW_H
