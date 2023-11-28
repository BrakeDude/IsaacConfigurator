#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QCheckBox>
#include <QSettings>
#include <QResizeEvent>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QTextStream>
#include <QTextBrowser>
#include <QThread>
#include "ui_about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableWidgetCheckBox : public QTableWidgetItem
{
public:
    bool operator<(const QTableWidgetItem &other) const override
    {
        if (other.column() == 0) {
            // compare checkboxes by their state
            QCheckBox *checkbox1 = dynamic_cast<QCheckBox *>(tableWidget()->cellWidget(row(), column()));
            QCheckBox *checkbox2 = dynamic_cast<QCheckBox *>(tableWidget()->cellWidget(other.row(), other.column()));
            return checkbox1->isChecked() < checkbox2->isChecked();
        } else {
            // compare other items normally
            return QTableWidgetItem::operator<(other);
        }
    }
};

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

    void stopTimer()
    {
        m_timer->stop();
    }

    void monitorFile()
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

signals:
    void fileLoaded(const QString& content);

private:
    QString m_filename;
    qint64 m_previousSize;
    QTimer* m_timer;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_modRadioButton_Name_toggled(bool checked);

    void on_modRadioButton_Folder_toggled(bool checked);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionAbout_triggered();

private:
    QString optionMessage1;
    QString optionMessage2;
    QStringList modTableHeaders;
    QString openDirName;
    QString modMessage1;
    QString modMessage2;
    QString gameMessage1;
    QString gameMessage2;
    FileMonitor* fileMonitor;
    QThread* monitorThread = new QThread(this);

    Ui::MainWindow *ui;
    Ui::about *ui_about;
    QDialog *aboutDialog;
    QString configDir;
    QString currentTranslator;
    void initLanguages(QString translation);
    void retranslateStrings(QString translator);
    void loadMods(QString directory);
    void SyncMods(QString directory);
    void LoadConfigFile();
    void LoadConfig(QString confDir);
    void SyncConfigFile(QSettings *settings);
    void ReSyncConfig(QString confDir);
    void LoadApp(QString FullDir, QString gameExe);
    void UpdateApp(QString FullDir, QString gameExe);
    void SortLineEdit();
    void ReSyncConfigIni(QSettings *settings);
    void ConfigIniLoad();
    QString getModPath();
    QString IsaacDLC(QString directory);
    QString GetExeName();
    QString GetFullDir();

};

#endif // MAINWINDOW_H
