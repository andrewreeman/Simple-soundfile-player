// The main gui window. MOC is run on the q_object files to expand the Q_OBJECT macro's.


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/SoundFile.hh"

//#ifndef NDEBUG
    #include <iostream>
//#endif

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    /* Must add this for MOC (Meta-Object Compiler) to make this a Q_Object.
    Among other things this allows the signal and slot mechanism to work. */
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setDriverIndex(int driver);
    void setDeviceIndex(int device);


    
private slots:
    // A slot is basically a subscriber. A signal is a publisher.
    void on_pushButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSettings_triggered();

private:

    int apiIndex;
    int deviceIndex;
    Ui::MainWindow *ui;

    QString m_filePath;

};

#endif // MAINWINDOW_H
