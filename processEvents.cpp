#include "mainwindow.h"

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event) {
    fi = QFileInfo(event->mimeData()->urls()[0].toLocalFile());
    setup();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    QMessageBox msg(QMessageBox::Question, "Confirmation", "Do you want to close?\n", QMessageBox::Yes | QMessageBox::No);
    msg.setStyleSheet("QPushButton{height: 25px}");
    if (msg.exec() == QMessageBox::Yes) {
        QProcess* close = new QProcess;
        close->start("taskkill /im \"EnhancingToolV2.exe\" /f /t");
        event->accept();
    }
    else event->ignore();
}