#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QTableWidgetItem>
#include <QTranslator>
#include <QLocale>

QStringList langs = QStringList{"ru_RU"};

void MainWindow::retranslateStrings(){
    optionMessage1 = tr("Can't find file");
    optionMessage2 = tr("File 'options.ini' doesn't exists. Try launching game first");
    modTableHeaders = QStringList() << tr("Active") << tr("Mod Name") <<  tr("Folder");
    ui->tableMods->setHorizontalHeaderLabels(modTableHeaders);
    openDirName = tr("Open Directory");
    modMessage1 = tr("No mod folder");
    modMessage2 = tr("Couldn't locate mod folder. Please, make sure you have Afterbirth+ or Repentance installed");
    gameMessage1 = tr("No game found");
    gameMessage2 = tr("Install game first before running app");
}

void MainWindow::initLanguages(){
    QTranslator translator;
    for (const QString &baseName : qAsConst(langs)) {
        QLocale *qloc = new QLocale(baseName);
        if (translator.load(":/i18n/imc_" + baseName)) {
            QString langN = qloc->nativeLanguageName();
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
                        retranslateStrings();
                        ui->tableMods->resizeColumnToContents(0);
                    }
                    currentTranslator = baseName;
                }
            });
            ui->menuLanguage->addAction(action);
        }
    }
    connect(ui->actionEnglish, &QAction::triggered, this, [=](){

        for(QAction *act: ui->menuLanguage->actions()){
            act->setChecked(false);
        }
        ui->actionEnglish->setChecked(true);
        if (!currentTranslator.isEmpty()){
            QTranslator translator;
            if(translator.load(":/i18n/imc_" + currentTranslator)){

                qApp->removeTranslator(&translator);

                // Retranslate the UI elements
                ui->retranslateUi(this);
                retranslateStrings();
                ui->tableMods->resizeColumnToContents(0);
            }
            currentTranslator = QString();
        }
    });
    retranslateStrings();
}
