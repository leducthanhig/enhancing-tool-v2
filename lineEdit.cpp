#include "mainwindow.h"

void MainWindow::on_lineEdit_Fps_textEdited(QString text) {
    if (text.toDouble() > fps) {
        if (!customOutputFileName) {
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + text + "FPS_" + ui->comboBox_Model->currentText() + "." + fo.suffix());
        }
        ui->comboBox_Fps->setCurrentText("--");
    }
    else {
        ui->comboBox_Fps->setCurrentText("x2");
    }
}

void MainWindow::on_lineEdit_Output_textChanged(QString text) {
    fo = QFileInfo(text);
}

void MainWindow::on_lineEdit_Output_textEdited() {
    customOutputFileName = 1;
}

void MainWindow::on_lineEdit_OutFormat_textEdited(QString suffix) {
    ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fo.completeBaseName() + "." + suffix);
}

void MainWindow::on_lineEdit_Res_textEdited(QString text) {
    if (text.toInt() > 0) {
        ui->comboBox_Res->setCurrentText("--");
        ui->lineEdit_Res_2->setText(QString::number(res[1] * text.toInt() / res[0]));
        if (!customOutputFileName) {
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + text + "x" + ui->lineEdit_Res_2->text() + "_" + ui->comboBox_Model->currentText() + "." + fo.suffix());
        }
    }
    else {
        ui->comboBox_Res->setCurrentText("x2");
    }
}

void MainWindow::on_lineEdit_Res_2_textEdited(QString text) {
    if (text.toInt() > 0) {
        ui->comboBox_Res->setCurrentText("--");
        ui->lineEdit_Res->setText(QString::number(res[0] * text.toInt() / res[1]));
        if (!customOutputFileName) {
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Res->text() + "x" + text + "_" + ui->comboBox_Model->currentText() + "." + fo.suffix());
        }
    }
    else {
        ui->comboBox_Res->setCurrentText("x2");
    }
}