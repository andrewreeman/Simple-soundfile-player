#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Simple Soundfile Player");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
