#include "settings.h"
#include "ui_settings.h"
#include "../include/AudioIO.hh"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->driverCombo->addItem("Default");
    std::vector<AudioInOut::ApiInfo> apiInf = AudioInOut::getHostApis();
    for(unsigned int i=0; i<apiInf.size(); ++i){
        ui->driverCombo->addItem( apiInf[i].apiName.c_str() );
    }


}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_OK_clicked()
{
    ( (MainWindow*)parent() )->setDeviceIndex(0);
    ( (MainWindow*)parent() )->setDriverIndex(0);
    TODO MAINWINDOW NOT FOUMD
}
