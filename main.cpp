#include "mainwindow.h"
#include <QApplication>
#include <QTableWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
     w.setWindowTitle("Ares");
    return a.exec();
}
