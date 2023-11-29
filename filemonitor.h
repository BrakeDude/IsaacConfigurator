#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include <QObject>
#include <QTimer>
#include <QTextCursor>

class FileMonitor : public QObject
{
    Q_OBJECT

public:
    explicit FileMonitor(const QString& filename, QObject* parent = nullptr)
        : QObject(parent), m_filename(filename), m_previousSize(0)
    {
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(monitorFile()));
        m_timer->start(1000);
    }

public slots:

    void stopTimer();

    void monitorFile();

signals:
    void fileLoaded(const QString& content);

private:
    QString m_filename;
    qint64 m_previousSize;
    QTimer* m_timer;
};

#endif // FILEMONITOR_H
