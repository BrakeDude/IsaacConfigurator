#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    if(QFile::exists(QApplication::applicationDirPath() + "/IsaacConfigurator.ini")){
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        config->setValue("Language",translator);
        config->endGroup();
        config->sync();
    }
    ui->label_FindBy->resize(ui->label_FindBy->sizeHint().width(),ui->label_FindBy->height());
    ui->modRadioButton_Name->move(ui->label_FindBy->pos().rx() + ui->label_FindBy->width(), ui->modRadioButton_Name->pos().ry());
    ui->modRadioButton_Name->resize(ui->modRadioButton_Name->sizeHint().width(),ui->modRadioButton_Name->height());
    ui->modRadioButton_Folder->move(ui->modRadioButton_Name->pos().rx() + ui->modRadioButton_Name->width(), ui->modRadioButton_Folder->pos().ry());
    ui->modRadioButton_Folder->resize(ui->modRadioButton_Folder->sizeHint().width(),ui->modRadioButton_Folder->height());
    ui->lineEdit->move(ui->modRadioButton_Folder->pos().rx() + ui->modRadioButton_Folder->width(), ui->lineEdit->pos().ry());
    ui->lineEdit->setGeometry(ui->lineEdit->pos().x(), ui->lineEdit->pos().y(), ui->groupModsBox->size().width() - ui->lineEdit->pos().x() - 10, ui->lineEdit->height());
    ui->checkBoxLogUpdate->resize(ui->checkBoxLogUpdate->sizeHint().width(),ui->checkBoxLogUpdate->height());
    ui->pushButtonLogUpdate->move(ui->checkBoxLogUpdate->pos().rx() + ui->checkBoxLogUpdate->width() + 20, ui->pushButtonLogUpdate->pos().ry());
    ui->savePresetButton->resize(ui->savePresetButton->sizeHint().width(),ui->savePresetButton->height());
    ui->loadPresetButton->resize(ui->loadPresetButton->sizeHint().width(),ui->loadPresetButton->height());
    ui->loadPresetButton->move(ui->savePresetButton->pos().rx() + ui->savePresetButton->width() + 5, ui->loadPresetButton->pos().ry());
    ui->checkBox_JEcontrols->resize(ui->checkBox_JEcontrols->sizeHint().width(),ui->checkBox_JEcontrols->height());
    ui->label_PopUp->resize(ui->label_PopUp->sizeHint().width(),ui->label_PopUp->height());
    ui->checkBox_WaterSurface->resize(ui->checkBox_WaterSurface->sizeHint().width(),ui->checkBox_WaterSurface->height());
}

void MainWindow::initLanguages(QString translation){
    QTranslator translator;
    langs.insert("ru_RU","Русский");
    langs.insert("es_ES","Español");
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
                        ui_about->retranslateUi(aboutDialog);
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
                ui_about->retranslateUi(aboutDialog);
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
                ui_about->retranslateUi(aboutDialog);
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
