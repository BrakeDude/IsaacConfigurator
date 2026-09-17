#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDesktopServices>
#include <QScrollBar>
#include <QStyleFactory>
#include <QTimer>

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
    recentFiles->setToolTipsVisible(true);
    InitThreads();
    if (!QFile::exists(GetIniFileLocation() + "/IsaacConfigurator.ini")) {
        QFile file(GetIniFileLocation() + "/IsaacConfigurator.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
        QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        config->setValue("Language","en_EN");
        config->setValue("DarkMode", 0);
        config->setValue("DisableRepentogon", 0);
        config->endGroup();
        config->beginGroup("Path");
        config->setValue("Executable", "");
        config->setValue("RecentFiles", "");
#ifdef Q_OS_LINUX
        config->setValue("WinePath", "");
        config->setValue("WinePrefix", "");
#endif
        config->sync();
        config->endGroup();
        currentTranslator = "en_EN";
        DarkMode(false);
    }else{
        QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        currentTranslator = config->value("Language").toString();

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

        config->beginGroup("Path");
        QString dirPath = config->value("Executable").toString();

        for (QString path : config->value("RecentFiles").toString().split(";", Qt::SkipEmptyParts)){
            if(IsExecutable(QFile(path))){
                QAction* act = MakeRecentPathAction(path);
                recentFiles->addAction(act);
            }
        }
#ifdef Q_OS_LINUX
        linuxWineApp = config->value("WinePath").toString();
#endif
        config->endGroup();
        config->sync();

        if(!dirPath.isEmpty() && QFile(dirPath).exists()){
            int lastSlashIndex = dirPath.lastIndexOf('/');
            gameDir  = dirPath.left(lastSlashIndex);
            gameExec = dirPath.mid(lastSlashIndex + 1);
        }

    }
    initLanguages(currentTranslator);

    ui->menuFile->insertMenu(ui->actionOpen_game_folder, recentFiles);

    connect(ui->actionDark_theme, &QAction::triggered, this, [=](){
        DarkMode(ui->actionDark_theme->isChecked());
        QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
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
        QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
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

    //QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
    GetSteamExecutable();
    LoadApp(gameDir+"/"+gameExec, true, !IsExecutable(QFile(gameDir+"/"+gameExec)));

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
        QString extention = QString("(isaac-ng*.exe)");
#ifdef Q_OS_LINUX
        extention = QString("(isaac-ng*.exe isaac.i386 isaac.x64)");
#endif
        QString location = QFileDialog::getOpenFileName(nullptr, openDirName, gameDir, extention, nullptr);
        if (QFile(location).exists()){
            LoadApp(location, true, !IsExecutable(location));
        }
    });


    connect(ui->actionStartGame, &QAction::triggered, this, [=](){
        QStringList options = QStringList();
        if (ui->actionDisable_Repentogon->isChecked() && gameDLC != "Repentance+"){
            options << "-repentogonoff";
        }
        #ifdef Q_OS_WINDOWS
            QProcess::startDetached(gameDir+"/"+gameExec,QStringList() << options);
        #elif defined(Q_OS_LINUX)
        options = QStringList() << "-applaunch" << "250900" << options;

            if (gameStore == "Steam"){
                QProcess::startDetached("steam", options);
            }
            else {
                //Temporary put this for linux
                QMessageBox::information(this, this->windowTitle(), "Please use launcher like Lutris or Heroic.");
            }
        #endif

    });
    connect(ui->actionCloseGame, &QAction::triggered, this, [=](){
        QProcess process;
        #ifdef Q_OS_WINDOWS
            process.start("taskkill", QStringList() << "/IM" << gameExec);
            process.waitForFinished();
        #elif defined(Q_OS_LINUX)

        if(gameExec.contains(".exe")){
            process.start("pgrep", {"-a", "isaac"});
            process.waitForFinished();
            QString output = process.readAllStandardOutput().trimmed();
            output.replace("\\", "/");
            if (output.contains(gameDir + "/" + gameExec)){
                QProcess::execute("kill", QStringList() << output.split(" ").at(0));
            }
        }else {
            QProcess::execute("pkill", QStringList() << gameDir + "/" + gameExec);
        }

        #endif
    });
#ifdef Q_OS_WINDOWS
    ui->actionWineExec->setEnabled(false);
    ui->actionWineExec->setVisible(false);
    ui->actionWinePrefix->setEnabled(false);
    ui->actionWinePrefix->setVisible(false);
#elif defined(Q_OS_LINUX)
    connect(ui->actionWineExec, &QAction::triggered, this, [=](){
        QUrl newWinePath = QFileDialog::getOpenFileUrl(this, setWineApp, QUrl(getenv(("HOME"))), "wine");
        if(!newWinePath.isEmpty() && QFile(newWinePath.path()).exists() && newWinePath.fileName() == "wine"){
            linuxWineApp = newWinePath.fileName();
        }else if(linuxWineApp.isEmpty() || !QDir(linuxWineApp).exists()){
            QMessageBox::information(this, this->windowTitle(), wineApp1);
        }
    });
    connect(ui->actionWinePrefix, &QAction::triggered, this, [=](){
        QUrl newWinePath = QFileDialog::getExistingDirectoryUrl(this, "Set Wine Prefix", linuxWinePrefix.isEmpty() ? QUrl(getenv("HOME")) : QUrl(linuxWinePrefix));
        QDir dir(newWinePath.path());
        if(!dir.isEmpty() && QDir(dir.path() + "/drive_c").exists() && QFile(dir.path() + "/system.reg").exists()){
            linuxWineApp = dir.path();
        }else if(linuxWineApp.isEmpty() || !QDir(linuxWineApp).exists()){
            QMessageBox::information(this, this->windowTitle(), winePrefix1);
        }
    });
#endif


    ui->scrollArea_VanillaOptions->setGeometry(ui->scrollArea_VanillaOptions->pos().x(), ui->scrollArea_VanillaOptions->pos().y(), ui->scrollArea_VanillaOptions->size().width() - 5, ui->scrollArea_VanillaOptions->size().height() + 5);
#ifdef Q_OS_LINUX
    GetWineApp();
    GetWinePrefix();
#endif
}

