#include "mainwindow.h"

void setTheme() {
    QFile f(":qdarkstyle/dark/darkstyle.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    qApp->setStyleSheet(ts.readAll());
}

void MainWindow::setEnabled_Res(int para) {
    ui->label_Res->setEnabled(para);
    ui->label_Res_2->setEnabled(para);
    ui->comboBox_Res->setEnabled(para);
    ui->lineEdit_Res->setEnabled(para);
    ui->lineEdit_Res_2->setEnabled(para);
}

void MainWindow::setEnabled_OutFormat(int para) {
    ui->label_OutFormat->setEnabled(para);
    ui->lineEdit_OutFormat->setEnabled(para);
}

void MainWindow::setEnabled_Segment(int para) {
    ui->label_Segment->setEnabled(para);
    ui->label_Segment_2->setEnabled(para);
    ui->lineEdit_Segment->setEnabled(para);
}

void MainWindow::setEnabled_TileSize(int para) {
    ui->label_TileSize->setEnabled(para);
    ui->lineEdit_TileSize->setEnabled(para);
    ui->pushButton_TileSize_Dec->setEnabled(para);
    ui->pushButton_TileSize_Inc->setEnabled(para);
}

void MainWindow::setEnabled_Thread(int para) {
    ui->lineEdit_Thread->setEnabled(para);
    ui->label_Thread->setEnabled(para);
}

void MainWindow::setEnabled_VCodec(int para) {
    ui->label_VCodec->setEnabled(para);
    ui->comboBox_VCodec->setEnabled(para);
}

void MainWindow::setEnabled_CRF(int para) {
    ui->label_CRF->setEnabled(para);
    ui->spinBox_CRF->setEnabled(para);
}

void MainWindow::setEnabled_Fps(int para) {
    ui->label_Fps->setEnabled(para);
    ui->lineEdit_Fps->setEnabled(para);
    ui->comboBox_Fps->setEnabled(para);
}

void MainWindow::setEnabled_Denoise(int para) {
    ui->label_Denoise->setEnabled(para);
    ui->comboBox_Denoise->setEnabled(para);
}

void MainWindow::setEnabled_SyncGap(int para) {
    ui->label_SyncGap->setEnabled(para);
    ui->comboBox_SyncGap->setEnabled(para);
}

void MainWindow::setVisible_ViewState(int para) {
    ui->groupBox_Timer->setVisible(para);
    ui->progressBar->setVisible(para);
    ui->pushButton_Stop->setVisible(para);
}

void MainWindow::setTimeTaken() {
    ui->label_TimeTaken->setText("Time taken: " + secToString(int(toSec(QTime::currentTime().toString("hh:mm:ss").split(':')) - toSec(startTime.toString("hh:mm:ss").split(':')))));

}

void MainWindow::setRemaining(QTime startTime) {
    if (ui->progressBar->value() != 0) {
        float remaining = (toSec(QTime::currentTime().toString("hh:mm:ss.zzz").split(':')) - toSec(startTime.toString("hh:mm:ss.zzz").split(':'))) * (float(100) / ui->progressBar->value() - 1);
        if (ui->label_Remaining->text() == "Remaining: unknown" || remaining < toSec(ui->label_Remaining->text().remove("Remaining: ").split(':')))
            ui->label_Remaining->setText("Remaining: " + secToString(qRound(remaining)));
    }
    else ui->label_Remaining->setText("Remaining: unknown");
}

void MainWindow::setProgressBarVal(int newVal) {
    for (int i = ui->progressBar->value(); i <= newVal; i++) {
        ui->progressBar->setValue(i);
        delay(3);
    }
}

void MainWindow::readMetadata() {
    QProcess* process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -i \"" + fi.filePath() + "\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0) delay(100);
    QString output = readStdOutput(process);

    if (output.contains("Metadata")) type = "video";
    else type = "image";

    res = getResolutions(output);
    ui->lineEdit_Res->setText(res[0]);
    ui->lineEdit_Res_2->setText(res[1]);
    if (ui->comboBox_Tool->currentIndex() == 0) setEnabled_Res(1);

    if (type == "video") {
        output = output.remove(0, output.indexOf("Duration"));
        output = output.remove(output.indexOf("fps") - 1, output.length());

        fps = "";
        int i = output.length() - 1;
        while (output[i] != ' ') {
            fps = output[i] + fps;
            i--;
        }
        ui->lineEdit_Fps->setText(fps);
        if (ui->comboBox_Tool->currentIndex() == 1)
            setEnabled_Fps(1);

        output = output.remove("Duration: ");
        output = output.remove(output.indexOf(','), output.length());
        dur = toSec(output.split(':'));
    }
}

void MainWindow::readModelName() {
    QDir modelsDir = QDir(currentPath + "/realesrgan-ncnn-vulkan/models");
    foreach(QString model, modelsDir.entryList()) {
        if (model.contains(".bin") && model[1] == 'x') {
            ui->comboBox_Model->addItem(model.remove(0, 3).remove(".bin"));
        }
    }
}

void MainWindow::setup() {
    ui->lineEdit_Res->clear();
    ui->lineEdit_Res_2->clear();
    ui->lineEdit_OutFormat->clear();
    ui->lineEdit_Fps->clear();

    ui->lineEdit_Input->setText(fi.filePath());
    ui->label_Output->setEnabled(1);
    ui->lineEdit_Output->setEnabled(1);
    ui->pushButton_Output->setEnabled(1);
    ui->pushButton_Start->setEnabled(1);
    
    if (fi.isDir()) {
        type = "dir";
        ui->lineEdit_Output->setText(fi.filePath() + "_x_");
        ui->comboBox_Tool->setCurrentIndex(0);
        ui->comboBox_Tool->setEnabled(0);
        ui->label_Res->setEnabled(1);
        ui->comboBox_Res->setEnabled(1);
        setEnabled_Res(0);
        setEnabled_OutFormat(0);
        setEnabled_Segment(0);
        setEnabled_TileSize(1);
        setEnabled_Thread(1);
        setEnabled_VCodec(0);
        setEnabled_CRF(0);
        setEnabled_Fps(0);
    }
    else {
        readMetadata();
        if (type == "image") {
            ui->lineEdit_Output->setText(fi.absolutePath() + "/" + fi.completeBaseName() + "_." + fi.suffix());
            ui->lineEdit_OutFormat->setText(fi.suffix());
            ui->comboBox_Tool->setCurrentIndex(0);
            ui->comboBox_Tool->setEnabled(0);
            ui->lineEdit_Segment->clear();
            ui->lineEdit_Thread->clear();
            setEnabled_Res(1);
            setEnabled_OutFormat(1);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(0);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }
        else { // type == "video"
            ui->lineEdit_Output->setText(fi.absolutePath() + "/" + fi.completeBaseName() + "_." + fi.suffix());
            ui->lineEdit_OutFormat->setText(fi.suffix());
            ui->comboBox_Tool->setEnabled(1);
            ui->lineEdit_Segment->setText("30");
            ui->comboBox_VCodec->setCurrentIndex(0);
            ui->spinBox_CRF->setValue(23);

            if (ui->comboBox_Tool->currentIndex() == 0) {
                setEnabled_Res(1);
                setEnabled_TileSize(1);
                setEnabled_Fps(0);
            }
            else {
                setEnabled_Res(0);
                setEnabled_TileSize(0);
                setEnabled_Fps(1);
            }
            setEnabled_OutFormat(1);
            setEnabled_Segment(1);
            setEnabled_Thread(1);
            setEnabled_VCodec(1);
            setEnabled_CRF(1);
        }
    }
    // Reset the option -> Must change the index then return
    // (0 + 1) % 5 = 1 -> (1 + 5 - 1) % 5 = 0
    // (1 + 1) % 5 = 2 -> (2 + 5 - 1) % 5 = 1
    // (2 + 1) % 5 = 3 -> (3 + 5 - 1) % 5 = 2
    // (3 + 1) % 5 = 4 -> (4 + 5 - 1) % 5 = 3
    // (4 + 1) % 5 = 0 -> (0 + 5 - 1) % 5 = 4
    // => set (index + 1) % 5 then set (new index + 4) % 5
    ui->comboBox_Presets->setCurrentIndex((ui->comboBox_Presets->currentIndex() + 1) % 5);
    ui->comboBox_Presets->setCurrentIndex((ui->comboBox_Presets->currentIndex() + 4) % 5);

    if (containsNonAnsi(fi.absoluteFilePath())) {
        QMessageBox msg(QMessageBox::Critical, "Warning!!!", "The input path contains Unicode characters that will raise an error when saving the result. Please rename and try again!\n", QMessageBox::Ok);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
        ui->pushButton_Start->setEnabled(0);
    }
}