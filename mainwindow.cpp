#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui_about = new Ui::about();
    aboutDialog = new QDialog(this);
    aboutDialog->setWindowFlags(aboutDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui_about->setupUi(aboutDialog);
    if (!QFile::exists(QApplication::applicationDirPath() + "/IsaacConfigurator.ini")) {
        QFile file(QApplication::applicationDirPath() + "/IsaacConfigurator.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        config->setValue("Language","en_EN");
        config->endGroup();
        config->sync();
        currentTranslator = "en_EN";
    }else{
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        currentTranslator = config->value("Language").toString();
        config->endGroup();
        config->sync();
    }
    initLanguages(currentTranslator);

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

#ifdef Q_OS_WINDOWS
    connect(ui->actionStartGame, &QAction::triggered, this, [=](){
        QProcess::startDetached(GetFullDir()+"/"+GetExeName(),QStringList());

    });
    connect(ui->actionCloseGame, &QAction::triggered, this, [=](){
        QProcess process;
        process.start("taskkill", QStringList() << "/IM" << GetExeName());
        process.waitForFinished();
    });
#else
    ui->actionStartGame->setEnabled(false);
    ui->actionCloseGame->setEnabled(false);
#endif

}

void MainWindow::LoadApp(QString FullDir, QString gameExe){
    if (QFile::exists(FullDir+"/"+gameExe)){
        ui->tabBox->setEnabled(true);
        ui->groupBox_Console->setEnabled(true);
        ui->groupBox_GFX->setEnabled(true);
        ui->groupBox_Misc->setEnabled(true);
        ui->groupBox_Effects->setEnabled(true);
        ui->groupBox_SFX->setEnabled(true);
        ui->menuGame->setEnabled(true);
        ui->actionSyncMods->setEnabled(true);
        ui->actionSyncOptions->setEnabled(true);
        QString str = getModPath();
        if (!str.isNull()){
            connect(ui->actionSyncMods, &QAction::triggered, this, [=](){
               SyncMods(str);
            });
            connect(ui->tableMods, &QTableWidget::itemDoubleClicked, ui->tableMods, [=](){
                QString folder = ui->tableMods->item(ui->tableMods->currentRow(), 2)->text();
                QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(getModPath()+"/"+folder)));
            });
            loadMods(str);
        }else{
            ui->tabBox->setEnabled(false);
            QMessageBox::information(this, modMessage1, modMessage2);
        }
        LoadConfigFile();
        ConfigIniLoad();
        //setWindowTitle(IsaacDLC(GetFullDir()) + " Configurator");
    }else {
        QMessageBox::information(this, gameMessage1, gameMessage2);
        ui->tabBox->setEnabled(false);
        ui->groupBox_Console->setEnabled(false);
        ui->groupBox_Effects->setEnabled(false);
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
    ui->tabBox->setGeometry(ui->tabBox->pos().x(), ui->tabBox->pos().y(), event->size().width() - ui->tabBox->pos().x() - 10, event->size().height() - ui->tabBox->pos().y() - 53);
    ui->tableMods->setGeometry(ui->tableMods->pos().x(), ui->tableMods->pos().y(),  ui->tabBox->size().width() - ui->tableMods->pos().x() - 10,  ui->tabBox->size().height() - ui->tableMods->pos().y() - 53);
    ui->logBrowser->setGeometry(ui->logBrowser->pos().x(), ui->logBrowser->pos().y(), ui->tabBox->size().width() - ui->tableMods->pos().x() , ui->tabBox->size().height() - ui->tableMods->pos().y() - 33);
    ui->lineEdit->setGeometry(ui->lineEdit->pos().x(), ui->lineEdit->pos().y(), ui->tabBox->size().width() - ui->lineEdit->pos().x() - 10, ui->lineEdit->height());
    ui->activateButton->move(ui->activateButton->pos().x(),  ui->tabBox->size().height() - 51);
    ui->deactivateButton->move(ui->deactivateButton->pos().x(),  ui->tabBox->size().height() - 51);
    ui->updateLogButton->move(ui->updateLogButton->pos().x(),  ui->tabBox->size().height() - 61);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_modRadioButton_Name_toggled(bool checked)
{
    if(checked){
        SortLineEdit();
    }
}


void MainWindow::on_modRadioButton_Folder_toggled(bool checked)
{
    if(checked){
        SortLineEdit();
    }
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    SortLineEdit();
}


void MainWindow::on_actionAbout_triggered()
{
    ui_about->labelAboutText->setFixedHeight(ui_about->labelAboutText->sizeHint().height());
    aboutDialog->setFixedSize(QSize(300, ui_about->labelAboutText->sizeHint().height() + ui_about->labelAboutName->sizeHint().height()));
    aboutDialog->exec();
}

