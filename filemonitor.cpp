#include "filemonitor.h"
#include <QFile>
#include <QTextStream>

void FileMonitor::stopTimer()
{
    m_timer->stop();
}

void FileMonitor::monitorFile()
{
    QFile file(m_filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qint64 currentSize = file.size();
        if (currentSize != m_previousSize)
        {
            QTextStream in(&file);
            QString content = in.readAll();

            // Emit a signal with the loaded content
            emit fileLoaded(content);
            m_previousSize = currentSize;
        }

        file.close();
    }
}
