/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Dec 6 02:57:19 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPlainTextEdit *textOutput;
    QLabel *labelName;
    QPushButton *btnConnect;
    QListView *listUser;
    QPushButton *btnSend;
    QLabel *lblIP;
    QLabel *lblPort;
    QLineEdit *textName;
    QLineEdit *textIP;
    QLineEdit *textPort;
    QLineEdit *textChat;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 529);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        textOutput = new QPlainTextEdit(centralwidget);
        textOutput->setObjectName(QString::fromUtf8("textOutput"));
        textOutput->setGeometry(QRect(10, 50, 581, 401));
        textOutput->setReadOnly(true);
        labelName = new QLabel(centralwidget);
        labelName->setObjectName(QString::fromUtf8("labelName"));
        labelName->setGeometry(QRect(10, 20, 51, 16));
        btnConnect = new QPushButton(centralwidget);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(630, 10, 114, 32));
        listUser = new QListView(centralwidget);
        listUser->setObjectName(QString::fromUtf8("listUser"));
        listUser->setGeometry(QRect(600, 50, 191, 401));
        listUser->setEditTriggers(QAbstractItemView::NoEditTriggers);
        btnSend = new QPushButton(centralwidget);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));
        btnSend->setGeometry(QRect(640, 460, 114, 32));
        lblIP = new QLabel(centralwidget);
        lblIP->setObjectName(QString::fromUtf8("lblIP"));
        lblIP->setGeometry(QRect(330, 20, 16, 16));
        lblPort = new QLabel(centralwidget);
        lblPort->setObjectName(QString::fromUtf8("lblPort"));
        lblPort->setGeometry(QRect(480, 20, 31, 16));
        textName = new QLineEdit(centralwidget);
        textName->setObjectName(QString::fromUtf8("textName"));
        textName->setGeometry(QRect(60, 20, 113, 22));
        textIP = new QLineEdit(centralwidget);
        textIP->setObjectName(QString::fromUtf8("textIP"));
        textIP->setGeometry(QRect(350, 20, 113, 22));
        textPort = new QLineEdit(centralwidget);
        textPort->setObjectName(QString::fromUtf8("textPort"));
        textPort->setGeometry(QRect(510, 20, 71, 22));
        textChat = new QLineEdit(centralwidget);
        textChat->setObjectName(QString::fromUtf8("textChat"));
        textChat->setGeometry(QRect(10, 460, 581, 21));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        labelName->setText(QApplication::translate("MainWindow", "Name:", 0, QApplication::UnicodeUTF8));
        btnConnect->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
        btnSend->setText(QApplication::translate("MainWindow", "Send", 0, QApplication::UnicodeUTF8));
        lblIP->setText(QApplication::translate("MainWindow", "IP:", 0, QApplication::UnicodeUTF8));
        lblPort->setText(QApplication::translate("MainWindow", "Port:", 0, QApplication::UnicodeUTF8));
        textIP->setText(QApplication::translate("MainWindow", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        textPort->setText(QApplication::translate("MainWindow", "27000", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
