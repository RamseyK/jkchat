/**
   jkchat, GUIClient
   mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
  * MainWindow Constructor
  * Sets up the ui, client process timer, user list
  *
  * @param parent Owner widget
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup the client process loop timer
    loopTimer = new QTimer(this);

    // Instantiate the model for the userList that will be accepted by the ListView
    userListModel = new QStringListModel();
    ui->listUser->setModel(userListModel);
}

/**
  * MainWindow Deconstructor
  *
  */
MainWindow::~MainWindow()
{
    loopTimer->stop();
    delete loopTimer;
    delete userListModel;
    delete ui;

}

/**
 * Print Line
 * Takes a standard string and outputs it to the main textOutput box. Called mostly by the client object
 *
 * @param str Standard string to output
 */
void MainWindow::printLine(std::string str) {
    QString qstr(str.c_str());
    ui->textOutput->appendPlainText(qstr);
}

/**
 * Set Client
 * Set's the MainWindow's Client object pointer and connect the loopTimer to the main networking loop function
 *
 * @param c Pointer to instanced client function for the window to use
 */
void MainWindow::setClient(Client *c) {
    cl = c;
    connect(loopTimer, SIGNAL(timeout()), this, SLOT(loopTimeout()));
}

/**
  * Update User
  * Updates the list in the GUI, called from the client's request handler when a ListPacket is received
  *
  * @param list vector containing the users connected
  */
void MainWindow::updateUserList(std::vector<std::string> list) {
    userStringList.clear();
    for(unsigned int i = 0; i<list.size(); i++) {
        userStringList.append(QString(list[i].c_str()));
    }
    userListModel->setStringList(userStringList);
}

/**
 * Loop Timeout
 * When the main timer loop times out (any time there is an idle moment) call the client process function
 * This is basically the main loop for networking operations
 */
void MainWindow::loopTimeout() {
    if(cl->isClientRunning())
        cl->clientProcess();
}

/**
 * Connect/Disconnect Button Clicked
 * When the connect button is clicked, this function is called.
 * Checks the state (connected/disconnected) and initiates or termiates a connection.
 * Starts / Stops the main loop timer and toggles the buttons functionality
 */
void MainWindow::on_btnConnect_clicked()
{
    std::string name = ui->textName->text().toStdString();
    cl->setName(name);

    // If not connected, Attempt to connect to the server
    if(!cl->isClientRunning()) {
        // Check if the entered username is empty
        if(name.empty()) {
            printLine("Please enter a username before connecting to the server");
            return;
        }

        // Attempt a connection to the IP and Port specified by the user
        if(!cl->attemptConnect(ui->textIP->text().toStdString(), ui->textPort->text().toInt()))
            return;

        // Start clientProcess timer
        loopTimer->start();

        cl->setClientRunning(true);

        // Toggle Connect button to disconnect
        ui->btnConnect->setText("Disconnect");
    } else {
        // Stop clientProcess timer
        loopTimer->stop();

        // Disconnect from network
        cl->disconnect();
        cl->setClientRunning(false);

        // Clear the user list
        userStringList.clear();
        userListModel->setStringList(userStringList);

        // Toggle button to Connect
        ui->btnConnect->setText("Connect");
    }
}

/**
 * Send Button Clicked
 * Takes the user input in textChat and sends it to the server
 */
void MainWindow::on_btnSend_clicked()
{
    // Get the string representation of the user's input in textChat and send it to the server
    string msg = ui->textChat->text().toStdString();
    cl->sendChatMsg(msg);

    // Clear the textChat box
    ui->textChat->clear();
}

/**
  * Enter Key Pressed
  * Simulates a send button click when pressing enter in textChat
  */
void MainWindow::on_textChat_returnPressed()
{
    on_btnSend_clicked();
}