QAction* MainWindow::MakeRecentPathAction(QString path){
    QAction* act = new QAction();
    if(path.length() > 100){
        act->setText(path.left(97)+"...");
        act->setToolTip(path);
    }else{
        act->setText(path);
    }
    connect(act, &QAction::triggered, this, [=](){
        if (QFile(path).exists()) {
            LoadApp(path);
        }
    });
    return act;
}

void MainWindow::addToRecent(QString location){
    bool doAdd = true;
    for(QAction *act: recentFiles->actions()){
        QString text = act->text();
        if (text.length() == 100 && text.right(3) == "..."){
            text = act->toolTip();
        }
        if(text == location && doAdd){
            doAdd = false;
            break;
        }
    }
    if(doAdd){
        while (recentFiles->actions().count() > 20){
            delete recentFiles->actions().first();
            recentFiles->actions().removeFirst();
        }
        QAction* act = MakeRecentPathAction(location);
        recentFiles->addAction(act);
        QStringList files;
        for(QAction *act: recentFiles->actions()){
            if(act->text().length() == 100 && act->text().right(3) == "..."){
                files.append(act->toolTip());
            }else{
                files.append(act->text());
            }
        }
        QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Path");
        config->setValue("RecentFiles", files.join(";"));
        config->endGroup();
    }
};

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
        //palette.setColor(QPalette::HighlightedText, Qt::black);

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
        //palette.setColor(QPalette::HighlightedText, Qt::white);

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

