#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include <QHBoxLayout>
#include <QMessageBox>

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

QString GetFullDir(){
    QString steamPath;
    QString ModFolder;
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

void MainWindow::LoadConfigFile(QString confDir = QString()){
    if (confDir.isEmpty()){
    QString osDir = "";
#ifdef Q_OS_WINDOWS
    osDir = QString(getenv("USERPROFILE")) + "/Documents/My Games/Binding of Isaac ";
#elif Q_OS_LINUX
    osDir = QString(getenv("HOME"));
#endif
    confDir = osDir + IsaacDLC(GetFullDir());
    }
    if (QFile::exists(confDir + "/options.ini")){
        QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
        settings.beginGroup("Options");
        //GFX and HUD
        if (settings.value("Fullscreen") == 1) {
            ui->checkBox_Fullscreen->setCheckState(Qt::Checked);
        }else{
            ui->checkBox_Fullscreen->setCheckState(Qt::Unchecked);
            ui->checkBox_Borderless->setEnabled(false);
        }
        connect(ui->checkBox_Fullscreen, &QCheckBox::stateChanged, [=](int state) {
            QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings.setValue("Fullscreen",0);
                ui->checkBox_Borderless->setEnabled(false);
            } else {
                settings.setValue("Fullscreen",1);
                ui->checkBox_Borderless->setEnabled(true);
            }
            settings.endGroup();
            settings.sync();
        });

        if (settings.value("Filter") == 1) {
            ui->checkBox_Filter->setCheckState(Qt::Checked);
        }else{
            ui->checkBox_Filter->setCheckState(Qt::Unchecked);
        }
        connect(ui->checkBox_Filter, &QCheckBox::stateChanged, [=](int state) {
            QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings.setValue("Filter",0);
            } else {
                settings.setValue("Filter",1);
            }
            settings.endGroup();
            settings.sync();
        });

        if (settings.value("CameraStyle") == 2) {
            ui->checkBox_Camera->setCheckState(Qt::Checked);
        }else{
            ui->checkBox_Camera->setCheckState(Qt::Unchecked);
        }
        connect(ui->checkBox_Camera, &QCheckBox::stateChanged, [=](int state) {
            QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings.setValue("CameraStyle",1);
            } else {
                settings.setValue("CameraStyle",2);
            }
            settings.endGroup();
            settings.sync();
        });

        if (settings.value("ChargeBars") == 1) {
            ui->checkBox_ChargeBar->setCheckState(Qt::Checked);
        }else{
            ui->checkBox_ChargeBar->setCheckState(Qt::Unchecked);
        }
        connect(ui->checkBox_ChargeBar, &QCheckBox::stateChanged, [=](int state) {
            QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings.setValue("CameraStyle",0);
            } else {
                settings.setValue("CameraStyle",1);
            }
            settings.endGroup();
            settings.sync();
        });

        if (settings.value("VSync") == 1) {
            ui->checkBox_VSync->setCheckState(Qt::Checked);
        }else{
            ui->checkBox_VSync->setCheckState(Qt::Unchecked);
        }
        connect(ui->checkBox_VSync, &QCheckBox::stateChanged, [=](int state) {
            QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings.setValue("VSync",0);
            } else {
                settings.setValue("VSync",1);
            }
            settings.endGroup();
            settings.sync();
        });

        if (settings.value("FoundHUD") == 1) {
            ui->checkBox_FoundHUD->setCheckState(Qt::Checked);
        }else{
            ui->checkBox_FoundHUD->setCheckState(Qt::Unchecked);
        }
        connect(ui->checkBox_FoundHUD, &QCheckBox::stateChanged, [=](int state) {
            QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings.setValue("FoundHUD",0);
            } else {
                settings.setValue("FoundHUD",1);
            }
            settings.endGroup();
            settings.sync();
        });

        //Music and SFX
        if (settings.value("MusicEnabled") == 1) {
            ui->checkBox_Music->setCheckState(Qt::Checked);
        }else{
            ui->checkBox_Music->setCheckState(Qt::Unchecked);
            ui->horizontalSlider_Music->setEnabled(false);
        }
        connect(ui->checkBox_Music, &QCheckBox::stateChanged, [=](int state) {
            QSettings settings(confDir + "/options.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            if (state == Qt::Unchecked) {
                settings.setValue("MusicEnabled",0);
                ui->horizontalSlider_Music->setEnabled(false);
            } else {
                settings.setValue("MusicEnabled",1);
                ui->horizontalSlider_Music->setEnabled(true);
            }
            settings.endGroup();
            settings.sync();
        });


        settings.endGroup();
    }else{
        ui->groupBox_Console->setEnabled(false);
        ui->groupBox_GFX->setEnabled(false);
        ui->groupBox_Gameplay->setEnabled(false);
        ui->groupBox_SFX->setEnabled(false);
    }
}

