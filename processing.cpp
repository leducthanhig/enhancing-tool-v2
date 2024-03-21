#include "mainwindow.h"

QString readStdOutput(QProcess* process) {
    return process->readAllStandardError() + process->readAllStandardOutput() + process->readAll();
}

void delay(int time) {
    QEventLoop loop;
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
}

bool ConfirmToStop() {
    QMessageBox msg(QMessageBox::Question, "Confirmation", "Do you want to stop current process?\n", QMessageBox::Yes | QMessageBox::No);
    msg.setStyleSheet("QPushButton{height: 25px}");
    if (msg.exec() == QMessageBox::Yes) return true;
    else return false;
}

bool containsNonAnsi(QString s) {
    for (QChar ch: s) {
        if (ch.unicode() > 255) { // ANSI characters have values from 0 to 255
            return true;
        }
    }
    return false;
}