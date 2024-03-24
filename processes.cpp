#include "mainwindow.h"

int MainWindow::Spliting() {
    setTimeTaken();

    if (ui->lineEdit_Segment->text() != "" && ui->lineEdit_Segment->text().QString::toDouble() < dur) {
        QDir splitedDir(fi.absolutePath() + '/' + fi.completeBaseName() + "_splited");
        if (splitedDir.exists()) splitedDir.removeRecursively();
        QDir(fi.absolutePath()).mkdir(fi.completeBaseName() + "_splited");

        process = new QProcess;
        QString cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -i \"" + fi.filePath() + "\" -c copy -map 0 -f segment -segment_time " + ui->lineEdit_Segment->text() + " -reset_timestamps 1 \"" + splitedDir.absolutePath() + "/%d." + fi.suffix() + "\" -hide_banner";
        process->start(cmd);
        while (process->state() != 0) {
            setTimeTaken();
            delay(100);
        }
        delete process;
        
        return splitedDir.count() - 2;
    }
    else return 0;
}

int MainWindow::Decoding(QFileInfo file, int numFrame) {
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QDir frameDir(file.absolutePath() + '/' + file.completeBaseName() + "_frames");
    if (frameDir.exists()) frameDir.removeRecursively();
    QDir(file.absolutePath()).mkdir(file.completeBaseName() + "_frames");

    ui->progressBar->setValue(0);
    process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -i \"" + file.filePath() + "\" \"" + frameDir.absolutePath() + "/%8d.png\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0) {
        setProgressBarVal(int(getNumFrameFinished(process) * 100 / numFrame));
        setTimeTaken();
        setRemaining(startTime);
        delay(100);

        if (ui->pushButton_Stop->isDown()) {
            if (ConfirmToStop()) {
                process->kill();
                state = "Stopped";
            }
        }
    }
    delete process;
    
    DecodingTime = DecodingTime.addSecs(-QTime::currentTime().secsTo(startTime));

    if (state != "Stopped") setProgressBarVal(100);

    return frameDir.count() - 2;
}

void MainWindow::Interpolating(QFileInfo file, int numFrame) {
retryInterpolating:;
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QDir frameDir(file.absolutePath() + '/' + file.completeBaseName() + "_interpolated");
    if (frameDir.exists()) frameDir.removeRecursively();
    QDir(file.absolutePath()).mkdir(file.completeBaseName() + "_interpolated");

    QString targetNumFrame;     // Only rife-v4 support target num frame
    if ((ui->comboBox_Engine->currentIndex() == 0 && ui->comboBox_Model->currentIndex() < 9) || (ui->comboBox_Engine->currentIndex() == 1 && ui->comboBox_Model->currentIndex() > 2)) targetNumFrame = "";
    else targetNumFrame = " -n " + QString::number(numFrame);

    QString UHD = "";
    if (res[1] >= 2160) UHD = " -u";

    ui->progressBar->setValue(0);
    process = new QProcess;
    QString cmd = "\"" + currentPath + "/" + ui->comboBox_Engine->currentText().toLower() + "/" + ui->comboBox_Engine->currentText().toLower() + ".exe\" -i \"" + file.absolutePath() + '/' + file.completeBaseName() + "_frames" + "\" -o \"" + frameDir.absolutePath() + "\"" + targetNumFrame + " -m " + ui->comboBox_Model->currentText() + getParameter("gpuid") + getParameter("thread") + UHD;
    process->start(cmd);
    while (process->state() != 0) {
        setProgressBarVal((QDir(frameDir.absolutePath()).count() - 2) * 100 / numFrame);
        setTimeTaken();
        setRemaining(startTime);
        delay(100);

        if (ui->pushButton_Stop->isDown()) {
            if (ConfirmToStop()) {
                process->kill();
                state = "Stopped";
            }
        }

        if (readStdOutput(process).contains("failed")) {
            process->kill();
            dealWithInterpolatingErr();
            if (state != "Stopped") goto retryInterpolating;
        }
    }
    delete process;
    
    InterpolatingTime = InterpolatingTime.addSecs(-QTime::currentTime().secsTo(startTime));

    if (state != "Stopped") {
        setProgressBarVal(100);
        QDir(file.absolutePath() + '/' + file.completeBaseName() + "_frames").removeRecursively();
    }
}

