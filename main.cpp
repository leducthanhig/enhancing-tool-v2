#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("QPushButton{background-color: rgb(66, 66, 66); border-radius: 7px}"
                    "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                    "QPushButton:pressed{background-color: rgb(60, 60, 60)}");
    w.show();
    return a.exec();
}
