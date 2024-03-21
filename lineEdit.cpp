#include "mainwindow.h"

void MainWindow::on_lineEdit_Fps_editingFinished() {
    if (ui->lineEdit_Fps->text().toDouble() > fps) {
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
    ui->lineEdit_Res_2->setText(QString::number(res[1] * ui->lineEdit_Res->text().QString::toInt() / res[0]));
    ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Res->text() + 'x' + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
    ui->comboBox_Res->setCurrentIndex(0);
}

void MainWindow::on_lineEdit_Res_2_editingFinished() {
    ui->lineEdit_Res->setText(QString::number(res[0] * ui->lineEdit_Res_2->text().QString::toInt() / res[1]));
    ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Res->text() + 'x' + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
    ui->comboBox_Res->setCurrentIndex(0);
}