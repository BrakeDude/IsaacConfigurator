#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDesktopServices>
#include <QScrollBar>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qApp->setStyle(QStyleFactory::create("fusion"));
    ui->setupUi(this);
    ui_about = new Ui::about();
    aboutDialog = new QDialog(this);
    aboutDialog->setWindowFlags(aboutDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui_about->setupUi(aboutDialog);
    timer = new QTimer(this);
    if (!QFile::exists(QApplication::applicationDirPath() + "/IsaacConfigurator.ini")) {
        QFile file(QApplication::applicationDirPath() + "/IsaacConfigurator.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        config->setValue("Language","en_EN");
        config->setValue("CheckUpdate", 1);
        config->setValue("ShowFullLog", 1);
        config->setValue("ShowErrors", 1);
        config->setValue("ShowWarnings", 1);
        config->setValue("ShowLuaDebug", 1);
        ui->checkBoxLogErrors->setEnabled(false);
        ui->checkBoxLogWarnings->setEnabled(false);
        ui->checkBoxLogLuaDebug->setEnabled(false);
        config->setValue("DarkMode", 0);
        config->setValue("DisableRepentogon", 0);
        config->endGroup();
        config->sync();
        currentTranslator = "en_EN";
    }else{
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");
        currentTranslator = config->value("Language").toString();

        Qt::CheckState checkState;
        if (config->value("CheckUpdate") == 0) {
            checkState = Qt::Unchecked;
        } else {
            checkState = Qt::Checked;
        }
        ui->checkBoxLogUpdate->setCheckState(checkState);

        if (config->value("ShowFullLog") == 0) {
            checkState = Qt::Unchecked;
        } else {
            checkState = Qt::Checked;
        }
        ui->checkBoxLogFull->setCheckState(checkState);

        if (checkState == Qt::Checked){
            ui->checkBoxLogErrors->setEnabled(false);
            ui->checkBoxLogWarnings->setEnabled(false);
            ui->checkBoxLogLuaDebug->setEnabled(false);
        }

        if (config->value("ShowErrors") == 0) {
            checkState = Qt::Unchecked;
        } else {
            checkState = Qt::Checked;
        }
        ui->checkBoxLogErrors->setCheckState(checkState);

        if (config->value("ShowWarnings") == 0) {
            checkState = Qt::Unchecked;
        } else {
            checkState = Qt::Checked;
        }
        ui->checkBoxLogWarnings->setCheckState(checkState);

        if (config->value("ShowLuaDebug") == 0) {
            checkState = Qt::Unchecked;
        } else {
            checkState = Qt::Checked;
        }
        ui->checkBoxLogLuaDebug->setCheckState(checkState);

        if(config->value("DisableRepentogon") == 1){
            ui->actionDisable_Repentogon->setChecked(true);
            ui->scrollArea_REPENTOGON->setEnabled(false);
        }else{
            ui->actionDisable_Repentogon->setChecked(false);
            ui->scrollArea_REPENTOGON->setEnabled(true);
        }
        ui->actionDark_theme->setChecked(config->value("DarkMode") == 1);
        DarkMode(config->value("DarkMode") == 1);
        config->endGroup();
        config->sync();
    }
    initLanguages(currentTranslator);

    connect(ui->checkBoxLogUpdate, &QCheckBox::stateChanged, this, [=](int state) {
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (state == 0) {
            config->setValue("CheckUpdate", 0);
        } else {
            config->setValue("CheckUpdate", 1);
        }
        config->endGroup();
        config->sync();
    });

    connect(ui->checkBoxLogFull, &QCheckBox::stateChanged, this, [=](int state) {
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (state == 0) {
            config->setValue("ShowFullLog", 0);
            ui->checkBoxLogErrors->setEnabled(true);
            ui->checkBoxLogWarnings->setEnabled(true);
            ui->checkBoxLogLuaDebug->setEnabled(true);
        } else {
            config->setValue("ShowFullLog", 1);
            ui->checkBoxLogErrors->setEnabled(false);
            ui->checkBoxLogWarnings->setEnabled(false);
            ui->checkBoxLogLuaDebug->setEnabled(false);
        }
        config->endGroup();
        config->sync();
        ui->logBrowser->clear();
        onFileLoaded(true);
    });

    connect(ui->checkBoxLogErrors, &QCheckBox::stateChanged, this, [=](int state) {
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (state == 0) {
            config->setValue("ShowErrors", 0);
        } else {
            config->setValue("ShowErrors", 1);
        }
        config->endGroup();
        config->sync();
        ui->logBrowser->clear();
        onFileLoaded(true);
    });

    connect(ui->checkBoxLogWarnings, &QCheckBox::stateChanged, this, [=](int state) {
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (state == 0) {
            config->setValue("ShowWarnings", 0);
        } else {
            config->setValue("ShowWarnings", 1);
        }
        ui->logBrowser->clear();
        config->endGroup();
        config->sync();
        ui->logBrowser->clear();
        onFileLoaded(true);
    });

    connect(ui->checkBoxLogLuaDebug, &QCheckBox::stateChanged, this, [=](int state) {
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (state == 0) {
            config->setValue("ShowLuaDebug", 0);
        } else {
            config->setValue("ShowLuaDebug", 1);
        }
        ui->logBrowser->clear();
        config->endGroup();
        config->sync();
        ui->logBrowser->clear();
        onFileLoaded(true);
    });

    connect(ui->actionDark_theme, &QAction::triggered, this, [=](){
        DarkMode(ui->actionDark_theme->isChecked());
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (ui->actionDark_theme->isChecked()) {
            config->setValue("DarkMode", 1);
        } else {
            config->setValue("DarkMode", 0);
        }
        config->endGroup();
        config->sync();
    });

    connect(ui->actionDisable_Repentogon, &QAction::triggered, this, [=](){
        QSettings *config = new QSettings(QApplication::applicationDirPath() + "/IsaacConfigurator.ini", QSettings::IniFormat);
        config->beginGroup("Options");

        if (ui->actionDisable_Repentogon->isChecked()) {
            config->setValue("DisableRepentogon", 1);
        } else {
            config->setValue("DisableRepentogon", 0);
        }
        ui->scrollArea_REPENTOGON->setEnabled(!ui->actionDisable_Repentogon->isChecked() && ui->scrollArea_VanillaOptions->isEnabled() && ui->tabWidget_Options->isEnabled());

        config->endGroup();
        config->sync();
    });

    connect(ui->checkBox_CaseSensitivity, &QCheckBox::stateChanged, this, [=](int state) {
        if (!ui->lineEditLog->text().isEmpty()){
            onFileLoaded(true);
        }
    });

    GetSteamExecutable();
    LoadApp(gameDir+"/"+gameExec);

    connect(ui->tableMods, &QTableWidget::itemDoubleClicked, ui->tableMods, [=](){
        QString folder = ui->tableMods->item(ui->tableMods->currentRow(), 2)->text();
        QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(getModPath()+"/"+folder)));
    });

    connect(ui->actionExit, &QAction::triggered, this, [=](){
        QApplication::quit();
    });

    connect(ui->actionOpen_config_folder, &QAction::triggered, this, [=](){
        if(QDir(configDir).exists()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(configDir)));
        }
    });

    connect(ui->actionOpen_game_folder, &QAction::triggered, this, [=](){
        if(QDir(gameDir).exists()){
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(gameDir)));
        }
    });

    connect(ui->actionFind_game_folder, &QAction::triggered, this, [=](){
        LoadApp(QFileDialog::getOpenFileName(this, openDirName,
                                                  gameDir,
                                                  QString("(isaac-ng*.exe)")));
    });


    connect(ui->actionStartGame, &QAction::triggered, this, [=](){
        QStringList options = QStringList();
        if (ui->actionDisable_Repentogon->isChecked()){
            options << "-repentogonoff";
        }
        #ifdef Q_OS_WINDOWS
            QProcess::startDetached(gameDir+"/"+gameExec,QStringList() << options);
        #elif defined(Q_OS_LINUX)
            if (gameStore == "Steam"){
                if(gameDLC == "Repentance" || gameDLC == "Repentance+"){
                    QProcess proton;
                    proton.setProgram(GetSteamPath() + "/compatibilitytools.d/proton/version/proton");
                    QStringList command;
                    command << "run"  << gameDir+"/"+gameExec << options;
                    proton.setArguments(command);
                    proton.startDetached();
                }else{
                    QProcess::startDetached("steam", QStringList() << "steam://rungameid/250900");
                }
            }
        #endif

    });
    connect(ui->actionCloseGame, &QAction::triggered, this, [=](){
        QProcess process;
        #ifdef Q_OS_WINDOWS
            process.start("taskkill", QStringList() << "/IM" << gameExec);
            process.waitForFinished();
        #elif defined(Q_OS_LINUX)
            QProcess::execute("pkill", QStringList() << gameExec);
        #endif
    });
    ui->scrollArea_VanillaOptions->setGeometry(ui->scrollArea_VanillaOptions->pos().x(), ui->scrollArea_VanillaOptions->pos().y(), ui->scrollArea_VanillaOptions->size().width() - 5, ui->scrollArea_VanillaOptions->size().height() + 5);
    timer->start(1000);
}

