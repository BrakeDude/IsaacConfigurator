#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>

void MainWindow::updateLogFile() {
    QFile file(configDir + "/log.txt");
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QScrollBar* scrollBar = ui->logBrowser->verticalScrollBar();
        int scrollPosition = scrollBar->value();
        QTextStream in(&file);
        QString fileContents = in.readAll();
        ui->logBrowser->setText(fileContents);
        file.close();
        scrollBar->setValue(scrollPosition);
    }
}
