#include "mainwindow.h"
#include "customerrorhandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifndef QT_DEBUG
    qInstallMessageHandler(CustomErrorHandler::handleMessage);
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
