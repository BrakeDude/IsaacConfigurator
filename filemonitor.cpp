#include "filemonitor.h"
#include <QFile>
#include <QTextStream>

void FileMonitor::stopTimer()
{
    m_timer->stop();
}

void FileMonitor::doMonitor()
{
    monitorFile(false);
}

void FileMonitor::monitorFile(bool force = false)
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
            emit fileLoaded(numberedContent, force);
            m_previousSize = currentSize;
        }

        file.close();
    }
}
