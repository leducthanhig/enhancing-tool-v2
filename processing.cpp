#include "mainwindow.h"

QString readStdOutput(QProcess* process) {
    return process->readAllStandardError() + process->readAllStandardOutput() + process->readAll();
}

void delay(int time) {
    QEventLoop loop;
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
}

float toSec(QStringList time) {
    return time[0].QString::toFloat() * 3600 + time[1].QString::toFloat() * 60 + time[2].QString::toFloat();
}

QString secToString(int sec) {
    return QTime(sec / 3600, (sec % 3600) / 60, sec % 60).toString("hh:mm:ss");
}

bool ConfirmToStop() {
    QMessageBox msg(QMessageBox::Warning, "Confirmation", "Do you want to stop current process?\n", QMessageBox::Yes | QMessageBox::No);
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