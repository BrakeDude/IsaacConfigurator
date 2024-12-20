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
#include <QFileDialog>
#include "ui_about.h"
#include "qtablewidgetcheckbox.h"
#include "filemonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_modRadioButton_Name_toggled(bool checked);

    void on_modRadioButton_Folder_toggled(bool checked);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionAbout_triggered();

    void onFileLoaded(QString text, bool force);

    void ReSyncConfigSlot(QString configDir);

    void ReSyncConfigIniSlot(QString configDir);

private:
    QString optionMessage1;
    QString optionMessage2;
    QStringList modTableHeaders;
    QString openDirName;
    QString modMessage1;
    QString modMessage2;
    QString gameMessage1;
    QString gameMessage2;
    QString jsonFilterText;
    QString saveFileText;
    QString openFileText;
    FileMonitor* logMonitor;
    FileMonitor* optionMonitor;
    QString currentDLCName;

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
    void ConnectVanillaOptions(QSettings *settings);
    void ConnectRepentogonOptions(QSettings *settings);
    void SyncConfigFile(QSettings *settings, bool extra);
    void ReSyncConfig(QString confDir);
    void LoadApp(QString FullDir, QString gameExe);
    void UpdateApp(QString FullDir, QString gameExe);
    void SortLineEdit();
    void ReSyncConfigIni(QSettings *settings);
    void ConfigIniLoad();
    void DarkMode(bool dark);
    QString getModPath();
    QString GetSteamPath();
    QString IsaacDLC(QString directory);
    QString GetExeName();
    QString GetFullDir();

};

#endif // MAINWINDOW_H
