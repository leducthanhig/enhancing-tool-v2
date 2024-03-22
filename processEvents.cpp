#include "mainwindow.h"

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event) {
    QFileInfo url(event->mimeData()->urls()[0].toLocalFile());

    if (url.isDir()) type = "dir";
    else {
        QMimeDatabase db;
        type = db.mimeTypeForFile(url).name();
        type.erase(type.begin() + type.indexOf('/'), type.end());
    }
    
    if (type == "image" || type == "video" || type == "dir") {
        fi = url;
        setup();
    }
    else {
        QMessageBox msg(QMessageBox::Warning, "Warning!!!", "The input file format is unsupported", QMessageBox::Ok);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    QMessageBox msg(QMessageBox::Question, "Confirmation", "Do you want to close?\n", QMessageBox::Yes | QMessageBox::No);
    msg.setStyleSheet("QPushButton{height: 25px}");
    if (msg.exec() == QMessageBox::Yes) {
        if (process != NULL) {
            process->kill();
            delete process;
        }
        event->accept();
    }
    else event->ignore();
}