void MainWindow::Upscaling(QFileInfo input, int numFrame) {
retryUpscaling:;
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QString inputPath;
    if (type == "video" || fi.suffix() == "gif") inputPath = input.absolutePath() + '/' + input.completeBaseName() + "_frames";
    else inputPath = input.filePath();

    QString outputPath, model = getParameter("model"); // Declare 'model' variable here to make 'resizingNeeded' variable assigned value
    if (type == "video" || fi.suffix() == "gif") outputPath = input.absolutePath() + '/' + input.completeBaseName() + "_upscaled";
    else if (resizingNeeded) {
        if (type == "image") outputPath = input.absolutePath() + '/' + input.completeBaseName() + "_upscaled." + fo.suffix();
        else outputPath = input.filePath() + "_upscaled";
    }
    else outputPath = ui->lineEdit_Output->text();

    if (type != "image" || fi.suffix() == "gif") {
        QDir dir(outputPath);
        if (dir.exists()) dir.removeRecursively();
        QDir(dir.absolutePath()).mkdir(outputPath);
    }

    ui->progressBar->setValue(0);
    process = new QProcess;
    QString cmd = "\"" + currentPath + "/" + ui->comboBox_Engine->currentText().toLower() + "/" + ui->comboBox_Engine->currentText().toLower() + ".exe\" -i \"" + inputPath + "\" -o \"" + outputPath + "\"" + model + " -t " + ui->lineEdit_TileSize->text() + getParameter("noise_syncgap") + getParameter("gpuid") + getParameter("thread");
    process->start(cmd);
    while (process->state() != 0) {
        setTimeTaken();
        setRemaining(startTime);
        delay(100);

        if (ui->pushButton_Stop->isDown()) {
            if (ConfirmToStop()) {
                process->kill();
                state = "Stopped";
            }
        }
        QString output = readStdOutput(process);
        if (type != "image" || fi.suffix() == "gif") setProgressBarVal((QDir(outputPath).count() - 2) * 100 / numFrame);
        else if (output.contains("%\r\n")) setProgressBarVal(qRound(output.remove("%\r\n").toDouble()));

        if (output.contains("alpha") && fo.suffix() == "jpg") {
            process->kill();
            state = "Stopped";
            QMessageBox msg(QMessageBox::Critical, "Conversion failed!", "Detected alpha channel!", QMessageBox::Close);
            msg.setStyleSheet("QPushButton{height: 25px}");
            msg.exec();
        }
        if (output.contains("failed")) {
            process->kill();
            dealWithUpscalingErr();
            if (state != "Stopped") goto retryUpscaling;
        }
    }
    delete process;

    UpscalingTime = UpscalingTime.addSecs(-QTime::currentTime().secsTo(startTime));

    if (state != "Stopped") {
        setProgressBarVal(100);
        if (type == "video" || fi.suffix() == "gif") QDir(inputPath).removeRecursively();
    }
}

void MainWindow::Encoding(QFileInfo file, double fps) {
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QString suffix;
    if (ui->comboBox_Tool->currentIndex() == 0) suffix = "_upscaled";
    else suffix = "_interpolated";

    QDir frameDir(file.absolutePath() + '/' + file.completeBaseName() + suffix);
    QString outputPath;
    if (numPart == 0) outputPath = ui->lineEdit_Output->text();
    else outputPath = frameDir.absolutePath() + '.' + fo.suffix();
    
    ui->progressBar->setValue(0);
    process = new QProcess;
    QString cmd;
    if (type == "video") {
        cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -r " + QString::number(fps, 'f', 2) + " -i \"" + frameDir.absolutePath() + "/%8d.png\" -i \"" + file.filePath() + "\" -map 0:v -map 1:a:? -map 1:s:? -map 1:t:? -c:a copy -c:s copy -c:t copy -disposition:s default -crf " + QString::number(ui->spinBox_CRF->value()) + getVCodec() + getScaleFilter() + " -y \"" + outputPath + "\" -hide_banner";
    }
    else { // fi.suffix == "gif"
        cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -r " + QString::number(fps, 'f', 2) + " -i \"" + frameDir.absolutePath() + "/%8d.png\"" + getScaleFilter() + " -y \"" + outputPath + "\" -hide_banner";
    }
    process->start(cmd);
    while (process->state() != 0) {
        setProgressBarVal(int(getNumFrameFinished(process) * 100 / numFrame));
        setTimeTaken();
        setRemaining(startTime);
        delay(100);

        if (ui->pushButton_Stop->isDown()) {
            if (ConfirmToStop()) {
                process->kill();
                state = "Stopped";
            }
        }
    }
    delete process;

    EncodingTime = EncodingTime.addSecs(-QTime::currentTime().secsTo(startTime));

    if (state != "Stopped") {
        setProgressBarVal(100);
        frameDir.removeRecursively();
    }
}

void MainWindow::Joining(QDir dir) {
    setTimeTaken();

    QFile list(dir.absolutePath() + "/list.txt");
    if (list.open(QIODevice::WriteOnly)) {
        QString suffix;
        if (ui->comboBox_Tool->currentIndex() == 0) suffix = "_upscaled";
        else suffix = "_interpolated";

        QTextStream stream(&list);
        for (int i = 0; i < numPart; i++) {
            stream << "file '" + dir.absolutePath() + "/" + QString::number(i) + suffix + "." + fo.suffix() + "'\n";
        }
    }
    ui->progressBar->setValue(0);
    process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -f concat -safe 0 -i \"" + dir.absolutePath() + "/list.txt\" -c copy -y \"" + ui->lineEdit_Output->text() + "\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0) {
        setTimeTaken();
        delay(100);
    }
    delete process;

    setProgressBarVal(100);

    list.close();
    dir.removeRecursively();
}

void MainWindow::Resizing(QFileInfo file) {
    setTimeTaken();

    QString output;
    if (type == "dir") output = file.filePath().replace(file.absolutePath(), fo.filePath());
    else if (type == "image") output = fo.filePath();
    else output = file.filePath().replace("_upscaled", "_resized");

    process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -i \"" + file.filePath() + "\" -vf scale=" + getScaleFilter() + ":flags=lanczos -y \"" + output + "\" -hide_banner";
    process->start(cmd);

    if (type == "image") {
        while (process->state() != 0) {
            setTimeTaken();
            delay(100);
        }
        delete process;
    }
}