#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTranslator>
#include <QMapIterator>
#include <QSettings>


QMap<QString, QString> langs;

void MainWindow::retranslateStrings(QString translator){
    optionMessage1 = tr("Can't find file");
    optionMessage2 = tr("File 'options.ini' doesn't exists. Try launching game first");
    modTableHeaders = QStringList() << tr("Active") << tr("Mod Name") <<  tr("Folder");
    ui->tableMods->setHorizontalHeaderLabels(modTableHeaders);
    openDirName = tr("Open Directory");
    modMessage1 = tr("No mod folder");
    modMessage2 = tr("Couldn't locate mod folder. Please, make sure you have Afterbirth+ or Repentance installed");
    gameMessage1 = tr("No game found");
    gameMessage2 = tr("Install game first before running app");
    if(QFile::exists(QApplication::applicationDirPath() + "/imc.ini")){
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/imc.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        config->setValue("Language",translator);
        config->endGroup();
        config->sync();
    }
}

void MainWindow::initLanguages(QString translation){
    QTranslator translator;
    langs.insert("ru_RU","Русский");
    QMapIterator<QString, QString> i(langs);

    while (i.hasNext()) {
        i.next();
        QString baseName = i.key();
        if (translator.load(":/i18n/imc_" + baseName)) {
            QString langN = i.value();
            QAction *action = new QAction(langN,this);
            action->setCheckable(true);
            connect(action, &QAction::triggered, this, [=](){

                for(QAction *act: ui->menuLanguage->actions()){
                    act->setChecked(false);
                }
                action->setChecked(true);
                if (currentTranslator != baseName){
                    QTranslator translator;
                    if(translator.load(":/i18n/imc_" + baseName)){

                        qApp->installTranslator(&translator);

                        // Retranslate the UI elements
                        ui->retranslateUi(this);
                        retranslateStrings(baseName);
                        ui->tableMods->resizeColumnToContents(0);
                    }
                    currentTranslator = baseName;
                }
            });
            ui->menuLanguage->addAction(action);
            if (translation == baseName){
                action->setChecked(true);
                qApp->installTranslator(&translator);
                ui->retranslateUi(this);
                retranslateStrings(baseName);
                ui->tableMods->resizeColumnToContents(0);
            }
        }
    }
    connect(ui->actionEnglish, &QAction::triggered, this, [=](){

        for(QAction *act: ui->menuLanguage->actions()){
            act->setChecked(false);
        }
        ui->actionEnglish->setChecked(true);
        if (currentTranslator != "en_EN"){
            QTranslator translator;
            if(translator.load(":/i18n/imc_" + currentTranslator)){

                qApp->removeTranslator(&translator);

                // Retranslate the UI elements
                ui->retranslateUi(this);
                retranslateStrings("en_EN");
                ui->tableMods->resizeColumnToContents(0);
            }
            currentTranslator = "en_EN";
        }
    });
    if (translation == "en_EN"){
        ui->actionEnglish->setChecked(true);
    }else{
        ui->actionEnglish->setChecked(false);
    }
    retranslateStrings(translation);
}
