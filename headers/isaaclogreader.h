#ifndef ISAACLOGREADER_H
#define ISAACLOGREADER_H

#include <QFileSystemWatcher>
#include <QThread>
#include <QFile>

class IsaacLogReader : public QThread
{
    Q_OBJECT
signals:
    void logRead(const QString &log);
    void clearLog();

private slots:
    void readNewData(const QString &path);

private:
    QFileSystemWatcher *reader = new QFileSystemWatcher();
    uint lastLogPosition = 0;
    uint logSize = 0;
    bool paused = false;

public:
    IsaacLogReader(QObject *parent = nullptr) ;
    ~IsaacLogReader();
    void run() override;
    void changeLogFile(QString path);
    void pause();
    void resume();
};

#endif // ISAACLOGREADER_H