void MainWindow::DarkMode(bool dark){
    if(dark){
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(53,53,53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(15,15,15));
        palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53,53,53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);

        //palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::black);

        palette.setColor(QPalette::Disabled, QPalette::Window, QColor(90,90,90));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::Button, QColor(90,90,90));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255,128,128));
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(90,90,90));

        qApp->setPalette(palette);
    }else{
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(240,240,240));
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Base, QColor(255,255,255));
        palette.setColor(QPalette::AlternateBase, QColor(245,245,245));
        palette.setColor(QPalette::ToolTipBase, QColor(255,255,220));
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(240,240,240));
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::BrightText, Qt::white);

        //palette.setColor(QPalette::Highlight, QColor(0,120,215).lighter());
        palette.setColor(QPalette::HighlightedText, Qt::white);

        palette.setColor(QPalette::Disabled, QPalette::Window, QColor(220,220,220));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(0,0,0));
        palette.setColor(QPalette::Disabled, QPalette::Button, QColor(180,180,180));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(128,128,128));
        palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(220,220,220));
        palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(0,0,0));

        qApp->setPalette(palette);
    }
}

void MainWindow::LoadApp(QString FullDir){
    if (FullDir.isEmpty())
    {
        FullDir = gameDir + "/" + gameExec;
    }
    if (QFile::exists(FullDir)){
        CheckDLCandStore(FullDir);
        ui->tabBox_ModsLog->widget(0)->setEnabled(true);
        ui->tabWidget_Options->setEnabled(true);
        ui->menuGame->setEnabled(true);
        QString str = getModPath();
        if (!str.isNull()){
            loadMods(str);
        }else{
            ui->modRadioButton_Folder->setEnabled(false);
            ui->modRadioButton_Name->setEnabled(false);
            ui->activateButton->setEnabled(false);
            ui->deactivateButton->setEnabled(false);
            ui->savePresetButton->setEnabled(false);
            ui->loadPresetButton->setEnabled(false);
            ui->pushButton_UpdateMods->setEnabled(false);
            ui->lineEdit->setEnabled(false);
            ui->tableMods->setEnabled(false);
            if (HasModSupport()){
                QMessageBox::information(this, modMessage1, modMessage2);
            }
        }
        CheckDLCandStore(FullDir);
        LoadConfigFile();
        ConfigIniLoad();
    }else {
        QMessageBox::information(this, gameMessage1, gameMessage2);
        ui->modRadioButton_Folder->setEnabled(false);
        ui->modRadioButton_Name->setEnabled(false);
        ui->activateButton->setEnabled(false);
        ui->deactivateButton->setEnabled(false);
        ui->savePresetButton->setEnabled(false);
        ui->loadPresetButton->setEnabled(false);
        ui->pushButton_UpdateMods->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        ui->tableMods->setEnabled(false);
        ui->tabWidget_Options->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete ui;
}


void MainWindow::on_modRadioButton_Name_toggled(bool checked)
{
    if(checked){
        SortLineEdit();
    }
}


void MainWindow::on_modRadioButton_Folder_toggled(bool checked)
{
    if(checked){
        SortLineEdit();
    }
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    SortLineEdit();
}


void MainWindow::on_actionAbout_triggered()
{
    ui_about->labelAboutText->setFixedHeight(ui_about->labelAboutText->sizeHint().height());
    aboutDialog->setFixedSize(QSize(300, ui_about->labelAboutText->sizeHint().height() + ui_about->labelAboutName->sizeHint().height()));
    aboutDialog->exec();
}


void MainWindow::onFileLoaded(bool force)
{
    if(ui->checkBoxLogUpdate->checkState() == Qt::Checked || force){
        QFile file(configDir + "/log.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString content = in.readAll();
            file.close();
            QStringList lines = content.split("\n");

            lines.removeAll("");

            for (int i = 0; i < lines.size(); i++) {
                lines[i] = QString::number(i + 1) + ": " + lines[i];
            }

            if(!ui->lineEditLog->text().isEmpty()){
                lines = lines.filter(ui->lineEditLog->text(), ui->checkBox_CaseSensitivity->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
            }

            QString numberedContent;
            if(!ui->checkBoxLogFull->isChecked()){
                QStringList pattern;
                if(ui->checkBoxLogErrors->isChecked())
                {
                    pattern.append("Error in");
                    pattern.append("[ASSERT]");
                }
                if(ui->checkBoxLogWarnings->isChecked())
                {
                    pattern.append("[warn]");
                }
                if(pattern.empty())
                {
                    lines = QStringList();
                }else{
                    lines.erase(std::remove_if(lines.begin(), lines.end(),
                                          [&pattern](const QString &item) {
                                              return !std::any_of(pattern.begin(), pattern.end(),
                                                                 [&item](const QString &word) { return item.contains(word); });
                                          }),
                                lines.end());
                }
            }
            numberedContent = lines.join("\n");

            if (numberedContent != ui->logBrowser->toPlainText()) {
                ui->logBrowser->setPlainText(numberedContent);
                ui->logBrowser->verticalScrollBar()->setValue(ui->logBrowser->verticalScrollBar()->maximum());
                HighlightTextInLog();
            }
        }
    }
}

void MainWindow::HighlightTextInLog()
{
    QTextCursor cursor(ui->logBrowser->document());
    QTextCharFormat highlightError;
    QTextCharFormat highlightWarning;
    highlightError.setForeground(QColor(Qt::red));
    if (ui->actionDark_theme->isChecked()){
        highlightWarning.setForeground(QColor(Qt::yellow));
    }else{
        highlightWarning.setForeground(QColor(255,140,0));
    }
    cursor.movePosition(QTextCursor::Start);
    while(!cursor.atEnd()){
        cursor.movePosition(QTextCursor::StartOfBlock);  // Move to start of the line
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);  // Select the entire line

        if (cursor.selectedText().contains("Error in", Qt::CaseSensitive)
            or cursor.selectedText().contains("[ASSERT]")) {
            cursor.setCharFormat(highlightError);
        }else if ((cursor.selectedText().contains("[warn]", Qt::CaseSensitive))
                   || (cursor.selectedText().contains("Lua Debug:", Qt::CaseSensitive))) {
            cursor.setCharFormat(highlightWarning);
        } else {
            cursor.setCharFormat(QTextCharFormat());
        }

        cursor.movePosition(QTextCursor::NextBlock);
    }

}

void MainWindow::on_lineEditLog_textChanged(const QString &arg1)
{
    onFileLoaded(true);
}

