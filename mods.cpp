#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlStreamReader>
#include <QTableWidgetItem>

void MainWindow::SortLineEdit(){
    QString str = ui->lineEdit->text();
    int modRow = std::abs(ui->modsButtonGroup->checkedId() + 1);
    for (int i = 0; i < ui->tableMods->rowCount(); ++i) {
        if (str == ""){
            ui->tableMods->showRow(i);
        }
        else
        {
            if (ui->tableMods->item(i,modRow)->text().contains(str, Qt::CaseInsensitive)){
                ui->tableMods->showRow(i);
            }
            else{
                ui->tableMods->hideRow(i);
            }
        }
    }
}

void MainWindow::SyncMods(QString directory){

    for (int i = 0; i < ui->tableMods->rowCount(); ++i) {

        QString folder = ui->tableMods->item(i,2)->text();
        QWidget *widget = ui->tableMods->cellWidget(i, 0);
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
        if (QFile::exists(directory + "/" + folder + "/disable.it")){
            checkBox->setCheckState(Qt::Unchecked);
        }else{
            checkBox->setCheckState(Qt::Checked);
        }
    }
    //ui->tableMods->sortItems(1);
}

void MainWindow::loadMods(QString directory) {
    QDir dir(directory);
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->tableMods->setColumnCount(3);
    ui->tableMods->setRowCount(folders.length());
    ui->tableMods->setSortingEnabled(true);
    ui->tableMods->setHorizontalHeaderLabels(modTableHeaders);
    ui->tableMods->clear();
    ui->tableMods->setHorizontalHeaderLabels(modTableHeaders);

    for (int i = 0; i < folders.size(); ++i) {

        QString folder = folders.at(i);

        QTableWidgetItem *item = new QTableWidgetItem(folder);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->tableMods->setItem(i,2,item);

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
                if(xml.hasError()){
                    QTableWidgetItem *failitem = new QTableWidgetItem(folder);
                    failitem->setFlags(failitem->flags() ^ Qt::ItemIsEditable);
                    ui->tableMods->setItem(i,1,failitem);
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

    connect(ui->activateButton, &QPushButton::clicked, this, [=](){
        //SyncMods(getModPath());
        for(int i=0; i < ui->tableMods->rowCount(); ++i){
            QWidget *widget = ui->tableMods->cellWidget(i, 0);
            QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
            QString folder = ui->tableMods->item(i,2)->text();
            if(checkBox->checkState() == Qt::Unchecked){
                if (QFile::exists(directory + "/" + folder + "/disable.it")) {
                    QFile::remove(directory + "/" + folder + "/disable.it");
                    checkBox->setCheckState(Qt::Checked);
                }
            }
        }
    });

    connect(ui->deactivateButton, &QPushButton::clicked, this, [=](){
        //SyncMods(getModPath());
        for(int i=0; i < ui->tableMods->rowCount(); ++i){
            QWidget *widget = ui->tableMods->cellWidget(i, 0);
            QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
            QString folder = ui->tableMods->item(i,2)->text();
            if(checkBox->checkState() == Qt::Checked && !QFile::exists(directory + "/" + folder + "/disable.it")){
                QFile file(directory + "/" + folder + "/disable.it");
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                file.close();
                checkBox->setCheckState(Qt::Unchecked);
            }
        }
    });
    //ui->tableMods->sortItems(1);
    ui->tableMods->resizeColumnToContents(0);
    ui->tableMods->resizeColumnToContents(1);
    ui->tableMods->resizeColumnToContents(2);
    ui->tableMods->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableMods->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
}
