#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <iostream>

#include "Client.h"

namespace Ui {
    class MainWindow;
}

class Client;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Client *cl;
    QTimer *loopTimer;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void printLine(std::string str);
    void setClient(Client *c);

private slots:
    void on_btnConnect_clicked();

    void loopTimeout();

    void on_btnSend_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
