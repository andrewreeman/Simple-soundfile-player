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
    for(int i=0; i<apiInf.size(); ++i){
        ui->driverCombo->addItem( apiInf[i].apiName.c_str() );
    }


}

Settings::~Settings()
{
    delete ui;
}
