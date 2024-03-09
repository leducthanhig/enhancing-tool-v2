#include "mainwindow.h"

void MainWindow::rightClickMenu_TimeTaken(QPoint pos) {
    QMenu Menu(this);
    QAction showRemaining("Show \"Remaining\"", this);
    connect(&showRemaining, SIGNAL(triggered()), this, SLOT(Remaining()));
    Menu.addAction(&showRemaining);
    Menu.exec(ui->label_TimeTaken->mapToGlobal(pos));
}

void MainWindow::rightClickMenu_Remaining(QPoint pos) {
    QMenu Menu(this);
    QAction showTimeTaken("Show \"Time taken\"", this);
    connect(&showTimeTaken, SIGNAL(triggered()), this, SLOT(TimeTaken()));
    Menu.addAction(&showTimeTaken);
    Menu.exec(ui->label_Remaining->mapToGlobal(pos));
}