#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlStreamReader>
#include <QTableWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

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

void MainWindow::SyncMods(QString directory) {
    QDir dir(directory);
    if(!dir.exists()){
        ui->modRadioButton_Folder->setEnabled(false);
        ui->modRadioButton_Name->setEnabled(false);
        ui->activateButton->setEnabled(false);
        ui->deactivateButton->setEnabled(false);
        ui->savePresetButton->setEnabled(false);
        ui->loadPresetButton->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->tableMods->setEnabled(false);
        ui->tableMods->clear();
        QMessageBox::information(this, modMessage1, modMessage2);
        return;
    }else{
        ui->modRadioButton_Folder->setEnabled(true);
        ui->modRadioButton_Name->setEnabled(true);
        ui->activateButton->setEnabled(true);
        ui->deactivateButton->setEnabled(true);
        ui->savePresetButton->setEnabled(true);
        ui->loadPresetButton->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->tableMods->setEnabled(true);
    }
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->tableMods->setColumnCount(3);
    ui->tableMods->setRowCount(folders.length());
    ui->tableMods->setSortingEnabled(true);
    ui->tableMods->setHorizontalHeaderLabels(modTableHeaders);
    int sortedColumn = ui->tableMods->horizontalHeader()->sortIndicatorSection();
    Qt::SortOrder sortOrder = ui->tableMods->horizontalHeader()->sortIndicatorOrder();
    sortedColumn = sortedColumn > -1 ? sortedColumn : 1;
    ui->tableMods->sortItems(0, Qt::SortOrder::DescendingOrder);
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
                        if (xml.name() == (QString)"name") {
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
        }else{
            QTableWidgetItem *failitem = new QTableWidgetItem(folder);
            failitem->setFlags(failitem->flags() ^ Qt::ItemIsEditable);
            ui->tableMods->setItem(i,1,failitem);
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
    ui->tableMods->sortItems(sortedColumn, sortOrder);

    ui->tableMods->resizeColumnsToContents();
    ui->tableMods->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableMods->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
}

void MainWindow::loadMods(QString directory) {

    SyncMods(directory);

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

    connect(ui->pushButton_UpdateMods, &QPushButton::clicked, this, [=](){
        SyncMods(getModPath());
        SortLineEdit();
    });

    connect(ui->savePresetButton, &QPushButton::clicked, this, [=](){
        if (ui->tableMods->rowCount() > 0){
            QJsonObject jsonObject;
            QJsonArray jsonArray;

            for (int i = 0; i < ui->tableMods->rowCount(); ++i) {
                QJsonObject item;
                item["folder"] = ui->tableMods->item(i,2)->text();
                QWidget *widget = ui->tableMods->cellWidget(i, 0);
                QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                item["active"] = checkBox->checkState() == Qt::Checked ? true : false;
                jsonArray.append(item);
            }

            jsonObject["mods"] = jsonArray;

            QJsonDocument jsonDoc(jsonObject);
            QByteArray jsonData = jsonDoc.toJson();

            QString filePath = QFileDialog::getSaveFileName(this, saveFileText, "", jsonFilterText);

            if (!filePath.isEmpty()) {
                QFile file(filePath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    file.write(jsonData);
                    file.close();
                }
            }
        }
    });

    connect(ui->loadPresetButton, &QPushButton::clicked, this, [=](){
        if (ui->tableMods->rowCount() > 0){
            QString fileName = QFileDialog::getOpenFileName(nullptr, openFileText, "",  jsonFilterText);
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open file.");
                return;
            }

            QByteArray data = file.readAll();
            QJsonDocument doc(QJsonDocument::fromJson(data));
            QJsonObject json = doc.object();

            QJsonArray items = json["mods"].toArray();
            for (const QJsonValueRef item : items) {
                QJsonObject obj = item.toObject();
                QString folder = obj["folder"].toString();
                bool checked = obj["active"].toBool();
                // Do something with the loaded data
                int rowCount = ui->tableMods->rowCount();
                int targetRow = -1;

                for (int row = 0; row < rowCount; ++row) {
                    QTableWidgetItem *item = ui->tableMods->item(row, 2);
                    if (item && item->text() == folder) {
                        targetRow = row;
                        break;
                    }
                }

                if (targetRow != -1) {
                    QWidget *widget = ui->tableMods->cellWidget(targetRow, 0);
                    QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                    checkBox->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
                }
            }
        }
    });

}
