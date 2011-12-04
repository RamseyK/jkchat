/**
   jkchat, GUIClient
   main.cpp
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

#include <QApplication>
#include "mainwindow.h"

#include <iostream>
#include "Client.h"

int main (int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow *mw = new MainWindow();
    int ret = 0;

    // Instantiate a Client object
    Client *cl = new Client(mw);
    mw->setClient(cl);

    mw->show();

    ret = app.exec();

    // LOOK INTO aboutToQuit() instead of waiting for app.exec() to return

    delete cl;
    delete mw;

    return ret;
}

