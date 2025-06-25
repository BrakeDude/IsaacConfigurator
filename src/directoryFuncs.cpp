#include "headers/mainwindow.h"
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QProcess>

bool MainWindow::CheckDLCandStore(QFile isaacng)
{
    if (isaacng.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&isaacng);
        QString line = in.readAll();
        in.flush();
        isaacng.close();
        QFileInfo isaacInfo = QFileInfo(isaacng);
        gameDir = isaacInfo.dir().path();
        gameExec = isaacInfo.fileName();
        if(!isaacInfo.isExecutable()){
            gameDLC = "Unknown";
            this->setWindowTitle("Isaac Configurator: " + gameDLC);
            return false;
        }
        QString gameNamePrefix("Binding of Isaac: ");
        QStringList DLCNames = {"Repentance+", "Repentance", "Afterbirth+", "Afterbirth", "Rebirth"};
        QMap<QString, QString> stores;
        stores.insert("GOG", "Galaxy");
        stores.insert("Epic", "Epic");
        foreach (QString var, DLCNames){
            for (auto it = stores.keyValueBegin(); it != stores.keyValueEnd(); ++it){
                if (line.contains(var + " (" + it->second + ")")){
                    gameStore = it->first;
                    goto StoreSet;
                }
            }
        }
        if (!GetSteamPath().isEmpty() && QDir(GetSteamPath() + "/steamapps/common/The Binding of Isaac Rebirth").exists()
            && isaacInfo.dir() == GetSteamPath() + "/steamapps/common/The Binding of Isaac Rebirth")
        {
            gameStore = "Steam";
        }
        StoreSet:
        foreach (QString var, DLCNames){
            if (line.contains(gameNamePrefix + var)){
                gameDLC = var;
                QString windowName = "Isaac Configurator: " + gameDLC + " (" + gameStore + ")";
                this->setWindowTitle(windowName);
                return true;
            }
        }
        gameDLC = "Rebirth";
        this->setWindowTitle("Isaac Configurator: " + gameDLC);
        return true;
    }
    gameDLC = "Unknown";
    this->setWindowTitle("Isaac Configurator: " + gameDLC);
    return false;
}

QString MainWindow::GetSteamPath(){
    QString steamPath;
#ifdef Q_OS_WIN
    QSettings registry32("HKEY_LOCAL_MACHINE\\SOFTWARE\\Valve\\Steam", QSettings::NativeFormat);
    steamPath = registry32.value("InstallPath").toString();
    if (steamPath.isEmpty()) {
        QSettings registry64("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Valve\\Steam", QSettings::NativeFormat);
        steamPath = registry64.value("InstallPath").toString();
    }
#elif defined(Q_OS_LINUX)
    QString homePath = QDir::homePath();
    QString configPath = homePath + "/.steam/steam/steamapps/libraryfolders.vdf";
    if (QFile::exists(configPath)) {
        steamPath = homePath + "/.steam/steam";
    }
#endif
    return steamPath;
}

void MainWindow::GetSteamExecutable(){
    QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
    config->beginGroup("Path");
    QString dirPath = config->value("Executable").toString();
    config->endGroup();
    if(dirPath.isEmpty() || !QFile(dirPath).exists()){

        QString path = GetSteamPath();
        if (QDir(path).exists()){
            if (QDir(path + "/steamapps/common/The Binding of Isaac Rebirth").exists()) {
                gameDir = path + "/steamapps/common/The Binding of Isaac Rebirth";
                gameExec = "isaac-ng.exe";
    #ifdef Q_OS_LINUX
                if (QFile(gameDir + "/isaac-ng.exe").exists()){
                    gameExec = "isaac-ng.exe";
                }else{
                    gameExec = "isaac.x64";
                }
    #endif
                CheckDLCandStore(QFile(gameDir + "/" + gameExec));
            }
        }
    }
}

QString MainWindow::getModPath() {
    if (gameDLC.contains("Repentance")){
        RepentanceMod:
        return gameDir + "/mods";
    }else if(gameDLC == "Afterbirth+"){
        QString directory;
#ifdef Q_OS_WIN
        directory = QString(getenv("USERPROFILE")) + "/Documents/My Games/Binding of Isaac Afterbirth+";
        if(gameStore == "GOG"){
            directory += " (Galaxy)";
        }
        directory += " Mods";
#elif defined(Q_OS_LINUX)
        if(gameDLC.contains("Repentance")) goto RepentanceMod;
        directory = QDir::homePath();
        if(gameExec.contains(".exe")){
            directory = "$WINEPREFIX/drive_c/users/$USER/Documents/My Games/Binding of Isaac Afterbirth+";
            if(gameStore == "GOG"){
                directory += " (Galaxy)";
            }
            directory += " Mods";
        }else{
            directory += "/.local/share/binding of isaac afterbirth+ mods";
        }
#endif
        return directory;
    }

    return NULL;
}

QString MainWindow::GetIniFileLocation(){
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath();
#elif defined(Q_OS_LINUX)
    return QDir::homePath() + "/.local/share";
#endif
}

void MainWindow::GetWineApp(){
    if(linuxWineApp.isEmpty() || linuxWineApp == ""){
        QProcess process;
        process.start("which", {"wine"});
        process.waitForFinished();

        QString output = process.readAllStandardOutput().trimmed();
        if(!output.isEmpty()){
            QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
            config->beginGroup("Path");
            config->setValue("WinePath", output);

            linuxWineApp = output;
            config->endGroup();
        }else{
            QMessageBox::information(this, this->windowTitle(), "Wine app was not found.");
            return;
        }
    }
}

void MainWindow::GetWinePrefix(){
    if(linuxWinePrefix.isEmpty() || linuxWinePrefix == ""){
        linuxWinePrefix = QString(getenv("WINEPREFIX"));
        if(linuxWinePrefix.isEmpty()){
            linuxWinePrefix = QDir::homePath() + "/.wine";
        }
        QSettings *config = new QSettings(GetIniFileLocation() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Path");
        config->setValue("WinePrefix", linuxWinePrefix);
        config->endGroup();
    }
}
