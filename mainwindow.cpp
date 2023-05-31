#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>
#include <QTranslator>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initLanguages();

    LoadApp(GetFullDir(), GetExeName());

    connect(ui->actionExit, &QAction::triggered, this, [=](){
        QApplication::quit();
    });

    connect(ui->actionFind_game_folder, &QAction::triggered, this, [=](){
        LoadApp(QFileDialog::getExistingDirectory(this, openDirName,
                                                  configDir,
                                                  QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks),GetExeName());
    });
    connect(ui->actionSyncOptions, &QAction::triggered, this, [=](){
        ReSyncConfig(configDir);
    });

    connect(ui->actionStartGame, &QAction::triggered, this, [=](){
        QProcess::startDetached(GetFullDir()+"/"+GetExeName(),QStringList());

    });
    connect(ui->actionCloseGame, &QAction::triggered, this, [=](){
        QProcess process;
        process.start("taskkill", QStringList() << "/IM" << GetExeName());
        process.waitForFinished();
    });

}

void MainWindow::LoadApp(QString FullDir, QString gameExe){
    if (QFile::exists(FullDir+"/"+gameExe)){
        ui->groupModsBox->setEnabled(true);
        ui->groupBox_Console->setEnabled(true);
        ui->groupBox_GFX->setEnabled(true);
        ui->groupBox_Misc->setEnabled(true);
        ui->groupBox_SFX->setEnabled(true);
        ui->menuGame->setEnabled(true);
        ui->actionSyncMods->setEnabled(true);
        ui->actionSyncOptions->setEnabled(true);
        QString str = getModPath();
        if (str != NULL){
            connect(ui->actionSyncMods, &QAction::triggered, this, [=](){
               //ui->tableMods->horizontalHeader()->sortIndicatorOrder();
               SyncMods(getModPath());
            });
            connect(ui->tableMods, &QTableWidget::itemDoubleClicked, ui->tableMods, [=](){
                QString folder = ui->tableMods->item(ui->tableMods->currentRow(), 2)->text();
                QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(getModPath()+"/"+folder)));
            });
            loadMods(str);
        }else{
            ui->groupModsBox->setEnabled(false);
            QMessageBox::information(this, modMessage1, modMessage2);
        }
        LoadConfigFile();
        setWindowTitle(IsaacDLC(GetFullDir()) + " configurator");
    }else {
        QMessageBox::information(this, gameMessage1, gameMessage2);
        ui->groupModsBox->setEnabled(false);
        ui->groupBox_Console->setEnabled(false);
        ui->groupBox_GFX->setEnabled(false);
        ui->groupBox_Misc->setEnabled(false);
        ui->groupBox_SFX->setEnabled(false);
        ui->menuGame->setEnabled(false);
        ui->actionSyncMods->setEnabled(false);
        ui->actionSyncOptions->setEnabled(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->groupModsBox->setGeometry(ui->groupModsBox->pos().x(), ui->groupModsBox->pos().y(), event->size().width() - ui->groupModsBox->pos().x() - 10, event->size().height() - ui->groupModsBox->pos().y() - 41);
    ui->tableMods->setGeometry(ui->tableMods->pos().x(), ui->tableMods->pos().y(),  ui->groupModsBox->size().width() - ui->tableMods->pos().x() - 10,  ui->groupModsBox->size().height() - ui->tableMods->pos().y() - 41);
    ui->activateButton->move(ui->activateButton->pos().x(),  ui->groupModsBox->size().height() - 31);
    ui->deactivateButton->move(ui->deactivateButton->pos().x(),  ui->groupModsBox->size().height() - 31);

}

MainWindow::~MainWindow()
{
    delete ui;
}

