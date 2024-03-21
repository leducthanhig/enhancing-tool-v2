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
    ui->label_TimeTaken->setText("Time taken: " + QTime(0, 0).addSecs(-QTime::currentTime().secsTo(startTime)).toString("hh:mm:ss"));
}

void MainWindow::setRemaining(QTime startTime) {
    if (type == "video" && numPart > 1) {
        QString str = ui->progressBar->text();
        double finished = str.sliced(str.indexOf("-") + 2, str.indexOf("of") - str.indexOf("-") - 2).toInt() - 1;
        QString stage = str.sliced(str.indexOf(':') + 2, str.indexOf("...") - str.indexOf(":") - 2);
        double weight;  // Estimate: Upscaling = 6 Encoding, Encoding = 5 Decoding => 30 + 5 + 1 = 36;
        if (stage == "Decoding") weight = 1.0 / 36;
        else if (stage == "Upscaling") weight = 31.0 / 36;
        else if (stage == "Encoding") weight = 1;
        finished += weight * ui->progressBar->value() / 100.0;
        
        if (finished > 0) {
            QString remaining = QTime(0, 0).addSecs(qRound(-QTime::currentTime().secsTo(MainWindow::startTime) * (numPart / finished - 1))).toString("hh:mm:ss");
            if (ui->label_Remaining->text() == "Remaining: unknown" || remaining < ui->label_Remaining->text().remove("Remaining: ")) {
                ui->label_Remaining->setText("Remaining: " + remaining);
            }
        }
        else {
            ui->label_Remaining->setText("Remaining: unknown");
        }
    }
    else {
        if (ui->progressBar->value() != 0) {
            QString remaining = QTime(0, 0).addSecs(qRound(-QTime::currentTime().secsTo(startTime) * (100.0 / ui->progressBar->value() - 1))).toString("hh:mm:ss");
            if (ui->label_Remaining->text() == "Remaining: unknown" || remaining < ui->label_Remaining->text().remove("Remaining: ")) {
                ui->label_Remaining->setText("Remaining: " + remaining);
            }
        }
        else {
            ui->label_Remaining->setText("Remaining: unknown");
        }
    }
}

void MainWindow::setProgressBarVal(int newVal) {
    for (int i = ui->progressBar->value(); i <= newVal; i++) {
        ui->progressBar->setValue(i);
        delay(3);
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
    
    if (type == "dir") {
        ui->lineEdit_Output->setText(fi.filePath() + "_x_");
        ui->comboBox_Tool->setCurrentIndex(0);
        ui->comboBox_Tool->setEnabled(0);
        setEnabled_Res(0);
        ui->label_Res->setEnabled(1);
        ui->comboBox_Res->setEnabled(1);
        setEnabled_OutFormat(0);
        setEnabled_Segment(0);
        setEnabled_TileSize(1);
        setEnabled_Thread(1);
        setEnabled_VCodec(0);
        setEnabled_CRF(0);
        setEnabled_Fps(0);
    }
    else {
        getMetadata();
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
        QMessageBox msg(QMessageBox::Warning, "Warning!!!", "The input path contains Unicode characters that will raise an error when saving the result. Please rename and try again!\n", QMessageBox::Ok);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
        ui->pushButton_Start->setEnabled(0);
    }
}