#include "settings.h"
#include "ui_settings.h"
#include "../include/AudioIO.hh"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    std::vector<AudioInOut::ApiInfo> apiInf = AudioInOut::getHostApis();
    //std::vector<AudioInOut::DevInfo> devInf;
    MainWindow* mainWindow = (MainWindow*)this->parent();

    ui->driverCombo->clear();
    ui->deviceCombo->clear();
    for(unsigned int i=0; i<apiInf.size(); ++i){
        if(apiInf[i].devices.size() != 0)
            ui->driverCombo->addItem( apiInf[i].apiName.c_str(), QVariant(true) );
        else
            ui->driverCombo->addItem( apiInf[i].apiName.c_str(), QVariant(false) );
    }
    ui->driverCombo->setCurrentIndex( mainWindow->getDriverIndex() );
    //devInf = apiInf[ ui->driverCombo->currentIndex() ].devices;

    ui->deviceCombo->setCurrentIndex( mainWindow->getDeviceIndex() );


}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_OK_clicked()
{
    int driverIndex = ui->driverCombo->currentIndex();
    bool driverHasDevices = ui->driverCombo->itemData(driverIndex).toBool();
    int deviceMenuIndex;
    int deviceAudioIndex;

    if(!driverHasDevices){
        QMessageBox err;
        err.critical(this, "Error", "The selected driver has no available devices!");
        return;
    }
    deviceMenuIndex = ui->deviceCombo->currentIndex();
    deviceAudioIndex = ui->deviceCombo->itemData(deviceMenuIndex).toInt();
    ( (MainWindow*)parent() )->setDriverIndex(driverIndex);
    ( (MainWindow*)parent() )->setDeviceIndex(deviceAudioIndex);
    close();
}

void Settings:: on_driverCombo_currentIndexChanged(int index)
{
    // Fill device combo with only output devicees. The data states the index of the audio device.
    std::vector<AudioInOut::ApiInfo> apiInf = AudioInOut::getHostApis();  
    std::vector<AudioInOut::DevInfo> devInf = apiInf[index].devices;
    ui->deviceCombo->clear();
    if(devInf.size() > 0){
        for(unsigned int i=0; i<devInf.size(); ++i){
            if(devInf[i].numOutputs != 0)
                ui->deviceCombo->addItem( devInf[i].devName.c_str(), QVariant(i) );
        }
    }
}
