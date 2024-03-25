#include "mainwindow.h"

void MainWindow::on_pushButton_Info_released() {
    QMessageBox msg(QMessageBox::NoIcon, "Enhancing Model Informations", "# Anime: (mostly x2)\n"
                                                        "AnimeJaNai-HD-V3-Super\tSpeed: ⭐⭐⭐\tQuality: ⭐⭐\n"
                                                        "AniScale\t\t\tSpeed: ⭐\t\tQuality: ⭐⭐\n"
                                                        "AniScale2S-Compact\t\tSpeed: ⭐\t\tQuality: ⭐⭐⭐\n"
                                                        "LD-Anime-Compact\t\tSpeed: ⭐\t\tQuality: ⭐⭐\n"
                                                        "realesr-animevideov3 (x2, 3, 4)\tSpeed: ⭐\t\tQuality: ⭐\n"
                                                        "\n# Animated Art: (mostly x4)\n"
                                                        "Fatal-Anime\t\t\tSpeed: ⭐\t\tQuality: ⭐⭐\n"
                                                        "KcjpunkAnime-2-Lite (x2)\tSpeed: ⭐⭐⭐\tQuality: ⭐⭐\n"
                                                        "NMKD-UltraYandere\t\tSpeed: ⭐\t\tQuality: ⭐⭐⭐\n"
                                                        "NMKD-Yandere2\t\tSpeed: ⭐\t\tQuality: ⭐⭐\n"
                                                        "NMKD-YandereNeo (x2)\t\tSpeed: ⭐⭐⭐\tQuality: ⭐⭐\n"
                                                        "NMKD-YandereNeoXL\t\tSpeed: ⭐\t\tQuality: ⭐⭐⭐\n"
                                                        "realesrgan-x4plus-anime\t\tSpeed: ⭐⭐⭐\tQuality: ⭐⭐\n"
                                                        "\n# AI Generated, Realistic, Text: (x4)\n"
                                                        "DF2K_JPEG\t\t\tSpeed: ⭐\t\tQuality: ⭐⭐⭐\n"
                                                        "realesrgan-x4plus\t\tSpeed: ⭐\t\tQuality: ⭐⭐\n"
                                                        "realesr-general-wdn-x4v3\tSpeed: ⭐⭐⭐\tQuality: ⭐⭐\n"
                                                        "realesr-general-x4v3\t\tSpeed: ⭐⭐⭐\tQuality: ⭐⭐\n"
                                                        "realesrnet-x4plus (Only Text)\tSpeed: ⭐\t\tQuality: ⭐⭐⭐\t\n"
                                                        "\n# RealCUGAN-NCNN-Vulkan (x2, 3, 4)\n",
                    QMessageBox::Close);
    msg.setStyleSheet("QPushButton{height: 25px}");
    msg.exec();
}

void MainWindow::on_pushButton_GpuID_released() {
    ui->pushButton_GpuID->setEnabled(0);

    process = new QProcess;
    QString cmd = "\"" + currentPath + "/realesrgan-ncnn-vulkan/realesrgan-ncnn-vulkan.exe\" -i check.jpg -o checked.jpg";
    process->start(cmd);
    process->waitForFinished();
    QString output = readStdOutput(process);
    delete process;

    if (output.contains("invalid gpu device")) {
        QMessageBox msg(QMessageBox::Critical, "Error!!!", "Your GPU does not support Vulkan or the graphic driver is out-of-date!\n", QMessageBox::Close);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
    }
    else {
        QStringList list = getGPUID(output);
        QMessageBox msg(QMessageBox::NoIcon, "GPU ID", '\n' + list.join("\t\n\n") + "\n", QMessageBox::Close);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
        ui->comboBox_GpuID->clear();
        for (int i = 0; i < list.length(); i++) {
            ui->comboBox_GpuID->addItem(list[i][1]);
        }
        ui->comboBox_GpuID->setToolTip(list.join("\n"));
    }
    ui->comboBox_GpuID->setEnabled(1);
    ui->comboBox_GpuID->setCurrentIndex(1);
    ui->pushButton_GpuID->setEnabled(1);
}

void MainWindow::on_pushButton_TileSize_Inc_released() {
    int TileSize = ui->lineEdit_TileSize->text().QString::toInt() * 2;
    if (TileSize == 0) TileSize = 32;
    else if (TileSize == 8192) ui->pushButton_TileSize_Inc->setEnabled(0);
    ui->pushButton_TileSize_Dec->setEnabled(1);
    ui->lineEdit_TileSize->setText(QString::number(TileSize));
}

