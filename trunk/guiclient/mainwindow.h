/**
   jkchat, GUIClient
   mainwindow.h
   Copyright 2011 Ramsey Kant, Keilan Jackson

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <iostream>
#include <QStringListModel>
#include <QSTringList>

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
    QStringListModel *userListModel;
    QStringList userStringList;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void printLine(std::string str);
    void setClient(Client *c);
    void updateUserList(std::vector<std::string> list);

private slots:
    void on_btnConnect_clicked();

    void loopTimeout();

    void on_btnSend_clicked();

    void on_textChat_returnPressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
