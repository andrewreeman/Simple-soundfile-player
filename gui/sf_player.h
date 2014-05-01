#ifndef SF_PLAYER_H
#define SF_PLAYER_H

#include <QObject>

class SF_Player : public QObject
{
    Q_OBJECT
public:
    explicit SF_Player(QObject *parent = 0);
    int m_api;
    int m_device;
    QString m_filePath;

signals:
    void finished();

public slots:
    void play();


};

#endif // SF_PLAYER_H
