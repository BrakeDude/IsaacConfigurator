#ifndef CUSTOMERRORHANDLER_H
#define CUSTOMERRORHANDLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

class CustomErrorHandler
{
public:
    static void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        QString logMessage;

        switch (type) {
        case QtDebugMsg:
            logMessage = QString("Debug: %1").arg(msg);
            break;
        case QtInfoMsg:
            logMessage = QString("Info: %1").arg(msg);
            break;
        case QtWarningMsg:
            logMessage = QString("Warning: %1").arg(msg);
            break;
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
        QFile file("error.log");
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream out(&file);
            out << logMessage << "\n";
        }
    }
};

#endif // CUSTOMERRORHANDLER_H
