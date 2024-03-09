#include "mainwindow.h"

void MainWindow::dealWithInterpolatingErr() {
    delay(1000);
    if (ui->lineEdit_Thread->text() != "") {
        QStringList list = ui->lineEdit_Thread->text().split(':');
        int load = list[0].toInt(),
            proc = list[1].toInt(),
            save = list[2].toInt();
        if (load == 1 && proc == 1 && save == 1) state = "Stopped";
        if (load > 1) load--;
        if (proc > 1) proc--;
        if (save > 1) save--;
        ui->lineEdit_Thread->setText(QString::number(load, 10) + ':' + QString::number(proc, 10) + ':' + QString::number(save, 10));
    }
    else {
        state = "Stopped";
    }
}

void MainWindow::dealWithUpscalingErr() {
    delay(1000);
    int TileSize = ui->lineEdit_TileSize->text().toInt();
    /*
        if tile > 1024 then down tile; 
        else if thread > 2 then down thread; 
        else if tile > 256 then down tile; 
        else if thread > 1 then down thread; 
        else if tile > 0 then set tile 0; 
        else stop
    */
    if (TileSize > 1024) {
        TileSize /= 2;
        ui->lineEdit_TileSize->setText(QString::number(TileSize));
    }
    else if (ui->lineEdit_Thread->text() != "") {
        QStringList list = ui->lineEdit_Thread->text().split(':');
        int load = list[0].toInt(),
            proc = list[1].toInt(),
            save = list[2].toInt();
        if (load > 2) load--;
        if (proc > 2) proc--;
        if (save > 2) save--;
        ui->lineEdit_Thread->setText(QString::number(load, 10) + ':' + QString::number(proc, 10) + ':' + QString::number(save, 10));
    }
    else if (TileSize > 256) {
        TileSize /= 2;
        ui->lineEdit_TileSize->setText(QString::number(TileSize));
    }
    else if (ui->lineEdit_Thread->text() != "") {
        QStringList list = ui->lineEdit_Thread->text().split(':');
        int load = list[0].toInt(),
            proc = list[1].toInt(),
            save = list[2].toInt();
        if (load > 1) load--;
        if (proc > 1) proc--;
        if (save > 1) save--;
        ui->lineEdit_Thread->setText(QString::number(load, 10) + ':' + QString::number(proc, 10) + ':' + QString::number(save, 10));
    }
    else if (TileSize > 0) {
        TileSize /= 2;
        ui->lineEdit_TileSize->setText(QString::number(TileSize));
    }
    else {
        state = "Stopped";
    }
}