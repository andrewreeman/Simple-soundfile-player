#include <QSettings>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), apiIndex(0), deviceIndex(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings(){
    QSettings settings("AReeman", "Simple Soundfile Player");
    m_lastDirectory = settings.value("lastDir", QDir::currentPath()).toString();
}

void MainWindow::writeSettings(){
    QSettings settings("AReeman", "Simple Soundfile Player");
    settings.setValue("lastDir", m_lastDirectory);
}


void MainWindow::setDeviceIndex(int device){
    deviceIndex = device;
    std::cout << "device index is: " << deviceIndex << std::endl;
    std::cout << "driver index is: " << apiIndex << std::endl;

}

void MainWindow::setDriverIndex(int driver){
    apiIndex = driver;
    std::cout << "device index is: " << deviceIndex << std::endl;
    std::cout << "driver index is: " << apiIndex << std::endl;
}



void MainWindow::on_pushButton_clicked()
{

    AudioInOut::AudioIOType api = AudioInOut::intToAudioIOType(apiIndex);
    std::cout << "Selected api is " << apiIndex <<  "\n and the device index is " << deviceIndex << std::endl;
    playSoundFile(m_filePath.toStdString().c_str(), api, deviceIndex);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filter = tr(
        "Audio (*.aiff *.au *.avr *.caf *.flac *.htk *.iff *.mat *.mpc *.oga *.paf *.pvf *.rf64 *.sf *.voc *.w64 *.wav *.wavex *.wve *xi) "
    );
    QFileDialog dialog(this); // who the daddy?

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDirectory(m_lastDirectory);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);

    // expand this
    dialog.setNameFilter(filter);
    if(dialog.exec())
        m_filePath = (dialog.selectedFiles()).at(0); // first selected file
        m_lastDirectory = QFileInfo(m_filePath).absoluteDir().absolutePath();
        writeSettings();

}

void MainWindow::on_actionSettings_triggered(){
    Settings settings(this);
    settings.setModal(true);
    settings.exec();
}


/*void MainWindow::keyPressEvent ( QKeyEvent * event )
{
qDebug("Came here,,,, Key Pressed");
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
qDebug("Came here,,,, Key Released");
} */

