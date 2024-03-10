#include "mainwindow.h"

int MainWindow::getNumFrameFinished(QProcess* process) {
    QString output = readStdOutput(process);
    if (output.contains("frame="))
        output = output.remove(output.indexOf("fps"), output.length()).remove("frame=");
    return output.toInt();
}

float MainWindow::getDuration(QFileInfo file) {
    setTimeTaken();

    QProcess* process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg/ffmpeg.exe\" -i \"" + file.filePath() + "\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0) {
        setTimeTaken();
        delay(100);
    }
    QString output = readStdOutput(process);

    output = output.remove(0, output.indexOf("Duration") + 10);
    output = output.remove(output.indexOf(','), output.length());

    return toSec(output.split(':'));
}

QString MainWindow::getParameter(QString para) {
    if (para == "model") {
        if (ui->comboBox_Engine->currentIndex() == 0) {
            switch (ui->comboBox_Model->currentIndex()) {
            case 0:
            {
                QString x = " -s ";
                if (ui->comboBox_Res->currentText() == "--") {
                    resizingNeeded = true;
                    if (res[0].toInt() * 2 > ui->lineEdit_Res->text().toInt()) x += "2";
                    else if (res[0].toInt() * 3 > ui->lineEdit_Res->text().toInt()) x += "3";
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
            }
            case 1: case 2: case 3: case 4: case 5:
                if (ui->comboBox_Res->currentText() == "x4") resizingNeeded = false;
                else {
                    resizingNeeded = true;
                    scale = 4;
                }
                return " -n " + ui->comboBox_Model->currentText();
            default:
                QString x, modelName;
                QDir modelsDir = QDir(currentPath + "/realesrgan-ncnn-vulkan/models");
                foreach(QString model, modelsDir.entryList()) {
                    if (model.length() > 3 && model.contains(".bin") && model.sliced(3, model.length() - 7) == ui->comboBox_Model->currentText()) {
                        modelName = model.remove(".bin");
                        break;
                    }
                }
                x += modelName[0];
                if (ui->comboBox_Res->currentText().remove("x") == x) resizingNeeded = false;
                else {
                    resizingNeeded = true;
                    scale = x.QString::toInt();
                }
                return " -n " + modelName + " -s " + x;
            }
        }
        else {
            QString x = " -s ";
            if (ui->comboBox_Res->currentText() == "--") {
                resizingNeeded = true;
                if (res[0].toInt() * 2 > ui->lineEdit_Res->text().toInt()) x += "2";
                else if (res[0].toInt() * 3 > ui->lineEdit_Res->text().toInt()) x += "3";
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

int MainWindow::getTargetNumFrame(int numFrame) {
    setTimeTaken();

    if (ui->comboBox_Fps->currentIndex() == 0) {
        return qRound(numFrame * ui->lineEdit_Fps->text().toFloat() / fps.QString::toFloat());
    }
    else return numFrame * (ui->comboBox_Fps->currentIndex() + 1);
}

QString MainWindow::getNewFps() {
    setTimeTaken();

    if (ui->comboBox_Fps->currentIndex() == 0) {
        return ui->lineEdit_Fps->text();
    }
    else return QString::number(fps.QString::toFloat() * (ui->comboBox_Fps->currentIndex() + 1), 'f', 2);
}

QString MainWindow::getVCodec() {
    if (ui->comboBox_VCodec->currentIndex() == 0) return " -c:v libx264 -vtag avc1";
    else return " -c:v libx265 -vtag hvc1";
}

QStringList getResolutions(QString output) {
    output = output.remove(0, output.indexOf("Stream #0:0:"));
    output = output.remove(0, output.indexOf("yuv"));
    output = output.remove(0, output.indexOf("),") + 3);
    output = output.remove(qMin(output.indexOf(','), output.indexOf(' ')), output.length());
    return QStringList{ output.first(output.indexOf('x')), output.last(output.length() - output.indexOf('x') - 1) };
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