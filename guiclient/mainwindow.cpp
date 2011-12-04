#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup the client process loop timer
    loopTimer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    loopTimer->stop();
    delete loopTimer;
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
