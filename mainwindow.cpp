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
    timer = new QTimer(this);
    if (!QFile::exists(QApplication::applicationDirPath() + "/IsaacConfigurator.ini")) {
        QFile file(QApplication::applicationDirPath() + "/IsaacConfigurator.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        config->setValue("Language","en_EN");
        config->setValue("CheckUpdate", 1);
        config->setValue("DarkMode", 0);
        config->setValue("DisableRepentogon", 0);
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
        if(config->value("DisableRepentogon") == 1){
            ui->actionDisable_Repentogon->setChecked(true);
            ui->scrollArea_REPENTOGON->setEnabled(false);
        }else{
            ui->actionDisable_Repentogon->setChecked(false);
            ui->scrollArea_REPENTOGON->setEnabled(true);
        }
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

    connect(ui->actionDisable_Repentogon, &QAction::triggered, this, [=](){
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (ui->actionDisable_Repentogon->isChecked()) {
            config->setValue("DisableRepentogon", 1);
        } else {
            config->setValue("DisableRepentogon", 0);
        }
        ui->scrollArea_REPENTOGON->setEnabled(!ui->actionDisable_Repentogon->isChecked() && ui->scrollArea_VanillaOptions->isEnabled() && ui->tabWidget_Options->isEnabled());

        config->endGroup();
        config->sync();
    });

    GetSteamExecutable();
    LoadApp(gameDir+"/"+gameExec);

    connect(ui->tableMods, &QTableWidget::itemDoubleClicked, ui->tableMods, [=](){
        QString folder = ui->tableMods->item(ui->tableMods->currentRow(), 2)->text();
        QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(getModPath()+"/"+folder)));
    });

    connect(ui->actionExit, &QAction::triggered, this, [=](){
        QApplication::quit();
    });

    connect(ui->actionOpen_config_folder, &QAction::triggered, this, [=](){
        if(QDir(configDir).exists()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(configDir)));
        }
    });

    connect(ui->actionOpen_game_folder, &QAction::triggered, this, [=](){
        if(QDir(gameDir).exists()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(gameDir)));
        }
    });

    connect(ui->actionFind_game_folder, &QAction::triggered, this, [=](){
        LoadApp(QFileDialog::getOpenFileName(this, openDirName,
                                                  gameDir,
                                                  QString("(isaac-ng*.exe)")));
    });


    connect(ui->actionStartGame, &QAction::triggered, this, [=](){
        QStringList options = QStringList();
        if (ui->actionDisable_Repentogon->isChecked()){
            options << "-repentogonoff";
        }
        #ifdef Q_OS_WINDOWS
            QProcess::startDetached(gameDir+"/"+gameExec,QStringList() << options);
        #elif defined(Q_OS_LINUX)
            if (gameStore == "Steam"){
                if(gameDLC == "Repentance" || gameDLC == "Repentance+"){
                    QProcess proton;
                    proton.setProgram(GetSteamPath() + "/compatibilitytools.d/proton/version/proton");
                    QStringList command;
                    command << "run"  << gameDir+"/"+gameExec << options;
                    proton.setArguments(command);
                    proton.startDetached();
                }else{
                    QProcess::startDetached("steam", QStringList() << "steam://rungameid/250900");
                }
            }
        #endif

    });
    connect(ui->actionCloseGame, &QAction::triggered, this, [=](){
        QProcess process;
        #ifdef Q_OS_WINDOWS
            process.start("taskkill", QStringList() << "/IM" << gameExec);
            process.waitForFinished();
        #elif defined(Q_OS_LINUX)
            QProcess::execute("pkill", QStringList() << gameExec);
        #endif
    });
    ui->scrollArea_VanillaOptions->setGeometry(ui->scrollArea_VanillaOptions->pos().x(), ui->scrollArea_VanillaOptions->pos().y(), ui->scrollArea_VanillaOptions->size().width() - 5, ui->scrollArea_VanillaOptions->size().height() + 5);
    timer->start(1000);
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

        palette.setColor(QPalette::Disabled, QPalette::Window, QColor(90,90,90));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::Button, QColor(90,90,90));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255,128,128));
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(90,90,90));

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

        palette.setColor(QPalette::Disabled, QPalette::Window, QColor(220,220,220));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(0,0,0));
        palette.setColor(QPalette::Disabled, QPalette::Button, QColor(180,180,180));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(220,220,220));
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(0,0,0));

        qApp->setPalette(palette);
    }
}

void MainWindow::LoadApp(QString FullDir){
    if (FullDir.isEmpty())
    {
        FullDir = gameDir + "/" + gameExec;
    }
    if (QFile::exists(FullDir)){
        CheckDLCandStore(FullDir);
        ui->tabBox_ModsLog->widget(0)->setEnabled(true);
        ui->tabWidget_Options->setEnabled(true);
        ui->menuGame->setEnabled(true);
        QString str = getModPath();
        if (!str.isNull()){
            loadMods(str);
        }else{
            ui->modRadioButton_Folder->setEnabled(false);
            ui->modRadioButton_Name->setEnabled(false);
            ui->activateButton->setEnabled(false);
            ui->deactivateButton->setEnabled(false);
            ui->savePresetButton->setEnabled(false);
            ui->loadPresetButton->setEnabled(false);
            ui->pushButton_UpdateMods->setEnabled(false);
            ui->lineEdit->setEnabled(false);
            ui->tableMods->setEnabled(false);
            if (HasModSupport()){
                QMessageBox::information(this, modMessage1, modMessage2);
            }
        }
        CheckDLCandStore(FullDir);
        LoadConfigFile();
        ConfigIniLoad();
    }else {
        QMessageBox::information(this, gameMessage1, gameMessage2);
        ui->modRadioButton_Folder->setEnabled(false);
        ui->modRadioButton_Name->setEnabled(false);
        ui->activateButton->setEnabled(false);
        ui->deactivateButton->setEnabled(false);
        ui->savePresetButton->setEnabled(false);
        ui->loadPresetButton->setEnabled(false);
        ui->pushButton_UpdateMods->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->tableMods->setEnabled(false);
        ui->tabWidget_Options->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    timer->stop();
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


void MainWindow::onFileLoaded()
{
    if(ui->checkBoxLogUpdate->checkState() == Qt::Checked){
        QFile file(configDir + "/log.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString content = in.readAll();

            QStringList lines = content.split("\n");
            lines.removeAll("");

            for (int i = 0; i < lines.size(); ++i) {
                lines[i] = QString::number(i + 1) + ": " + lines[i];
            }

            QString numberedContent = lines.join("\n");
            file.close();
            if (numberedContent != ui->logBrowser->toPlainText()) {
                ui->logBrowser->setPlainText(numberedContent);
                ui->logBrowser->verticalScrollBar()->setValue(ui->logBrowser->verticalScrollBar()->maximum());
            }
        }
    }
}
