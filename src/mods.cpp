#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlStreamReader>
#include <QTableWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "headers/qtablewidgetcheckbox.h"

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


void MainWindow::ReSyncMods()
{
    QString str = getModPath();
    if(!ui->tableMods->isEnabled() || !ui->tabBox_ModsLog->isEnabled()){
        if(ui->tableMods->rowCount() > 0){
            ui->tableMods->clear();
        }
        return;
    }
    if(str.isNull()){
        if(ui->tableMods->rowCount() > 0){
            ui->tableMods->clear();
        }
    }else{
        SyncMods(str, false);
    }
}

void MainWindow::SyncMods(QString directory, bool force, bool message) {
    QDir dir(directory);
    if((!dir.exists() || !HasModSupport()) && ui->tableMods->isEnabled()){
        ui->modRadioButton_Folder->setEnabled(false);
        ui->modRadioButton_Name->setEnabled(false);
        ui->activateButton->setEnabled(false);
        ui->deactivateButton->setEnabled(false);
        ui->savePresetButton->setEnabled(false);
        ui->loadPresetButton->setEnabled(false);
        ui->pushButton_UpdateMods->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->tableMods->setEnabled(false);
        ui->tableMods->clear();
        if (HasModSupport() && message)
        {
            QMessageBox::information(this, this->windowTitle(), modMessage1);
        }
        return;
    }else{
        ui->modRadioButton_Folder->setEnabled(true);
        ui->modRadioButton_Name->setEnabled(true);
        ui->activateButton->setEnabled(true);
        ui->deactivateButton->setEnabled(true);
        ui->savePresetButton->setEnabled(true);
        ui->loadPresetButton->setEnabled(true);
        ui->pushButton_UpdateMods->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        ui->tableMods->setEnabled(true);
    }
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    if(folders.length() != ui->tableMods->rowCount() || force){
        RebuildModsList:
        ui->tableMods->setColumnCount(3);
        ui->tableMods->setRowCount(folders.length());
        ui->tableMods->setSortingEnabled(true);
        ui->tableMods->setHorizontalHeaderLabels(modTableHeaders);
        int sortedColumn = ui->tableMods->horizontalHeader()->sortIndicatorSection();
        Qt::SortOrder sortOrder = ui->tableMods->horizontalHeader()->sortIndicatorOrder();
        sortedColumn = (sortedColumn > -1 && sortedColumn <  ui->tableMods->columnCount()) ? sortedColumn : 0;
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
        if (sortedColumn == 0){
            ui->tableMods->sortItems(1, Qt::SortOrder::AscendingOrder);
        }
        ui->tableMods->sortItems(sortedColumn, sortOrder);

        ui->tableMods->resizeColumnsToContents();
        ui->tableMods->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->tableMods->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    }else{
        for (int i = 0; i < ui->tableMods->rowCount(); ++i) {
            QString folder = ui->tableMods->item(i, 2)->text();
            if(QDir(directory + "/" + folder).exists()){
                QCheckBox *checkbox = dynamic_cast<QCheckBox *>(ui->tableMods->cellWidget(i, 0));
                if (QFile::exists(directory + "/" + folder + "/disable.it")){
                    checkbox->setCheckState(Qt::Unchecked);
                }else{
                    checkbox->setCheckState(Qt::Checked);
                }

            }else goto RebuildModsList;
        }
    }
}

bool loaded = false;

void MainWindow::loadMods(QString directory) {

    SyncMods(directory, true, true);
    if (!loaded){
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
            SyncMods(getModPath(), true);
            SortLineEdit();
        });

        connect(ui->savePresetButton, &QPushButton::clicked, this, [=](){
            if (ui->tableMods->rowCount() > 0){
                QString param = "(*.json *.txt)";
                QString filePath = QFileDialog::getSaveFileName(this, saveFileText, "", fileFiterText, &param);

                if (!filePath.isEmpty()) {
                    QFile file(filePath);
                    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QFileInfo fileInto(file);
                        if(fileInto.suffix() == "json"){
                            QJsonObject jsonObject;
                            QJsonArray jsonArray;

                            for (int i = 0; i < ui->tableMods->rowCount(); ++i) {
                                QWidget *widget = ui->tableMods->cellWidget(i, 0);
                                QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                                if (checkBox->checkState() == Qt::Checked){
                                    QJsonObject item;
                                    item["folder"] = ui->tableMods->item(i,2)->text();
                                    item["name"] = ui->tableMods->item(i,1)->text();
                                    jsonArray.append(item);
                                }
                            }

                            jsonObject["mods"] = jsonArray;

                            QJsonDocument jsonDoc(jsonObject);
                            QByteArray jsonData = jsonDoc.toJson();
                            file.write(jsonData);
                            file.close();
                        }else if(QFileInfo(filePath).suffix() == "txt"){
                            QStringList list;
                            QTextStream stream(&file);
                            for (int i = 0; i < ui->tableMods->rowCount(); ++i) {
                                QWidget *widget = ui->tableMods->cellWidget(i, 0);
                                QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                                if (checkBox->checkState() == Qt::Checked){
                                    list.append(ui->tableMods->item(i,2)->text());
                                }
                            }
                            list.sort(Qt::CaseInsensitive);
                            stream << list.join("\n");
                            file.close();
                        }else{
                            file.close();
                            QMessageBox::information(this, this->windowTitle(), "Can't save to unsopported format");
                        }
                    }
                }
            }
        });

        connect(ui->loadPresetButton, &QPushButton::clicked, this, [=](){
            if (ui->tableMods->rowCount() > 0){
                QString param = "(*.json *.txt)";
                QString fileName = QFileDialog::getOpenFileName(nullptr, openFileText, "",  fileFiterText, &param);
                QFile file(fileName);
                if (!file.open(QIODevice::ReadOnly)) {
                    qWarning("Couldn't open file.");
                    return;
                }

                QFileInfo fileInfo(file);

                QByteArray data = file.readAll();
                QMap<QString, int> mods = QMap<QString, int>{};
                for (int row = 0; row < ui->tableMods->rowCount(); ++row) {
                    QTableWidgetItem *item = ui->tableMods->item(row, 2);
                    if (item)
                    {
                        mods.insert(item->text(), row);
                    }
                }
                if(fileInfo.suffix() == "json"){
                    QJsonDocument doc(QJsonDocument::fromJson(data));
                    QJsonObject json = doc.object();

                    QJsonArray items = json["mods"].toArray();

                    ui->deactivateButton->click();
                    for (const QJsonValueRef item : items) {
                        QJsonObject obj = item.toObject();
                        QString folder = obj["folder"].toString();

                        if (mods.contains(folder) && ((obj["active"].isBool() && obj["active"].toBool()) || obj["active"].isNull()) ) {
                            QWidget *widget = ui->tableMods->cellWidget(mods.value(folder), 0);
                            QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                            checkBox->setCheckState(Qt::Checked);
                        }
                    }
                }else if(fileInfo.suffix() == "txt"){
                    ui->deactivateButton->click();
                    QStringList modList = QString(data).split("\r\n");
                    for(QString modFolder : modList){
                        if (mods.contains(modFolder)){
                            QWidget *widget = ui->tableMods->cellWidget(mods.value(modFolder), 0);
                            QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
                            checkBox->setCheckState(Qt::Checked);
                        }
                    }
                }else {
                    QMessageBox::information(this, this->windowTitle(), "Can't load from unsopported format");
                }
                file.close();
            }
        });
        loaded = true;
    };
}

bool MainWindow::HasModSupport()
{
    return gameDLC == "Afterbirth+" || gameDLC == "Repentance" || gameDLC == "Repentance+";
}
