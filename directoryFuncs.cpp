#include "mainwindow.h"
#include <QTextStream>
#include <QDir>
#include <QFileInfo>

void MainWindow::CheckDLCandStore(QFile isaacng)
{
    if (isaacng.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&isaacng);
        QString line = in.readAll();
        in.flush();
        isaacng.close();
        QString gameNamePrefix("Binding of Isaac: ");
        QStringList DLCNames = {"Repentance+", "Repentance", "Afterbirth+", "Afterbirth"};
        QFileInfo isaacInfo = QFileInfo(isaacng);
        QString dir = isaacInfo.dir().path();
        gameExec = isaacInfo.fileName();
        if (line.contains("(Galaxy)")){
            gameStore = "GOG";
            this->setWindowTitle("Isaac Configurator: " + gameDLC);
        }else if (!GetSteamPath().isEmpty() && QDir(GetSteamPath() + "/steamapps/common/The Binding of Isaac Rebirth").exists()
            && isaacInfo.dir() == GetSteamPath() + "/steamapps/common/The Binding of Isaac Rebirth")
        {
            gameStore = "Steam";
        }
        UpdateGameDir(dir);
        foreach (QString var, DLCNames){
            if (line.contains(gameNamePrefix + var)){
                gameDLC = var;
                this->setWindowTitle("Isaac Configurator: " + gameDLC + " (" + gameStore + ")");
                return;
            }
        }
    }
    gameDLC = "Rebirth";
    this->setWindowTitle("Isaac Configurator: " + gameDLC);
}

void MainWindow::UpdateGameDir(QString dir)
{
    gameDir = dir;
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
    QString path = GetSteamPath();
    if (QDir(path).exists()){
        if (QDir(path + "/steamapps/common/The Binding of Isaac Rebirth").exists()) {
            gameDir = path + "/steamapps/common/The Binding of Isaac Rebirth";
            gameExec = "isaac-ng.exe";
            CheckDLCandStore(QFile(gameDir + "/" + gameExec));
        }
    }
}

QString MainWindow::getModPath() {
    if (gameDLC == "Repentance" || gameDLC == "Repentance+"){
        return gameDir + "/mods";
    }else if(gameDLC == "Afterbirth+"){
        QString directory;
#ifdef Q_OS_WIN
        directory = QString(getenv("USERPROFILE"))+"/Documents/My Games/Binding of Isaac Afterbirth+";
        if(gameStore == "GOG"){
            directory += " (Galaxy)";
        }
        directory += " Mods";
#elif defined(Q_OS_LINUX)
        directory = QString(getenv("HOME"))+"/.local/share/binding of isaac afterbirth+ mods";
#endif
        return directory;
    }

    return NULL;
}
