#include "mainwindow.h"
#include <QTextStream>

QString MainWindow::IsaacDLC(QString directory)
{
    if (currentDLCName.isEmpty()){
        currentDLCName = "Rebirth";
        QFile isaacng(directory+"/isaac-ng.exe");
        if (!isaacng.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return "Rebirth"; // File couldn't be opened
        }
        QTextStream in(&isaacng);
        QString line = in.readAll();
        in.flush();
        isaacng.close();
        QString gameNamePrefix("Binding of Isaac: ");
        QStringList DLCNames = {"Repentance+", "Repentance", "Afterbirth+", "Afterbirth"};
        foreach (QString var, DLCNames){
            if (line.contains(gameNamePrefix + var)){
                currentDLCName = var;
                break;
            }
        }
    }
    return currentDLCName;
}

QString MainWindow::GetExeName(){
    QString gameExe = "";
#ifdef Q_OS_WINDOWS
    gameExe = "isaac-ng.exe";
#elif defined(Q_OS_LINUX)
    if (IsaacDLC(GetFullDir()) == "Repentance"){
        gameExe = "isaac-ng.exe";
    }else{
    #ifdef Q_PROCESSOR_X86_64
        gameExe = "isaac.x64";
    #elif defined(Q_PROCESSOR_X86)
        gameExe = "isaac.i386";
    #endif
    }
#endif
    return gameExe;
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

QString MainWindow::GetFullDir(){
    return GetSteamPath() + "/steamapps/common/The Binding of Isaac Rebirth";
}

QString MainWindow::getModPath() {
    QString GameDLC;
    QString FullDir = GetFullDir();

    GameDLC = IsaacDLC(FullDir);
    this->setWindowTitle("Isaac Configurator: " + GameDLC);
    if (GameDLC == "Repentance"){
        return FullDir + "/mods";
    }else if(GameDLC == "Repentance+"){
        QString directory;
#ifdef Q_OS_WIN
        directory = QString(getenv("USERPROFILE"))+"/Documents/My Games/Binding of Isaac Repentance+";
#elif defined(Q_OS_LINUX)
        directory = QString(getenv("HOME"))+"/.local/share/binding of isaac repentance+";
#endif
        return directory + "/mods";
    }else if(GameDLC == "Afterbirth+"){
        QString directory;
#ifdef Q_OS_WIN
        directory = QString(getenv("USERPROFILE"))+"/Documents/My Games/Binding of Isaac Afterbirth+ Mods";
#elif defined(Q_OS_LINUX)
        directory = QString(getenv("HOME"))+"/.local/share/binding of isaac afterbirth+ mods";
#endif
        return directory;
    }

    return NULL;
}
