#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication magically knows about MainWindow...amazing!!!
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
