#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;

private slots:
    void on_OK_clicked();
    void on_driverCombo_currentIndexChanged(int index);
};

#endif // SETTINGS_H