void MainWindow::on_pushButton_TileSize_Dec_released() {
    int TileSize = ui->lineEdit_TileSize->text().QString::toInt() / 2;
    if (TileSize == 16) {
        TileSize = 0;
        ui->pushButton_TileSize_Dec->setEnabled(0);
    }
    ui->pushButton_TileSize_Inc->setEnabled(1);
    ui->lineEdit_TileSize->setText(QString::number(TileSize));
}

void MainWindow::on_pushButton_Input_released() {
    QString url = QFileDialog::getOpenFileName(this, "Open File", QString(), "All Files (*.*) ;; Image Files (*.png *.jpg *.jpeg) ;; Video Files (*.mp4 *.mkv *.ts)");
    if (url != "") {
        QFileInfo file(url);
        
        QMimeDatabase db;
        type = db.mimeTypeForFile(file).name();
        type.erase(type.begin() + type.indexOf('/'), type.end());

        if (type == "image" || type == "video") {
            fi = file;
            setup();
        }
        else {
            QMessageBox msg(QMessageBox::Warning, "Warning!!!", "The input file format is unsupported", QMessageBox::Ok);
            msg.setStyleSheet("QPushButton{height: 25px}");
            msg.exec();
        }
    }
}

void MainWindow::on_pushButton_Output_released() {
    QString url = QFileDialog::getExistingDirectory(this, "Open File", QString());
    if (url != "") {
        ui->lineEdit_Output->setText(url + "/" + QFileInfo(ui->lineEdit_Output->text()).fileName());
    }
}

