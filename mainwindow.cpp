#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDesktopServices>
#include <QScrollBar>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qApp->setStyle(QStyleFactory::create("fusion"));
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
        config->setValue("CheckUpdate", 1);
        config->setValue("DarkMode", 0);
        config->endGroup();
        config->sync();
        currentTranslator = "en_EN";
    }else{
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        currentTranslator = config->value("Language").toString();

        Qt::CheckState checkState;
        if (config->value("CheckUpdate") == 0) {
            checkState = Qt::Unchecked;
        } else {
            checkState = Qt::Checked;
        }
        ui->checkBoxLogUpdate->setCheckState(checkState);

        ui->actionDark_theme->setChecked(config->value("DarkMode") == 1);
        DarkMode(config->value("DarkMode") == 1);
        config->endGroup();
        config->sync();
    }
    initLanguages(currentTranslator);

    connect(ui->checkBoxLogUpdate, &QCheckBox::stateChanged, this, [=](int state) {
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (state == 0) {
            config->setValue("CheckUpdate", 0);
        } else {
            config->setValue("CheckUpdate", 1);
        }
        config->endGroup();
        config->sync();
    });

    connect(ui->actionDark_theme, &QAction::triggered, this, [=](){
        DarkMode(ui->actionDark_theme->isChecked());
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (ui->actionDark_theme->isChecked()) {
            config->setValue("DarkMode", 1);
        } else {
            config->setValue("DarkMode", 0);
        }
        config->endGroup();
        config->sync();
    });

    LoadApp(GetFullDir(), GetExeName());

    connect(ui->actionExit, &QAction::triggered, this, [=](){
        QApplication::quit();
    });

    connect(ui->actionOpen_config_folder, &QAction::triggered, this, [=](){
        if(QDir(configDir).exists()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(configDir)));
        }
    });

    connect(ui->actionOpen_game_folder, &QAction::triggered, this, [=](){
        if(QDir(configDir).exists()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(GetFullDir())));
        }
    });

    connect(ui->actionFind_game_folder, &QAction::triggered, this, [=](){
        LoadApp(QFileDialog::getExistingDirectory(this, openDirName,
                                                  configDir,
                                                  QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks),GetExeName());
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

void MainWindow::DarkMode(bool dark){
    if(dark){
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(15,15,15));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);

        //palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(palette);
    }else{
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(240,240,240));
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Base, QColor(255,255,255));
        palette.setColor(QPalette::AlternateBase, QColor(245,245,245));
        palette.setColor(QPalette::ToolTipBase, QColor(255,255,220));
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(240,240,240));
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::BrightText, Qt::white);

        //palette.setColor(QPalette::Highlight, QColor(0,120,215).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::white);
        qApp->setPalette(palette);
    }
}

void MainWindow::LoadApp(QString FullDir, QString gameExe){
    if (QFile::exists(FullDir+"/"+gameExe)){
        ui->tabBox->widget(0)->setEnabled(true);
        ui->groupBox_Console->setEnabled(true);
        ui->groupBox_GFX->setEnabled(true);
        ui->groupBox_Misc->setEnabled(true);
        ui->groupBox_Effects->setEnabled(true);
        ui->groupBox_SFX->setEnabled(true);
        ui->menuGame->setEnabled(true);
        QString str = getModPath();
        if (!str.isNull()){
            connect(ui->tableMods, &QTableWidget::itemDoubleClicked, ui->tableMods, [=](){
                QString folder = ui->tableMods->item(ui->tableMods->currentRow(), 2)->text();
                QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(getModPath()+"/"+folder)));
            });
            loadMods(str);
        }else{
            ui->modRadioButton_Folder->setEnabled(false);
            ui->modRadioButton_Name->setEnabled(false);
            ui->activateButton->setEnabled(false);
            ui->deactivateButton->setEnabled(false);
            ui->savePresetButton->setEnabled(false);
            ui->loadPresetButton->setEnabled(false);
            ui->lineEdit->setEnabled(false);
            ui->tableMods->setEnabled(false);
            QMessageBox::information(this, modMessage1, modMessage2);
        }
        LoadConfigFile();
        ConfigIniLoad();
        //setWindowTitle(IsaacDLC(GetFullDir()) + " Configurator");
    }else {
        QMessageBox::information(this, gameMessage1, gameMessage2);
        ui->modRadioButton_Folder->setEnabled(false);
        ui->modRadioButton_Name->setEnabled(false);
        ui->activateButton->setEnabled(false);
        ui->deactivateButton->setEnabled(false);
        ui->savePresetButton->setEnabled(false);
        ui->loadPresetButton->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->tableMods->setEnabled(false);
        ui->groupBox_Console->setEnabled(false);
        ui->groupBox_Effects->setEnabled(false);
        ui->groupBox_GFX->setEnabled(false);
        ui->groupBox_Misc->setEnabled(false);
        ui->groupBox_SFX->setEnabled(false);
        ui->menuGame->setEnabled(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->tabBox->setGeometry(ui->tabBox->pos().x(), ui->tabBox->pos().y(), event->size().width() - ui->tabBox->pos().x() - 10, event->size().height() - ui->tabBox->pos().y() - 53);
    ui->tableMods->setGeometry(ui->tableMods->pos().x(), ui->tableMods->pos().y(),  ui->tabBox->size().width() - ui->tableMods->pos().x() - 10,  ui->tabBox->size().height() - ui->tableMods->pos().y() - 53);
    ui->logBrowser->setGeometry(ui->logBrowser->pos().x(), ui->logBrowser->pos().y(), ui->tabBox->size().width() - ui->tableMods->pos().x() + 1, ui->tabBox->size().height() - ui->tableMods->pos().y() - 20);
    ui->lineEdit->setGeometry(ui->lineEdit->pos().x(), ui->lineEdit->pos().y(), ui->tabBox->size().width() - ui->lineEdit->pos().x() - 10, ui->lineEdit->height());
    ui->activateButton->move(ui->activateButton->pos().x(),  ui->tabBox->size().height() - 51);
    ui->deactivateButton->move(ui->deactivateButton->pos().x(),  ui->tabBox->size().height() - 51);
    ui->pushButton_UpdateMods->move(ui->pushButton_UpdateMods->pos().x(),  ui->tabBox->size().height() - 51);
    ui->savePresetButton->move(ui->savePresetButton->pos().x(),  ui->tabBox->size().height() - 51);
    ui->loadPresetButton->move(ui->loadPresetButton->pos().x(),  ui->tabBox->size().height() - 51);
    ui->checkBoxLogUpdate->move(ui->checkBoxLogUpdate->pos().x(),  ui->tabBox->size().height() - 51);
    ui->pushButtonLogUpdate->move(ui->pushButtonLogUpdate->pos().x(),  ui->tabBox->size().height() - 51);
}

MainWindow::~MainWindow()
{
    logMonitor->stopTimer();
    delete logMonitor;
    optionMonitor->stopTimer();
    delete optionMonitor;
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

void MainWindow::onFileLoaded(QString text, bool force = false)
{
    if(ui->checkBoxLogUpdate->checkState() == Qt::Checked || force == true){
        ui->logBrowser->setPlainText(text);
        ui->logBrowser->verticalScrollBar()->setValue(ui->logBrowser->verticalScrollBar()->maximum());
    }
}

