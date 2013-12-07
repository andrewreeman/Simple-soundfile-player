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
    //playSoundFile("E:/Software/Projects/simpleSF_Player/gui/Debug/release/media/test.wav");
    playSoundFile("media/test.wav");
    //playSine();

}
