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

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString optionMessage1;
    QString optionMessage2;
    QStringList modTableHeaders;
    QString openDirName;
    QString modMessage1;
    QString modMessage2;
    QString gameMessage1;
    QString gameMessage2;

    Ui::MainWindow *ui;
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
    QString getModPath();
    QString IsaacDLC(QString directory);
    QString GetExeName();
    QString GetFullDir();

};

#endif // MAINWINDOW_H
