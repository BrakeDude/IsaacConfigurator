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
#include <QTextStream>
#include <QFileDialog>
#include <QThread>
#include "ui_about.h"
#include <QDebug>
#include <QFileSystemWatcher>
#include <QTextCursor>
#include <QMenu>

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

    void ReSyncConfig();

    void ReSyncConfigIniSlot();

    void ReSyncMods();

private:
    QString optionMessage1;
    QStringList modTableHeaders;
    QString openDirName;
    QString modMessage1;
    QString gameMessage1;
    QString fileFiterText = "(*.json *.txt);;(*.json);;(*.txt)";
    QString saveFileText;
    QString openFileText;
    QString setWineApp;
    QString wineApp1;
    QString wineApp2;
    QString winePrefix1;

    QThread *ConfigReader = new QThread();
    QThread *ConfigIniReader = new QThread();
    QThread *ModSyncer = new QThread();
    QStringList logLines;

    Ui::MainWindow *ui;
    Ui::about *ui_about;
    QDialog *aboutDialog;
    QString configDir;
    QString gameDir;
    QString gameExec = "isaac-ng.exe";
    QString gameStore = "Unknown";
    QString gameDLC = "Rebirth";
    QString currentTranslator;
    QString linuxWineApp = "";
    QString linuxWinePrefix = "";
    int currentIndex = -1;
    QList<int> matches;
    QMenu *recentFiles = new QMenu(tr("Recent"));

    void initLanguages(QString translation);
    void retranslateStrings(QString translator);
    void loadMods(QString directory);
    void SyncMods(QString directory, bool force, bool message = false);
    void FindConfigFile();
    void LoadConfig(bool disable = false);
    void ConnectVanillaOptions();
    void DisconnectVanillaOptions();
    void ConnectRepentogonOptions();
    void DisconnectRepentogonOptions();
    void SyncConfigFile(bool repentogon);
    void LoadApp(QString FullDir = "", bool force = false, bool disable = false);
    void UpdateGameDir(QString dir);
    void SortLineEdit();
    void ReSyncConfigIni(QSettings *settings);
    void ConfigIniLoad();
    void ConfigIniUnload();
    void DarkMode(bool dark);
    void GetSteamExecutable();
    bool HasModSupport();
    QString GetSteamPath();
    QString getModPath();
    QString IsaacDLC(QString directory);
    QString GetFullDir();
    bool CheckDLCandStore(QFile file);
    QString GetIniFileLocation();
    void GetWineApp();
    void GetWinePrefix();
    void InitThreads();
    bool IsExecutable(QFile file);
    void addToRecent(QString location);
    QAction* MakeRecentPathAction(QString path);
};

#endif // MAINWINDOW_H
