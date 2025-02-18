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
#include <QTimer>
#include "ui_about.h"
#include "qtablewidgetcheckbox.h"
#include <QDebug>

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

    void onFileLoaded(bool force=false);

    void ReSyncConfigSlot();

    void ReSyncConfigIniSlot();

    void on_lineEditLog_textChanged(const QString &arg1);

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
    QTimer* timer;
    bool connectedVanillaOption = false;
    bool connectedRepentogonOption = false;

    Ui::MainWindow *ui;
    Ui::about *ui_about;
    QDialog *aboutDialog;
    QString configDir;
    QString gameDir;
    QString gameExec = "isaac-ng.exe";
    QString gameStore = "Unknown";
    QString gameDLC = "Rebirth";
    QString currentTranslator;
    void initLanguages(QString translation);
    void retranslateStrings(QString translator);
    void loadMods(QString directory);
    void SyncMods(QString directory);
    void LoadConfigFile();
    void LoadConfig(QString confDir);
    void ConnectVanillaOptions(QSettings *settings, bool force = false);
    void ConnectRepentogonOptions(QSettings *settings, bool force = false);
    void SyncConfigFile(QSettings *settings, bool extra);
    void ReSyncConfig();
    void LoadApp(QString FullDir = "");
    void UpdateGameDir(QString dir);
    void SortLineEdit();
    void ReSyncConfigIni(QSettings *settings);
    void ConfigIniLoad();
    void DarkMode(bool dark);
    void GetSteamExecutable();
    bool HasModSupport();
    QString GetSteamPath();
    QString getModPath();
    QString IsaacDLC(QString directory);
    QString GetFullDir();
    void CheckDLCandStore(QFile file);
    void HighlightTextInLog();

};

#endif // MAINWINDOW_H
