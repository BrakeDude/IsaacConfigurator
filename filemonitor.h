#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include <QObject>
#include <QTimer>
#include <QTextCursor>

class FileMonitor : public QObject
{
    Q_OBJECT

public:
    explicit FileMonitor(const QString& filename, int logOrOption, QObject* parent = nullptr)
        : QObject(parent), m_filename(filename), m_logOption(logOrOption), m_previousSize(0)
    {
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(doMonitor()));
        m_timer->start(1000);
    }

public slots:

    void stopTimer();

    void monitorLog(bool force);

    void monitorOption(QString configDir);

    void doMonitor();

signals:
    void logLoaded(const QString& content, const bool& force);
    void optionLoaded(const QString& configDir);

private:
    QString m_filename;
    qint64 m_previousSize;
    int m_logOption;
    QTimer* m_timer;
};

#endif // FILEMONITOR_H
