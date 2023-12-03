#include "filemonitor.h"
#include <QFile>
#include <QTextStream>

void FileMonitor::stopTimer()
{
    m_timer->stop();
}

void FileMonitor::doMonitor()
{
    if(m_logOption == 1){
        monitorLog(false);
    }else if(m_logOption == 2){
            monitorOption(m_filename);
        }
}

void FileMonitor::monitorLog(bool force = false)
{
    QFile file(m_filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        qint64 currentSize = file.size();
        if (currentSize != m_previousSize || force)
        {
            QTextStream in(&file);
            QString content = in.readAll();

            QStringList lines = content.split("\n");

            for (int i = 0; i < lines.size(); ++i) {
                lines[i] = QString::number(i + 1) + ": " + lines[i];
            }

            QString numberedContent = lines.join("\n");
            emit logLoaded(numberedContent, force);
            m_previousSize = currentSize;
        }

        file.close();
    }
}

void FileMonitor::monitorOption(QString configDir)
{
    emit optionLoaded(configDir);
}
