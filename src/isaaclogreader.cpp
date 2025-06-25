#include "headers/isaaclogreader.h"
#include <QDebug>

IsaacLogReader::IsaacLogReader(QObject *parent): QThread(parent) {}

IsaacLogReader::~IsaacLogReader(){
    requestInterruption();
}

void IsaacLogReader::run(){
    connect(reader, &QFileSystemWatcher::fileChanged, this, &IsaacLogReader::readNewData);
}

void IsaacLogReader::readNewData(const QString &path){
    if (paused) { return; }
    QFile file(path);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if(logSize > file.size()){
            emit clearLog();
        }
        const int bufferSize = 4096;
        QByteArray buffer;

        QString text;
        file.seek(lastLogPosition);
        while (!file.atEnd() && !isInterruptionRequested()) {
            buffer = file.read(bufferSize);
            QString chunkText(buffer);
            text += chunkText;
        }
        file.close();
        logSize = file.size();
        if(!isInterruptionRequested()){
            emit logRead(text);
        }
    }
}

void IsaacLogReader::changeLogFile(QString path){
    for (QString oldPath : reader->files()){
        reader->removePath(oldPath);
    }
    reader->addPath(path);
    lastLogPosition = 0;
    emit clearLog();
    paused = false;
    readNewData(path);
}

void IsaacLogReader::pause(){
    requestInterruption();
    paused = true;
}

void IsaacLogReader::resume(){
    paused = false;
}
