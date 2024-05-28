#include "mainwindow.h"
#include "customerrorhandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(CustomErrorHandler::handleMessage);
    MainWindow w;
    w.show();
    return a.exec();
}
