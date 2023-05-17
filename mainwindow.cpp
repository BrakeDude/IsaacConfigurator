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

bool Exit = false;

QString IsaacDLC(QString directory)
{
    if (QFile(directory+"/resources/packed/repentance.a").exists()){
        return "Repentance";
    }else if(QFile(directory+"/resources/packed/afterbirthp.a").exists()){
        return "Afterbirth+";
    }else if(QFile(directory+"/resources/packed/afterbirth.a").exists()){
        return "Afterbirth";
    }else if(QFile(directory+"/resources/packed/anitbirth.a").exists()){
        return "Antibirth";
    }
    return "Rebirth";
}

QString GetExeName(){
    QString gameExe = "isaac-ng";
#ifdef Q_OS_WIN
    gameExe = gameExe + ".exe";
#endif
    return gameExe;
}

QString GetFullDir(){
    QString steamPath;
#ifdef Q_OS_WIN
    QSettings registry32("HKEY_LOCAL_MACHINE\\SOFTWARE\\Valve\\Steam", QSettings::NativeFormat);
    steamPath = registry32.value("InstallPath").toString();
    if (steamPath.isEmpty()) {
        QSettings registry64("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Valve\\Steam", QSettings::NativeFormat);
        steamPath = registry64.value("InstallPath").toString();
    }
#elif Q_OS_LINUX
    QString homePath = QDir::homePath();
    QString configPath = homePath + "/.steam/steam/steamapps/libraryfolders.vdf";
    if (QFile::exists(configPath)) {
        steamPath = homePath + "/.steam/steam";
    }
#endif
    return steamPath + "/steamapps/common/The Binding of Isaac Rebirth/";
}

void MainWindow::SyncConfigFile(QSettings *settings){
    //GFX and HUD
    settings->beginGroup("Options");
    if (settings->value("Fullscreen") == 1) {
        ui->checkBox_Fullscreen->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Fullscreen->setCheckState(Qt::Unchecked);
    }

    if (settings->value("UseBorderlessFullscreen") == 1) {
        ui->checkBox_Borderless->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Borderless->setCheckState(Qt::Unchecked);
    }

    if (settings->value("Filter") == 1) {
        ui->checkBox_Filter->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Filter->setCheckState(Qt::Unchecked);
    }

    if (settings->value("PopUps") == 1) {
        ui->checkBox_PopUp->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_PopUp->setCheckState(Qt::Unchecked);
    }

    if (settings->value("CameraStyle") == 1) {
        ui->checkBox_Camera->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Camera->setCheckState(Qt::Unchecked);
    }

    if (settings->value("ChargeBars") == 1) {
        ui->checkBox_ChargeBar->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_ChargeBar->setCheckState(Qt::Unchecked);
    }

    if (settings->value("VSync") == 1) {
        ui->checkBox_VSync->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_VSync->setCheckState(Qt::Unchecked);
    }

    if (settings->value("FoundHUD") == 1) {
        ui->checkBox_FoundHUD->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_FoundHUD->setCheckState(Qt::Unchecked);
    }

    ui->comboBox_ConsoleFont->setCurrentIndex(settings->value("ConsoleFont").toInt());

    if (settings->value("BossHpOnBottom") == 1) {
        ui->checkBox_BossHPBot->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_BossHPBot->setCheckState(Qt::Unchecked);
    }

    if (settings->value("CameraStyle") == 1) {
        ui->checkBox_Camera->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Camera->setCheckState(Qt::Unchecked);
    }

    if (settings->value("BulletVisibility") == 1) {
        ui->checkBox_BulletVisibility->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_BulletVisibility->setCheckState(Qt::Unchecked);
    }

    ui->comboBox_ExtraHUD->setCurrentIndex(settings->value("ShowRecentItems").toInt());

    ui->horizontalSlider_Map->setValue((settings->value("MapOpacity").toFloat()*10));

    ui->horizontalSlider_HUD->setValue((settings->value("HudOffset").toFloat()*10));

    ui->spinBox_Gamma->setValue((settings->value("Gamma").toFloat()*100));

    ui->spinBox_Width->setValue((settings->value("WindowWidth").toInt()));
    ui->spinBox_Height->setValue((settings->value("WindowHeight").toInt()));
    ui->spinBox_PosX->setValue((settings->value("WindowPosX").toInt()));
    ui->spinBox_PosY->setValue((settings->value("WindowPosY").toInt()));

    //Music and SFX
    if (settings->value("MusicEnabled") == 1) {
        ui->checkBox_Music->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Music->setCheckState(Qt::Unchecked);
    }

    ui->comboBox_Announcer->setCurrentIndex(settings->value("AnnouncerVoiceMode").toInt());

    ui->horizontalSlider_Music->setValue((settings->value("MusicVolume").toFloat()*10));

    ui->horizontalSlider_SFX->setValue((settings->value("SFXVolume").toFloat()*10));

    //Console and Debugging
    if (settings->value("EnableDebugConsole") == 1) {
        ui->checkBox_ConsoleEnabled->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_ConsoleEnabled->setCheckState(Qt::Unchecked);
    }

    if (settings->value("FadedConsoleDisplay") == 1) {
        ui->checkBox_FadeConsole->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_FadeConsole->setCheckState(Qt::Unchecked);
    }

    if (settings->value("SaveCommandHistory") == 1) {
        ui->checkBox_SaveCMDHistory->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_SaveCMDHistory->setCheckState(Qt::Unchecked);
    }

    ui->comboBox_ConsoleFont->setCurrentIndex(settings->value("ConsoleFont").toInt());

    //Misc
    if (settings->value("RumbleEnabled") == 1) {
        ui->checkBox_Rumble->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Rumble->setCheckState(Qt::Unchecked);
    }

    if (settings->value("PauseOnFocusLost") == 1) {
        ui->checkBox_Pause->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Pause->setCheckState(Qt::Unchecked);
    }

    if (settings->value("MouseControl") == 1) {
        ui->checkBox_Mouse->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Mouse->setCheckState(Qt::Unchecked);
    }

    if (settings->value("SteamCloud") == 1) {
        ui->checkBox_Steam->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Steam->setCheckState(Qt::Unchecked);
    }

    settings->endGroup();
}

