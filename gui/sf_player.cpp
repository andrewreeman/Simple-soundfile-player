#include "sf_player.h"
#include "../include/SoundFile.hh"
#include <QDebug>

SF_Player::SF_Player(QObject *parent) :
    QObject(parent)
{
}

void SF_Player::play(){
    AudioInOut::AudioIOType api = AudioInOut::intToAudioIOType(m_api);
    playSoundFile(m_filePath.toStdString().c_str(), api, m_device);
    emit finished();
}
