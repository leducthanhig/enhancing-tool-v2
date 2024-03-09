#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFile f(":qdarkstyle/dark/darkstyle.qss");

    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    qApp->setStyleSheet(ts.readAll());

    MainWindow w;
    w.show();

    return a.exec();
}
