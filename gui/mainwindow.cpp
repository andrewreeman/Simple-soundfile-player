#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    //playSoundFile("E:/Software/Projects/simpleSF_Player/media/test.wav");
    playSoundFile(m_filePath.toStdString().c_str());
    //playSine();

}

void MainWindow::on_actionOpen_triggered()
{
    // update to filter audio files
    m_filePath = QFileDialog::getOpenFileName(this, tr("Open audio file"), QDir::currentPath(), "Any(*)");

}