void MainWindow::LoadApp(QString FullDir, bool force, bool disable){
    if (FullDir.isEmpty())
    {
        FullDir = gameDir + "/" + gameExec;
    }
    if (QFile::exists(FullDir) && CheckDLCandStore(QFile(FullDir)) && !disable){
        ui->actionOpen_game_folder->setEnabled(true);
        ui->tabBox_ModsLog->setEnabled(true);
        ui->tabWidget_Options->setEnabled(true);
        #ifdef Q_OS_WINDOWS
            ui->menuGame->setEnabled(true);
        #elif defined(Q_OS_LINUX)
            ui->menuGame->setEnabled(false);
            ui->menuGame->setVisible(false);
        #endif
        QString str = getModPath();
        if (!str.isNull()){
            loadMods(str);
            if(!ModSyncer->isRunning()){
                ModSyncer->start();
            }
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
            if(ModSyncer->isRunning()){
                ModSyncer->exit();
            }
            if (HasModSupport()){
                QMessageBox::information(this, this->windowTitle(), modMessage1);
            }
        }
        FindConfigFile();
        ConfigIniUnload();
        ConfigIniLoad();
        LoadConfig(true);
        ConfigReader->start();
        QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Path");
        config->setValue("Executable", FullDir);
        config->endGroup();
        ui->actionDisable_Repentogon->setVisible(gameDLC != "Repentance+");
        addToRecent(FullDir);
    }else if(ui->menuGame->isEnabled() || force) {
        QMessageBox::information(this, this->windowTitle(), gameMessage1);
        ui->menuGame->setEnabled(false);
        ui->modRadioButton_Folder->setEnabled(false);
        ui->modRadioButton_Name->setEnabled(false);
        ui->activateButton->setEnabled(false);
        ui->deactivateButton->setEnabled(false);
        ui->savePresetButton->setEnabled(false);
        ui->loadPresetButton->setEnabled(false);
        ui->pushButton_UpdateMods->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->tabBox_ModsLog->setEnabled(false);
        ui->tabWidget_Options->setEnabled(false);
        ui->actionOpen_game_folder->setEnabled(false);
        ui->actionOpen_config_folder->setEnabled(false);
    }
    ui->tabBox_ModsLog->tabBar()->setTabEnabled(1, false);
    ui->tabBox_ModsLog->tabBar()->setTabVisible(1, false);
}

MainWindow::~MainWindow()
{
    ConfigReader->exit();
    ConfigIniReader->exit();
    ModSyncer->exit();
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

void MainWindow::InitThreads(){
    QTimer *configTimer = new QTimer();
    configTimer->moveToThread(ConfigReader);
    connect(configTimer, &QTimer::timeout, ConfigReader, [=](){
        ReSyncConfig();
    });
    connect(ConfigReader, &QThread::finished, configTimer, &QTimer::stop);
    connect(ConfigReader, &QThread::started, [configTimer]() {
        configTimer->start(200); // Start the timer after thread starts
    });
    QTimer *configIniTimer = new QTimer();
    configIniTimer->moveToThread(ConfigIniReader);
    connect(configIniTimer, &QTimer::timeout, ConfigIniReader, [=](){
        ReSyncConfigIniSlot();
    });
    connect(ConfigIniReader, &QThread::finished, configIniTimer, &QTimer::stop);
    connect(ConfigIniReader, &QThread::started, [configIniTimer]() {
        configIniTimer->start(200); // Start the timer after thread starts
    });
    QTimer *modTimer = new QTimer();
    modTimer->moveToThread(ModSyncer);
    connect(modTimer, &QTimer::timeout, ModSyncer, [=](){
        ReSyncMods();
    });
    connect(ModSyncer, &QThread::finished, modTimer, &QTimer::stop);
    connect(ModSyncer, &QThread::started, [modTimer]() {
        modTimer->start(1000); // Start the timer after thread starts
    });

}

bool MainWindow::IsExecutable(QFile file){
    QFileInfo isaacInfo = QFileInfo(file);
    return isaacInfo.isExecutable();
}