bool MainWindow::getGamePath() {
    QString GameDLC;
    QString FullDir = GetFullDir();
    bool found = false;
    QString gameExe = "isaac-ng";
    #ifdef Q_OS_WIN
        gameExe = gameExe + ".exe";
    #endif
    if (QDir(FullDir).exists()){
        if (!QFile(FullDir+gameExe).exists()){
            goto GameNotExists;
        }
        GameDLC = IsaacDLC(FullDir);
        if (GameDLC == "Repentance"){
            found = true;
            directory = FullDir + "mods";
        }else if(GameDLC == "Afterbirth+"){
            found = true;
            #ifdef Q_OS_WIN
               directory = QString(getenv("USERPROFILE"))+"/Documents/My Games/Binding of Isaac Afterbirth+ Mods";
            #elif Q_OS_LINUX
               directory = QString(getenv("HOME"))+"/.local/share/binding of isaac afterbirth+ mods";
            #endif
        }
    }else{
        GameNotExists:
        QMessageBox::information(this, "Game is not installed", "Try locating mod folder manually");
    }
    return found;
}

void MainWindow::ReloadModList(){
    ui->tableWidget->clear();
    loadMods(directory,ui->tableWidget);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen_Mods, &QAction::triggered, this, &MainWindow::on_actionOpenMods);
    connect(ui->actionOpen_Config, &QAction::triggered, this, &MainWindow::on_actionOpenConfig);
    connect(ui->actionReload, &QAction::triggered, this, &MainWindow::ReloadModList);
    if (getGamePath()){
        loadMods(directory, ui->tableWidget);
        ui->actionOpen_Mods->setEnabled(false);
    }
    LoadConfigFile();
    setWindowTitle(IsaacDLC(GetFullDir()) + " Configurator");
}

void MainWindow::on_actionOpenMods() {
    QString osDir = "";
    #ifdef Q_OS_WINDOWS
        osDir = QString(getenv("USERPROFILE"));
    #elif Q_OS_LINUX
        osDir = QString(getenv("HOME"));
    #endif
    directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                 osDir,
                                                                 QFileDialog::ShowDirsOnly
                                                                     | QFileDialog::DontResolveSymlinks);

    loadMods(directory, ui->tableWidget);

}

void MainWindow::on_actionOpenConfig() {
    QString osDir = "";
#ifdef Q_OS_WINDOWS
    osDir = QString(getenv("USERPROFILE"));
#elif Q_OS_LINUX
    osDir = QString(getenv("HOME"));
#endif
    configDir = QFileDialog::getOpenFileName(this, tr("Open File"),
                                             osDir, tr("Options file (options.ini)"));
    LoadConfigFile(configDir);
}

void MainWindow::loadMods(QString directory, QTableWidget *table) {
    QDir dir(directory);
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    table->setColumnCount(3);
    table->setRowCount(folders.length());
    table->setSortingEnabled(true);
    table->setHorizontalHeaderLabels(QStringList() << "Active" << "Mod Name" << "Folder");

    for (int i = 0; i < folders.size(); ++i) {

        QString folder = folders.at(i);

        QTableWidgetItem *item = new QTableWidgetItem(folder);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(i,2,item);

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
                           table->setItem(i,1,modname);
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

        connect(checkBox, &QCheckBox::stateChanged, [=](int state) {
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
        table->setCellWidget(i,0,checkBox);

        table->setItem(i,0, new QTableWidgetCheckBox());

    }

    table->sortItems(1);
    table->resizeColumnToContents(0);
    table->resizeColumnToContents(1);
    table->resizeColumnToContents(2);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