void MainWindow::LoadConfig(QString confDir){
    if (QFile::exists(confDir + "/options.ini")){
        QSettings *settings = new QSettings(configDir + "/options.ini", QSettings::IniFormat);

        SyncConfigFile(settings);
        //GFX and HUD
        connect(ui->checkBox_Fullscreen, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("Fullscreen",0);
            } else {
                settings->setValue("Fullscreen",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_Borderless, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("UseBorderlessFullscreen",0);
            } else {
                settings->setValue("UseBorderlessFullscreen",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_Filter, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("Filter",0);
            } else {
                settings->setValue("Filter",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_PopUp, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("PopUps",0);
            } else {
                settings->setValue("PopUps",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_Camera, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("CameraStyle",2);
            } else {
                settings->setValue("CameraStyle",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_ChargeBar, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("CameraStyle",0);
            } else {
                settings->setValue("CameraStyle",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_VSync, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("VSync",0);
            } else {
                settings->setValue("VSync",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_FoundHUD, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("FoundHUD",0);
            } else {
                settings->setValue("FoundHUD",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->comboBox_ConsoleFont, &QComboBox::currentTextChanged, this, [=](QString text){
            settings->beginGroup("Options");
            settings->setValue("ConsoleFont",ui->comboBox_ConsoleFont->currentIndex());
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_BossHPBot, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("BossHpOnBottom",0);
            } else {
                settings->setValue("BossHpOnBottom",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_Camera, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("BossHpOnBottom",2);
            } else {
                settings->setValue("BossHpOnBottom",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_BulletVisibility, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("BulletVisibility",0);
            } else {
                settings->setValue("BulletVisibility",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->comboBox_ExtraHUD, &QComboBox::currentTextChanged, this, [=](QString text){
            settings->beginGroup("Options");
            settings->setValue("ShowRecentItems",ui->comboBox_ExtraHUD->currentIndex());
            settings->endGroup();
            settings->sync();
        });

        connect(ui->horizontalSlider_HUD, &QSlider::valueChanged, this, [=](int val) {
            settings->beginGroup("Options");
            settings->setValue("HudOffset",QString::number(val/10.0000, 'f', 4));
            settings->endGroup();
            settings->sync();
        });

        connect(ui->horizontalSlider_Map, &QSlider::valueChanged, this, [=](int val) {
            settings->beginGroup("Options");
            settings->setValue("MapOpacity",QString::number(val/10.0000, 'f', 4));
            settings->endGroup();
            settings->sync();
        });

        connect(ui->spinBox_Gamma, &QSpinBox::textChanged, this, [=](QString text) {
            settings->beginGroup("Options");
            settings->setValue("Gamma",QString::number(text.toInt()/100.0000, 'f', 4));
            settings->endGroup();
            settings->sync();
        });

        connect(ui->spinBox_Width, &QSpinBox::textChanged, this, [=](QString text) {
            settings->beginGroup("Options");
            settings->setValue("WindowWidth",text.toInt());
            settings->endGroup();
            settings->sync();
        });

        connect(ui->spinBox_Height, &QSpinBox::textChanged, this, [=](QString text) {
            settings->beginGroup("Options");
            settings->setValue("WindowHeight",text.toInt());
            settings->endGroup();
            settings->sync();
        });

        connect(ui->spinBox_PosX, &QSpinBox::textChanged, this, [=](QString text) {
            settings->beginGroup("Options");
            settings->setValue("WindowPosX",text.toInt());
            settings->endGroup();
            settings->sync();
        });

        connect(ui->spinBox_PosY, &QSpinBox::textChanged, this, [=](QString text) {
            settings->beginGroup("Options");
            settings->setValue("WindowPosY",text.toInt());
            settings->endGroup();
            settings->sync();
        });


        //Music and SFX
        connect(ui->checkBox_Music, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("MusicEnabled",0);
            } else {
                settings->setValue("MusicEnabled",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->horizontalSlider_Music, &QSlider::valueChanged, this, [=](int val) {
            settings->beginGroup("Options");
            settings->setValue("MusicVolume",QString::number(val/10.0000, 'f', 4));
            settings->endGroup();
            settings->sync();
        });

        connect(ui->horizontalSlider_SFX, &QSlider::valueChanged, this, [=](int val) {
            settings->beginGroup("Options");
            settings->setValue("SFXVolume",QString::number(val/10.0000, 'f', 4));
            settings->endGroup();
            settings->sync();
        });

        //Console and Debugging
        connect(ui->checkBox_ConsoleEnabled, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("EnableDebugConsole",0);
            } else {
                settings->setValue("EnableDebugConsole",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_FadeConsole, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("FadedConsoleDisplay",0);
            } else {
                settings->setValue("FadedConsoleDisplay",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_SaveCMDHistory, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("SaveCommandHistory",0);
            } else {
                settings->setValue("SaveCommandHistory",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->comboBox_ConsoleFont, &QComboBox::currentTextChanged, this, [=](QString text){
            settings->beginGroup("Options");
            settings->setValue("ConsoleFont",ui->comboBox_ConsoleFont->currentIndex());
            settings->endGroup();
            settings->sync();
        });

        //Misc
        connect(ui->checkBox_Rumble, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("RumbleEnabled",0);
            } else {
                settings->setValue("RumbleEnabled",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_Mouse, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("MouseControl",0);
            } else {
                settings->setValue("MouseControl",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_Pause, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("PauseOnFocusLost",0);
            } else {
                settings->setValue("PauseOnFocusLost",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->checkBox_Steam, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("SteamCloud",0);
            } else {
                settings->setValue("SteamCloud",1);
            }
            settings->endGroup();
            settings->sync();
        });

    }else{
        ui->groupBox_Console->setEnabled(false);
        ui->groupBox_GFX->setEnabled(false);
        ui->groupBox_Misc->setEnabled(false);
        ui->groupBox_SFX->setEnabled(false);
        QMessageBox::information(this, "Can't find file", "File 'options.ini' doesn't exists. Try location it manually");
    }
}

void MainWindow::LoadConfigFile(){
    QString osDir = "";
#ifdef Q_OS_WINDOWS
    osDir = QString(getenv("USERPROFILE")) + "/Documents/My Games/Binding of Isaac ";
#elif Q_OS_LINUX
    osDir = QString(getenv("HOME"));
#endif
    configDir = osDir + IsaacDLC(GetFullDir());
    LoadConfig(configDir);
}

void MainWindow::ReSyncConfig(QString confDir){
    configDir = confDir;
    if (QFile::exists(configDir + "/options.ini")){
        QSettings *settings = new QSettings(configDir + "/options.ini", QSettings::IniFormat);
        SyncConfigFile(settings);
    }
}

QString getModPath() {
    QString GameDLC;
    QString FullDir = GetFullDir();

        GameDLC = IsaacDLC(FullDir);
        if (GameDLC == "Repentance"){
            return FullDir + "mods";
        }else if(GameDLC == "Afterbirth+"){
            QString directory;
            #ifdef Q_OS_WIN
               directory = QString(getenv("USERPROFILE"))+"/Documents/My Games/Binding of Isaac Afterbirth+ Mods";
            #elif Q_OS_LINUX
               directory = QString(getenv("HOME"))+"/.local/share/binding of isaac afterbirth+ mods";
            #endif
           return directory;
        }

    return NULL;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoadApp(GetFullDir(), GetExeName());

    connect(ui->actionExit, &QAction::triggered, this, [=](bool checked){
        QApplication::quit();
    });

    connect(ui->actionFind_game_folder, &QAction::triggered, this, [=](){
        LoadApp(QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                  configDir,
                                                  QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks),GetExeName());
    });
    connect(ui->actionSyncOptions, &QAction::triggered, this, [=](){
        ReSyncConfig(configDir);
    });

    connect(ui->actionStartGame, &QAction::triggered, this, [=](){
        QProcess::startDetached(GetFullDir()+GetExeName(),QStringList());

    });
    connect(ui->actionCloseGame, &QAction::triggered, this, [=](){
        QProcess process;
        process.start("taskkill", QStringList() << "/IM" << GetExeName());
        process.waitForFinished();
    });

}

void MainWindow::LoadApp(QString FullDir, QString gameExe){
    if (QFile::exists(FullDir+gameExe)){
       QString str = getModPath();
       if (str != NULL){
           connect(ui->actionSyncMods, &QAction::triggered, this, [=](){
               ui->tableMods->horizontalHeader()->sortIndicatorOrder();
               SyncMods(getModPath());
           });
           loadMods(str);
       }else{
           ui->tableMods->setEnabled(false);
           QMessageBox::information(this, "No mod folder", "Couldn't locate mod folder. Please, make sure you have Afterbirth+ or Repentance installed.");
       }
       LoadConfigFile();
       setWindowTitle(IsaacDLC(GetFullDir()) + " Configurator");
    }else {
       QMessageBox::information(this, "No game found", "Install game first before running app.");
       ui->tableMods->setEnabled(false);
       ui->groupBox_Console->setEnabled(false);
       ui->groupBox_GFX->setEnabled(false);
       ui->groupBox_Misc->setEnabled(false);
       ui->groupBox_SFX->setEnabled(false);
       ui->menuGame->setEnabled(false);
       ui->actionSyncMods->setEnabled(false);
       ui->actionSyncOptions->setEnabled(false);
    }
}

void MainWindow::SyncMods(QString directory){
    QDir dir(directory);
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->tableMods->sortItems(2);
    for (int i = 0; i < folders.size(); ++i) {

       QString folder = folders.at(i);

       QTableWidgetItem *item = new QTableWidgetItem(folder);
       item->setFlags(item->flags() ^ Qt::ItemIsEditable);
       ui->tableMods->setItem(i,2,item);

       QString modName=folder;
       QString metadata_path = directory + "/" + folder + "/metadata.xml";
       if (QFile::exists(metadata_path)) {
           QFile file(metadata_path);
           if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
               QXmlStreamReader xml(&file);
               while (!xml.atEnd() && !xml.hasError()) {
                   QXmlStreamReader::TokenType token = xml.readNext();
                   if (token == QXmlStreamReader::StartDocument) continue;
                   if (token == QXmlStreamReader::StartElement) {
                       if (xml.name() == "name") {
                           QTableWidgetItem *modname = new QTableWidgetItem(xml.readElementText());
                           modname->setFlags(modname->flags() ^ Qt::ItemIsEditable);
                           ui->tableMods->setItem(i,1,modname);
                           break;
                       }
                   }
               }
               file.close();
           }
       }

       QCheckBox *checkBox = new QCheckBox();
       if (QFile::exists(directory + "/" + folder + "/disable.it")){
           checkBox->setCheckState(Qt::Unchecked);
       }else{
           checkBox->setCheckState(Qt::Checked);
       }

       connect(checkBox, &QCheckBox::stateChanged, this, [=](int state) {
           if (state == Qt::Unchecked) {
               QFile file(directory + "/" + folder + "/disable.it");
               file.open(QIODevice::WriteOnly | QIODevice::Text);
               file.close();
           } else {
               if (QFile::exists(directory + "/" + folder + "/disable.it")) {
                   QFile::remove(directory + "/" + folder + "/disable.it");
               }
           }
       });
       ui->tableMods->setCellWidget(i,0,checkBox);

       ui->tableMods->setItem(i,0, new QTableWidgetCheckBox());

    }
    ui->tableMods->sortItems(1);
}

void MainWindow::loadMods(QString directory) {
    QDir dir(directory);
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->tableMods->setColumnCount(3);
    ui->tableMods->setRowCount(folders.length());
    ui->tableMods->setSortingEnabled(true);
    ui->tableMods->sortItems(2);
    ui->tableMods->setHorizontalHeaderLabels(QStringList() << "Active" << "Mod Name" << "Folder");
    SyncMods(directory);
    ui->tableMods->resizeColumnToContents(0);
    ui->tableMods->resizeColumnToContents(1);
    ui->tableMods->resizeColumnToContents(2);
    ui->tableMods->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableMods->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->tableMods->setGeometry(ui->tableMods->pos().x(), ui->tableMods->pos().y(), event->size().width() - ui->tableMods->pos().x() - 10, event->size().height() - ui->tableMods->pos().y() - 41);
}

MainWindow::~MainWindow()
{
    delete ui;
}

