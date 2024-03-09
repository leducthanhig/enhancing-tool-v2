#include "mainwindow.h"

void MainWindow::on_lineEdit_Fps_editingFinished() {
    if (ui->lineEdit_Fps->text().toFloat() > fps.toFloat()) {
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Fps->text() + "FPS." + fo.suffix());
        ui->comboBox_Fps->setCurrentIndex(0);
    }
}

void MainWindow::on_lineEdit_Output_textChanged(const QString text) {
    fo = QFileInfo(text);
}

void MainWindow::on_lineEdit_OutFormat_textChanged(const QString suffix) {
    ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fo.completeBaseName() + '.' + suffix);
}

void MainWindow::on_lineEdit_Res_editingFinished() {
    if (type != "dir") {    //When "type == dir" does not support custom resolution
        ui->lineEdit_Res_2->setText(QString::number(res[1].QString::toInt() * ui->lineEdit_Res->text().QString::toInt() / res[0].QString::toInt(), 10));
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Res->text() + 'x' + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
        ui->comboBox_Res->setCurrentIndex(0);
    }
}

void MainWindow::on_lineEdit_Res_2_editingFinished() {
    if (type != "dir") {    //When "type == dir" does not support custom resolution
        ui->lineEdit_Res->setText(QString::number(res[0].QString::toInt() * ui->lineEdit_Res_2->text().QString::toInt() / res[1].QString::toInt(), 10));
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Res->text() + 'x' + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
        ui->comboBox_Res->setCurrentIndex(0);
    }
}