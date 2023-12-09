#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>

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

    int popup_value = settings->value("PopUps").toInt();
    if (ui->comboBox_PopUp->count() == 3 && !settings->contains("AcceptedPublicBeta")){
        ui->comboBox_PopUp->removeItem(2);
        popup_value = std::max(popup_value, 1);
    }else if(ui->comboBox_PopUp->count() == 2 && settings->contains("AcceptedPublicBeta")){
        ui->comboBox_PopUp->addItem(tr("Small"));
    }
    ui->comboBox_PopUp->setCurrentIndex(popup_value);

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
    ui->spinBox_MaxScale->setValue((settings->value("MaxScale").toInt()));
    ui->spinBox_MaxRenderScale->setValue((settings->value("MaxRenderScale").toInt()));

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

    if (settings->value("EnableMods") == 1) {
        ui->checkBox_ModsEnabled->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_ModsEnabled->setCheckState(Qt::Unchecked);
    }

    //Online beta
    if (settings->value("StreamerMode") == 1) {
        ui->checkBox_Steamer->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Steamer->setCheckState(Qt::Unchecked);
    }

    if (settings->value("JacobEsauControls") == 1) {
        ui->checkBox_JEcontrols->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_JEcontrols->setCheckState(Qt::Unchecked);
    }

    ui->horizontalSlider_PlayersSFX->setValue(settings->value("OnlinePlayerVolume").toInt());

    ui->horizontalSlider_PlayerOpacity->setValue(settings->value("OnlinePlayerOpacity").toInt());

    ui->horizontalSlider_HUD->setValue((settings->value("HudOffset").toFloat()*10));

    settings->endGroup();
}

void MainWindow::LoadConfig(QString confDir){
    if (QFile::exists(confDir + "/options.ini")){
        ui->groupBox_Console->setEnabled(true);
        ui->groupBox_GFX->setEnabled(true);
        ui->groupBox_Misc->setEnabled(true);
        ui->groupBox_SFX->setEnabled(true);
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

        connect(ui->comboBox_PopUp, &QComboBox::currentTextChanged, this, [=](){
            settings->beginGroup("Options");
            settings->setValue("PopUps",ui->comboBox_PopUp->currentIndex());
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

        connect(ui->comboBox_ConsoleFont, &QComboBox::currentTextChanged, this, [=](){
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

        connect(ui->comboBox_ExtraHUD, &QComboBox::currentTextChanged, this, [=](){
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

        connect(ui->spinBox_MaxScale, &QSpinBox::textChanged, this, [=](QString text) {
            settings->beginGroup("Options");
            settings->setValue("MaxScale",text.toInt());
            settings->endGroup();
            settings->sync();
        });

        connect(ui->spinBox_MaxRenderScale, &QSpinBox::textChanged, this, [=](QString text) {
            settings->beginGroup("Options");
            settings->setValue("MaxRenderScale",text.toInt());
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

        connect(ui->comboBox_ConsoleFont, &QComboBox::currentTextChanged, this, [=](){
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

        connect(ui->checkBox_ModsEnabled, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("EnableMods",0);
            } else {
                settings->setValue("EnableMods",1);
            }
            settings->endGroup();
            settings->sync();
        });

        //Online Beta
        connect(ui->checkBox_JEcontrols, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("JacobEsauControls",0);
            } else {
                settings->setValue("JacobEsauControls",1);
            }
            settings->endGroup();
            settings->sync();
        });
        connect(ui->checkBox_Steamer, &QCheckBox::stateChanged, this, [=](int state) {
            settings->beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings->setValue("StreamerMode",0);
            } else {
                settings->setValue("StreamerMode",1);
            }
            settings->endGroup();
            settings->sync();
        });

        connect(ui->horizontalSlider_PlayerOpacity, &QSlider::valueChanged, this, [=](int val) {
            settings->beginGroup("Options");
            settings->setValue("OnlinePlayerOpacity",QString::number(val));
            settings->endGroup();
            settings->sync();
        });

        connect(ui->horizontalSlider_PlayersSFX, &QSlider::valueChanged, this, [=](int val) {
            settings->beginGroup("Options");
            settings->setValue("OnlinePlayerVolume",QString::number(val));
            settings->endGroup();
            settings->sync();
        });


    }else{
        ui->groupBox_Console->setEnabled(false);
        ui->groupBox_GFX->setEnabled(false);
        ui->groupBox_Misc->setEnabled(false);
        ui->groupBox_SFX->setEnabled(false);
        ui->groupBox_OnlineBetaSettings->setEnabled(false);
        QMessageBox::information(this, optionMessage1, optionMessage2);
    }
}

void MainWindow::ReSyncConfigSlot(QString configDir){
    ReSyncConfig(configDir);
}

void MainWindow::LoadConfigFile(){
    QString DLCName = IsaacDLC(GetFullDir());
#ifdef Q_OS_WINDOWS
    configDir = QString(getenv("USERPROFILE")) + "/Documents/My Games/Binding of Isaac " + DLCName;
#elif defined(Q_OS_LINUX)
    if (DLCName == "Repentance") {
        configDir = QDir::homePath() + "/.steam/steam/steamapps/compatdata/250900/pfx/drive_c/users/steamuser/Documents/My Games/Binding of Isaac Repentance/";
    }else{
        configDir = QDir::homePath() + "/.local/share/binding of isaac " + DLCName.toLower();
    }
#endif
    LoadConfig(configDir);
    if (QFile::exists(configDir + "/log.txt")){
        logMonitor = new FileMonitor(configDir + "/log.txt", 1, 100);

        connect(logMonitor, SIGNAL(logLoaded(QString, bool)), this, SLOT(onFileLoaded(QString,bool)));
        connect(ui->pushButtonLogUpdate, &QPushButton::clicked, this, [=](){
            logMonitor->monitorLog(true);
        });
        connect(ui->checkBoxLogUpdate, &QCheckBox::stateChanged, this, [=](int state) {
            if (state == Qt::Checked) {
                logMonitor->monitorLog(true);
            }
        });
    }
    if (QFile::exists(configDir + "/options.ini")){
        optionMonitor = new FileMonitor(configDir, 2, 100);
        connect(optionMonitor, SIGNAL(optionLoaded(QString)), this, SLOT(ReSyncConfigSlot(QString)));
    }
}

void MainWindow::ReSyncConfig(QString confDir){
    configDir = confDir;
    if (QDir(configDir).exists())
    {
        ui->actionOpen_config_folder->setEnabled(true);
    }else{
        ui->actionOpen_config_folder->setEnabled(false);
    }
    if (QFile::exists(configDir + "/options.ini")){
        ui->groupBox_Console->setEnabled(true);
        ui->groupBox_GFX->setEnabled(true);
        ui->groupBox_Misc->setEnabled(true);
        ui->groupBox_SFX->setEnabled(true);
        ui->groupBox_OnlineBetaSettings->setEnabled(true);
        QSettings *settings = new QSettings(configDir + "/options.ini", QSettings::IniFormat);
        settings->beginGroup("Options");
        if (!settings->contains("AcceptedPublicBeta")){
            ui->groupBox_OnlineBetaSettings->setEnabled(false);
        }
        settings->endGroup();
        SyncConfigFile(settings);
    }else{
        ui->groupBox_Console->setEnabled(false);
        ui->groupBox_GFX->setEnabled(false);
        ui->groupBox_Misc->setEnabled(false);
        ui->groupBox_SFX->setEnabled(false);
        ui->groupBox_OnlineBetaSettings->setEnabled(false);
        //QMessageBox::information(this, optionMessage1, optionMessage2);
    }
}