void MainWindow::on_pushButton_Start_released() {
    DecodingTime = InterpolatingTime = UpscalingTime = EncodingTime = QTime(0, 0);
    startTime = QTime::currentTime();
    state = "Started";

    ui->pushButton_Start->setVisible(0);
    ui->groupBox_IO->setEnabled(0);
    ui->label_Tool->setEnabled(0);
    ui->comboBox_Tool->setEnabled(0);
    ui->label_Presets->setEnabled(0);
    ui->comboBox_Presets->setEnabled(0);
    ui->pushButton_Info->setEnabled(0);
    ui->groupBox_Setting->setEnabled(0);
    setVisible_ViewState(1);
    setAcceptDrops(0);

    switch (ui->comboBox_Tool->currentIndex()) {
    case 0:
        if (type == "dir") {
            ui->progressBar->setFormat("%p% - Upscaling...");
            numFrame = QDir(fi.absoluteFilePath()).count() - 2;
            Upscaling(fi, numFrame);
            if (state == "Stopped") goto end;

            if (resizingNeeded) {
                ui->progressBar->setFormat("%p% - Resizing...");
                ui->progressBar->setValue(0);

                QDir outDir(fo.filePath());
                if (outDir.exists()) outDir.removeRecursively();
                QDir(fo.absolutePath()).mkdir(fo.filePath());

                QDir inDir(fi.filePath() + "_upscaled");
                QStringList list = inDir.entryList(QDir::Files);

                QTime startTime = QTime::currentTime();
                for (int i = 0; i < list.length(); i++) {
                    Resizing(QFileInfo(inDir.path() + "/" + list[i]));

                    delay(300);
                    setProgressBarVal((i + 1) * 100 / numFrame);
                    setRemaining(startTime);

                    if (ui->pushButton_Stop->isDown()) {
                        if (ConfirmToStop()) {
                            state = "Stopped";
                            goto end;
                        }
                    }
                }
                while (QDir(outDir.path()).count() - 2 < numFrame) {
                    setTimeTaken();
                    delay(100);
                }

                setProgressBarVal(100);

                inDir.removeRecursively();
            }
        }
        else if (type == "image" && fi.suffix() != "gif") {
            ui->progressBar->setFormat("%p% - Upscaling...");
            Upscaling(fi);
            if (state == "Stopped") goto end;

            if (resizingNeeded) {
                ui->progressBar->setFormat("%p% - Resizing...");
                ui->progressBar->setValue(0);

                Resizing(QFileInfo(fi.absolutePath() + '/' + fi.completeBaseName() + "_upscaled." + fo.suffix()));

                setProgressBarVal(100);

                QFile(fi.absolutePath() + '/' + fi.completeBaseName() + "_upscaled." + fo.suffix()).remove();
            }
        }
        else { // type == "video" && fi.suffix() == "gif"
            numPart = Spliting();

            if (numPart == 0) {
                ui->progressBar->setFormat("%p% - Decoding...");
                numFrame = Decoding(fi, qRound(dur * fps));
                if (numFrame == 0) state = "Stopped";
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - Upscaling...");
                Upscaling(fi, numFrame);
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - Encoding...");
                Encoding(fi, fps);
                if (state == "Stopped") goto end;
            }
            else {
                double dur;
                for (int i = 0; i < numPart; i++) {
                    QFileInfo file = QFileInfo(fi.absolutePath() + '/' + fi.completeBaseName() + "_splited/" + QString::number(i) + '.' + fi.suffix());
                    dur = getDuration(file);

                    ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Decoding...");
                    numFrame = Decoding(file, qRound(dur * fps));
                    if (state == "Stopped") goto end;

                    ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Upscaling...");
                    Upscaling(file, numFrame);
                    if (state == "Stopped") goto end;

                    ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Encoding...");
                    Encoding(file, fps);
                    if (state == "Stopped") goto end;
                }
                ui->progressBar->setFormat("%p% - Joining splited parts...");
                Joining(QDir(fi.absolutePath() + '/' + fi.completeBaseName() + "_splited"));
            }
        }
        break;
    case 1:
        numPart = Spliting();

        if (numPart == 0) {
            ui->progressBar->setFormat("%p% - Decoding...");
            numFrame = Decoding(fi, qRound(dur * fps));
            if (state == "Stopped") goto end;

            ui->progressBar->setFormat("%p% - Interpolating...");
            int targetNumFrame = getTargetNumFrame(numFrame);
            Interpolating(fi, targetNumFrame);
            if (state == "Stopped") goto end;

            ui->progressBar->setFormat("%p% - Encoding...");
            double newFps = getNewFps();
            Encoding(fi, newFps);
            if (state == "Stopped") goto end;
        }
        else {
            double dur;
            for (int i = 0; i < numPart; i++) {
                QFileInfo file = QFileInfo(fi.absolutePath() + '/' + fi.completeBaseName() + "_splited/" + QString::number(i) + '.' + fi.suffix());
                dur = getDuration(file);

                ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Decoding...");
                numFrame = Decoding(file, qRound(dur * fps));
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Interpolating...");
                int targetNumFrame = getTargetNumFrame(numFrame);
                Interpolating(file, targetNumFrame);
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Encoding...");
                double newFps = getNewFps();
                Encoding(file, newFps);
                if (state == "Stopped") goto end;
            }
            ui->progressBar->setFormat("%p% - Joining splited parts...");
            Joining(QDir(fi.absolutePath() + '/' + fi.completeBaseName() + "_splited"));
        }
    }
    state = "Finished";
end:;
    QString notification = state + "\n\n" + ui->label_TimeTaken->text() + "\n\n";
    if (-DecodingTime.secsTo(QTime(0, 0)) > 0) 
        notification += "Decoding Time: " + DecodingTime.toString("hh:mm:ss") + '\n';
    if (-InterpolatingTime.secsTo(QTime(0, 0)) > 0) 
        notification += "Interpolating Time: " + InterpolatingTime.toString("hh:mm:ss") + '\n';
    if (-UpscalingTime.secsTo(QTime(0, 0)) > 0) 
        notification += "Upscaling Time: " + UpscalingTime.toString("hh:mm:ss") + '\n';
    if (-EncodingTime.secsTo(QTime(0, 0)) > 0) 
        notification += "Encoding Time: " + EncodingTime.toString("hh:mm:ss") + '\n';
    
    QMessageBox msg(QMessageBox::Information, "Notification", notification, QMessageBox::Close);
    msg.setStyleSheet("QPushButton{height: 25px}");
    msg.exec();
    
    setAcceptDrops(1);
    setVisible_ViewState(0);
    ui->pushButton_Start->setVisible(1);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("%p% - Preparing...");
    ui->groupBox_IO->setEnabled(1);
    if (type == "video") ui->comboBox_Tool->setEnabled(1);
    ui->label_Presets->setEnabled(1);
    ui->comboBox_Presets->setEnabled(1);
    ui->pushButton_Info->setEnabled(1);
    ui->groupBox_Setting->setEnabled(1);
}