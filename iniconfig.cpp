#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QDebug>

void MainWindow::ConfigIniLoad(){
    QString configIniDir = GetFullDir() + "/resources";
    if (!QFile::exists(configIniDir + "/config.ini")) {
        QSettings *settings = new QSettings(configIniDir + "/config.ini", QSettings::IniFormat);
        settings->beginGroup("Options");
        settings->setValue("EnableColorCorrection",1);
        settings->setValue("EnableCaustics",1);
        settings->setValue("EnableShockwave",1);
        settings->setValue("EnableLighting",1);
        settings->setValue("EnableFilter",1);
        settings->setValue("EnablePixelation",1);
        settings->setValue("EnableBloom",1);
        settings->setValue("EnableWaterSurface",1);
        settings->setValue("EnableInterpolation",1);
        settings->endGroup();
        settings->sync();
    }
    QSettings *settings = new QSettings(configIniDir + "/config.ini", QSettings::IniFormat);

    ReSyncConfigIni(settings);

    connect(ui->checkBox_ColorCorrection, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableColorCorrection",0);
        } else {
            settings->setValue("EnableColorCorrection",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_Caustics, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableCaustics",0);
        } else {
            settings->setValue("EnableCaustics",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_Shockwave, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableShockwave",0);
        } else {
            settings->setValue("EnableShockwave",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_Lighting, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableLighting",0);
        } else {
            settings->setValue("EnableLighting",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_EffectsFilter, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableFilter",0);
        } else {
            settings->setValue("EnableFilter",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_Pixelation, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnablePixelation",0);
        } else {
            settings->setValue("EnablePixelation",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_Bloom, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableBloom",0);
        } else {
            settings->setValue("EnableBloom",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_WaterSurface, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableWaterSurface",0);
        } else {
            settings->setValue("EnableWaterSurface",1);
        }
        settings->endGroup();
        settings->sync();
    });

    connect(ui->checkBox_Interpolation, &QCheckBox::stateChanged, this, [=](int state) {
        settings->beginGroup("Options");
        if (state == Qt::Unchecked) {
            settings->setValue("EnableInterpolation",0);
        } else {
            settings->setValue("EnableInterpolation",1);
        }
        settings->endGroup();
        settings->sync();
    });

}

void MainWindow::ReSyncConfigIni(QSettings *settings){
    settings->beginGroup("Options");

    if (settings->value("EnableColorCorrection") == 1) {
        ui->checkBox_ColorCorrection->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_ColorCorrection->setCheckState(Qt::Unchecked);
    }

    if (settings->value("EnableCaustics") == 1) {
        ui->checkBox_Caustics->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Caustics->setCheckState(Qt::Unchecked);
    }

    if (settings->value("EnableShockwave") == 1) {
        ui->checkBox_Shockwave->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Shockwave->setCheckState(Qt::Unchecked);
    }

    if (settings->value("EnableLighting") == 1) {
        ui->checkBox_Lighting->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Lighting->setCheckState(Qt::Unchecked);
    }

    if (settings->value("EnableFilter") == 1) {
        ui->checkBox_EffectsFilter->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_EffectsFilter->setCheckState(Qt::Unchecked);
    }

    if (settings->value("EnablePixelation") == 1) {
        ui->checkBox_Pixelation->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Pixelation->setCheckState(Qt::Unchecked);
    }

    if (settings->value("EnableBloom") == 1) {
        ui->checkBox_Bloom->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Bloom->setCheckState(Qt::Unchecked);
    }

    if (settings->value("EnableWaterSurface") == 1) {
        ui->checkBox_WaterSurface->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_WaterSurface->setCheckState(Qt::Unchecked);
    }
    if (settings->value("EnableInterpolation") == 1) {
        ui->checkBox_Interpolation->setCheckState(Qt::Checked);
    }else{
        ui->checkBox_Interpolation->setCheckState(Qt::Unchecked);
    }

    settings->endGroup();
}
