#ifndef CUSTOMERRORHANDLER_H
#define CUSTOMERRORHANDLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>

class CustomErrorHandler
{
public:
    static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        QString logMessage;

        switch (type) {
        /*case QtInfoMsg:
            logMessage = QString("Info: %1").arg(msg);
            break;
        case QtWarningMsg:
            logMessage = QString("Warning: %1").arg(msg);
            break;*/
        case QtCriticalMsg:
            logMessage = QString("Critical: %1").arg(msg);
            // Show the critical error message in a pop-up dialog
            QMessageBox::critical(nullptr, "Critical Error", logMessage);
            break;
        case QtFatalMsg:
            logMessage = QString("Fatal: %1").arg(msg);
            // Show the fatal error message in a pop-up dialog
            QMessageBox::critical(nullptr, "Fatal Error", logMessage);
            break;
        }
        // Log the message to a file
        QString fileName = "ICError.log";
#ifdef Q_OS_LINUX
        fileName = QString(QDir::homePath() + "/.local/share/" + fileName);
#endif
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream out(&file);
            out << logMessage << "\n";
            file.close();
        }
    }
};

#endif // CUSTOMERRORHANDLER_H
