#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), apiIndex(0), deviceIndex(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDeviceIndex(int device){
    deviceIndex = device;
}

void MainWindow::setDriverIndex(int driver){
    apiIndex = driver;
}

void MainWindow::on_pushButton_clicked()
{
    // Eventually to a C string!
    AudioInOut::AudioIOType api = AudioInOut::intToAudioIOType(apiIndex - 1);
    playSoundFile(m_filePath.toStdString().c_str(), api, deviceIndex-1);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filter = tr(
        "Audio (*.aiff *.au *.avr *.caf *.flac *.htk *.iff *.mat *.mpc *.oga *.paf *.pvf *.rf64 *.sf *.voc *.w64 *.wav *.wavex *.wve *xi) "
    );
    QFileDialog dialog(this); // who the daddy?
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDirectory(QDir::currentPath());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);

    // expand this
    dialog.setNameFilter(filter);
    if(dialog.exec())
        m_filePath = (dialog.selectedFiles()).at(0); // first selected file
}

void MainWindow::on_actionSettings_triggered(){
    Settings settings;
    settings.setModal(true);
    settings.setParent(this);
    settings.exec();
}




