#include "mainwindow.h"

int getNumFrameFinished(QProcess* process) {
    QString output = readStdOutput(process);
    if (output.contains("frame="))
        output = output.remove(output.indexOf("fps"), output.length()).remove("frame=");
    return output.toInt();
}

double getDuration(QFileInfo file) {
    QMediaPlayer* player = new QMediaPlayer;
    player->setSource(QUrl(file.absoluteFilePath()));
    QMediaMetaData metaData(player->metaData());
    delete player;

    return qvariant_cast<double>(QVariant(metaData.value(QMediaMetaData::Duration))) / 1000.0;
}

QStringList getGPUID(QString output) {
    QStringList list = output.split("\n[");
    for (int i = 0; i < list.length(); i++) {
        list[i] = list[i].remove(list[i].indexOf(']'), list[i].length()).remove('[');
        list[i] = list[i].insert(0, '[');
        list[i] = list[i].insert(2, ']');
    }
    list.removeDuplicates();
    return list;
}

QString MainWindow::getParameter(QString para) {
    if (para == "model") {
        if (ui->comboBox_Engine->currentIndex() == 0) {
            QString x;
            switch (ui->comboBox_Model->currentIndex()) {
            case 0:
                x = " -s ";
                if (ui->comboBox_Res->currentText() == "--") {
                    resizingNeeded = true;
                    if (res[0] * 2 > ui->lineEdit_Res->text().toInt()) x += "2";
                    else if (res[0] * 3 > ui->lineEdit_Res->text().toInt()) x += "3";
                    else x += "4";
                }
                else if (ui->comboBox_Res->currentText() == "x1") {
                    resizingNeeded = true;
                    x += "2";
                    scale = 2;
                }
                else {
                    resizingNeeded = false;
                    x += ui->comboBox_Res->currentText().remove('x');
                }
                return " -n " + ui->comboBox_Model->currentText() + x;
            
            case 1: case 2: case 3: case 4: case 5:
                if (ui->comboBox_Res->currentText() == "x4") resizingNeeded = false;
                else {
                    resizingNeeded = true;
                    scale = 4;
                }
                return " -n " + ui->comboBox_Model->currentText();
            
            default:
                QString modelName;
                QDir modelsDir = QDir(currentPath + "/realesrgan-ncnn-vulkan/models");
                foreach (QString model, modelsDir.entryList(QDir::Files)) {
                    if (model.contains(".bin") && model.sliced(3, model.length() - 7) == ui->comboBox_Model->currentText()) {
                        modelName = model.remove(".bin");
                        break;
                    }
                }
                x += modelName[0];
                if (ui->comboBox_Res->currentText().remove("x") == x) resizingNeeded = false;
                else {
                    resizingNeeded = true;
                    scale = x.toInt();
                }
                return " -n " + modelName + " -s " + x;
            }
        }
        else {
            QString x = " -s ";
            if (ui->comboBox_Res->currentText() == "--") {
                resizingNeeded = true;
                if (res[0] * 2 > ui->lineEdit_Res->text().toInt()) x += "2";
                else if (res[0] * 3 > ui->lineEdit_Res->text().toInt()) x += "3";
                else x += "4";
            }
            else if (ui->comboBox_Res->currentText() == "x1") {
                resizingNeeded = true;
                x += "2";
                scale = 2;
            }
            else {
                resizingNeeded = false;
                x += ui->comboBox_Res->currentText().remove('x');
            }
            return " -m " + ui->comboBox_Model->currentText() + x;
        }
    }
    else if (para == "gpuid") {
        if (ui->comboBox_GpuID->currentText() != "")
            return " -g " + ui->comboBox_GpuID->currentText();
        else return "";
    }
    else if (para == "thread") {
        if (ui->lineEdit_Thread->text() != "")
            return " -j " + ui->lineEdit_Thread->text();
        else return "";
    }
    else {
        if (ui->comboBox_Engine->currentIndex() == 1)
            return " -n " + QString::number(ui->comboBox_Denoise->currentIndex() - 1) + " -c " + QString::number(ui->comboBox_SyncGap->currentIndex());
        else return "";
    }
}

QString MainWindow::getVCodec() {
    if (ui->comboBox_VCodec->currentIndex() == 0) return " -c:v libx264 -vtag avc1";
    else return " -c:v libx265 -vtag hvc1";
}

QString MainWindow::getScaleFilter() {
    QString scaleFilter;
    if (resizingNeeded && ui->comboBox_Tool->currentIndex() == 0) {
        scaleFilter = " -vf scale=";
        if (ui->comboBox_Res->currentIndex() == 0) scaleFilter += ui->lineEdit_Res->text() + ":" + ui->lineEdit_Res_2->text();
        else {
            int t = ui->comboBox_Res->currentText().remove("x").QString::toInt();
            switch (scale) {
            case 1:
                switch (t) {
                case 2: scaleFilter += "iw*2:ih*2"; break;
                case 3: scaleFilter += "iw*3:ih*3"; break;
                case 4: scaleFilter += "iw*4:ih*4"; break;
                }
                break;
            case 2:
                switch (t) {
                case 1: scaleFilter += "iw*0.5:ih*0.5"; break;
                case 3: scaleFilter += "iw*1.5:ih*1.5"; break;
                case 4: scaleFilter += "iw*2:ih*2"; break;
                }
                break;
            case 4:
                switch (t) {
                case 1: scaleFilter += "iw*0.25:ih*0.25"; break;
                case 2: scaleFilter += "iw*0.5:ih*0.5"; break;
                case 3: scaleFilter += "iw*0.75:ih*0.75"; break;
                }
            }
        }
        scaleFilter += ":flags=lanczos";
    }
    return scaleFilter;
}

int MainWindow::getTargetNumFrame(int numFrame) {
    if (ui->comboBox_Fps->currentIndex() == 0) {
        return qRound(numFrame * ui->lineEdit_Fps->text().toDouble() / fps);
    }
    else return numFrame * (ui->comboBox_Fps->currentIndex() + 1);
}

double MainWindow::getNewFps() {
    if (ui->comboBox_Fps->currentIndex() == 0) {
        return ui->lineEdit_Fps->text().toDouble();
    }
    else return fps * (ui->comboBox_Fps->currentIndex() + 1);
}

void MainWindow::getMetadata() {
    QSize size;

    if (type == "video" || fi.suffix() == "gif") {
        QMediaPlayer* player = new QMediaPlayer;
        player->setSource(QUrl(fi.absoluteFilePath()));
        QMediaMetaData metaData(player->metaData());
        delete player;

        size = qvariant_cast<QSize>(QVariant(metaData.value(QMediaMetaData::Resolution)));

        dur = getDuration(fi);

        fps = qvariant_cast<double>(QVariant(metaData.value(QMediaMetaData::VideoFrameRate)));
        ui->lineEdit_Fps->setText(QString::number(fps, 'f', 2));
    }
    else if (type == "image") {
        size = QImage(fi.absoluteFilePath()).size();
    }
    res[0] = size.width();
    res[1] = size.height();
    ui->lineEdit_Res->setText(QString::number(res[0]));
    ui->lineEdit_Res_2->setText(QString::number(res[1]));
}

void MainWindow::readModelName() {
    QDir modelsDir = QDir(currentPath + "/realesrgan-ncnn-vulkan/models");
    foreach(QString model, modelsDir.entryList(QDir::Files)) {
        if (model.contains(".bin") && model[1] == 'x') {
            ui->comboBox_Model->addItem(model.remove(0, 3).remove(".bin"));
        }
    }
}