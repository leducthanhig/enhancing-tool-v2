#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMimeData>
#include <QFileInfo>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QEventLoop>
#include <QTimer>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setEnabled_Res(0);
    setEnabled_OutFormat(0);
    setEnabled_Segment(0);
    setEnabled_TileSize(1);
    setEnabled_Thread(0);
    setEnabled_VCodec(0);
    setEnabled_CRF(0);
    setEnabled_Fps(0);
    setEnabled_Denoise(0);
    setEnabled_SyncGap(0);
    setVisible_ViewState(0);
    ui->lineEdit_Input->setReadOnly(1);
    ui->label_Output->setEnabled(0);
    ui->lineEdit_Output->setEnabled(0);
    ui->pushButton_Output->setEnabled(0);
    ui->comboBox_GpuID->setEnabled(0);
    ui->lineEdit_TileSize->setText("256");
    ui->spinBox_CRF->setRange(0, 51);
    ui->spinBox_CRF->setValue(23);
    ui->pushButton_Start->setEnabled(0);
    ui->label_Remaining->setVisible(0);
    ui->label_TimeTaken->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->label_TimeTaken, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClickMenu_TimeTaken(QPoint)));
    ui->label_Remaining->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->label_Remaining, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClickMenu_Remaining(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setEnabled_Res(int para)
{
    ui->label_Res->setEnabled(para);
    ui->label_Res_2->setEnabled(para);
    ui->comboBox_Res->setEnabled(para);
    ui->lineEdit_Res->setEnabled(para);
    ui->lineEdit_Res_2->setEnabled(para);
}

void MainWindow::setEnabled_OutFormat(int para)
{
    ui->label_OutFormat->setEnabled(para);
    ui->lineEdit_OutFormat->setEnabled(para);
}

void MainWindow::setEnabled_Segment(int para)
{
    ui->label_Segment->setEnabled(para);
    ui->label_Segment_2->setEnabled(para);
    ui->lineEdit_Segment->setEnabled(para);
}

void MainWindow::setEnabled_TileSize(int para)
{
    ui->label_TileSize->setEnabled(para);
    ui->lineEdit_TileSize->setEnabled(para);
    ui->pushButton_TileSize_Dec->setEnabled(para);
    ui->pushButton_TileSize_Inc->setEnabled(para);
}

void MainWindow::setEnabled_Thread(int para)
{
    ui->lineEdit_Thread->setEnabled(para);
    ui->label_Thread->setEnabled(para);
}

void MainWindow::setEnabled_VCodec(int para)
{
    ui->label_VCodec->setEnabled(para);
    ui->comboBox_VCodec->setEnabled(para);
}

void MainWindow::setEnabled_CRF(int para)
{
    ui->label_CRF->setEnabled(para);
    ui->spinBox_CRF->setEnabled(para);
}

void MainWindow::setEnabled_Fps(int para)
{
    ui->label_Fps->setEnabled(para);
    ui->lineEdit_Fps->setEnabled(para);
    ui->comboBox_Fps->setEnabled(para);
}

void MainWindow::setEnabled_Denoise(int para)
{
    ui->label_Denoise->setEnabled(para);
    ui->comboBox_Denoise->setEnabled(para);
}

void MainWindow::setEnabled_SyncGap(int para)
{
    ui->label_SyncGap->setEnabled(para);
    ui->comboBox_SyncGap->setEnabled(para);
}

void MainWindow::setVisible_ViewState(int para)
{
    ui->groupBox_Timer->setVisible(para);
    ui->progressBar->setVisible(para);
    ui->pushButton_Stop->setVisible(para);
}

void MainWindow::on_comboBox_Tool_currentIndexChanged(int index)
{
    ui->comboBox_Model->clear();

    switch (index)
    {
    case 0:
        ui->comboBox_Model->addItems(QStringList{"RealESRGAN-NCNN-Vulkan", "RealCUGAN-NCNN-Vulkan", "RealSR-NCNN-Vulkan"});

        if (type == "")
        {
            setEnabled_Res(0);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(0);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }

        else if (type == "dir")
        {
            ui->comboBox_Ver->setCurrentIndex(4);
            setEnabled_Res(0);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(1);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }
        else if (type == "image")
        {
            ui->comboBox_Ver->setCurrentIndex(4);
            setEnabled_Res(1);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(0);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }
        else
        {
            ui->comboBox_Ver->setCurrentIndex(2);
            setEnabled_Res(1);
            setEnabled_Segment(1);
            setEnabled_TileSize(1);
            setEnabled_Thread(1);
            setEnabled_VCodec(1);
            setEnabled_CRF(1);
            setEnabled_Fps(0);
        }
        break;
    case 1:
        ui->comboBox_Model->addItems(QStringList{"RIFE-NCNN-Vulkan", "IFRNet-NCNN-Vulkan"});
        ui->comboBox_Ver->setCurrentIndex(11);
        setEnabled_Res(0);
        setEnabled_Segment(1);
        setEnabled_TileSize(0);
        setEnabled_Thread(1);
        setEnabled_VCodec(1);
        setEnabled_CRF(1);
        if (fi.filePath() != "")
        {
            setEnabled_Fps(1);
            ui->comboBox_Fps->setCurrentIndex(1);
        }
        break;
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    fi = QFileInfo(event->mimeData()->urls()[0].toLocalFile());
    setup();
}

void MainWindow::setup()
{
    ui->lineEdit_Res->clear();
    ui->lineEdit_Res_2->clear();
    ui->lineEdit_OutFormat->clear();
    ui->lineEdit_Fps->clear();

    ui->lineEdit_Input->setText(fi.filePath());
    ui->lineEdit_Output->setEnabled(1);
    ui->lineEdit_Output->setEnabled(1);
    ui->pushButton_Output->setEnabled(1);
    ui->pushButton_Start->setEnabled(1);

    if (fi.isDir())
    {
        type = "dir";
        ui->lineEdit_Output->setText(fi.filePath() + "_x_");

        ui->comboBox_Tool->setCurrentIndex(0);
        ui->comboBox_Tool->setEnabled(0);
        ui->comboBox_Model->setCurrentIndex(0);
        ui->comboBox_Ver->setCurrentIndex(4);
        setEnabled_Res(0);
        setEnabled_OutFormat(0);
        setEnabled_Segment(0);
        setEnabled_TileSize(1);
        setEnabled_Thread(1);
        setEnabled_VCodec(0);
        setEnabled_CRF(0);
        setEnabled_Fps(0);

        ui->label_Res->setEnabled(1);
        ui->comboBox_Res->setEnabled(1);
        ui->comboBox_Res->setCurrentIndex(0);
        ui->comboBox_Res->setCurrentIndex(3);
    }
    else
    {
        readMetadata();
        if (type == "image")
        {
            ui->lineEdit_Output->setText(fi.absolutePath() + "/" + fi.baseName() + "_." + fi.suffix());
            ui->lineEdit_OutFormat->setText(fi.suffix());

            ui->comboBox_Tool->setCurrentIndex(0);
            ui->comboBox_Tool->setEnabled(0);
            ui->comboBox_Model->setCurrentIndex(0);
            ui->comboBox_Ver->setCurrentIndex(4);
            setEnabled_Res(1);
            ui->comboBox_Res->setCurrentIndex(0);
            ui->comboBox_Res->setCurrentIndex(3);
            setEnabled_OutFormat(1);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(0);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }
        else
        {
            ui->lineEdit_Output->setText(fi.absolutePath() + "/" + fi.baseName() + "_." + fi.suffix());
            ui->lineEdit_OutFormat->setText(fi.suffix());

            ui->comboBox_Tool->setEnabled(1);
            if (ui->comboBox_Tool->currentIndex() == 0)
            {
                ui->comboBox_Model->setCurrentIndex(0);
                ui->comboBox_Ver->setCurrentIndex(2);
                setEnabled_Res(1);
                ui->comboBox_Res->setCurrentIndex(0);
                ui->comboBox_Res->setCurrentIndex(1);
                setEnabled_TileSize(1);
                setEnabled_Fps(0);
            }
            else
            {
                ui->comboBox_Model->setCurrentIndex(0);
                ui->comboBox_Ver->setCurrentIndex(11);
                setEnabled_Res(0);
                setEnabled_TileSize(0);
                setEnabled_Fps(1);
                ui->comboBox_Fps->setCurrentIndex(0);
                ui->comboBox_Fps->setCurrentIndex(1);
            }
            setEnabled_OutFormat(1);
            setEnabled_Segment(1);
            setEnabled_Thread(1);
            setEnabled_VCodec(1);
            setEnabled_CRF(1);
        }
    }
}

void MainWindow::on_lineEdit_OutFormat_textChanged(const QString suffix)
{
    ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fo.baseName() + '.' + suffix);
}

void MainWindow::on_pushButton_Input_released()
{
    QString url = QFileDialog::getOpenFileName(this, "Open File", QString(),
                                        "All Files (*.*) ;; Image Files (*.png *.jpg *.jpeg) ;; Video Files (*.mp4 *.mkv *.ts)");
    if (url != "")
    {
        fi = QFileInfo(url);
        setup();
    }
}

void MainWindow::on_pushButton_Output_released()
{
    QString url = QFileDialog::getExistingDirectory(this, "Open File", QString());
    if (url != "")
        ui->lineEdit_Output->setText(url + "/" + QFileInfo(ui->lineEdit_Output->text()).fileName());
}

QString readStdOutput(QProcess *process)
{
    return process->readAllStandardError() + process->readAllStandardOutput() + process->readAll();
}

void delay(int time)
{
    QEventLoop loop;
    QTimer::singleShot(time, &loop, SLOT(quit()));
    loop.exec();
}

float toSec(QStringList time)
{
    return time[0].QString::toFloat()*3600 + time[1].QString::toFloat()*60 + time[2].QString::toFloat();
}

QStringList setResolutions(QString output)
{
    output = output.remove(0, output.indexOf("Stream #0:0:"));
    output = output.remove(0, output.indexOf("yuv"));
    output = output.remove(0, output.indexOf("),") + 3);
    output = output.remove(qMin(output.indexOf(','), output.indexOf(' ')), output.length());
    return QStringList{output.first(output.indexOf('x')), output.last(output.length() - output.indexOf('x') - 1)};
}

void MainWindow::readMetadata()
{
    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg.exe\" -i \"" + fi.filePath() + "\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0) delay(100);
    QString output = readStdOutput(process);

    if (output.contains("Metadata")) type = "video";
    else type = "image";

    res = setResolutions(output);
    ui->lineEdit_Res->setText(res[0]);
    ui->lineEdit_Res_2->setText(res[1]);
    if (ui->comboBox_Tool->currentIndex() == 0)
        setEnabled_Res(1);

    if (type == "video")
    {
        output = output.remove(0, output.indexOf("Duration"));
        output = output.remove(output.indexOf("fps") - 1, output.length());

        fps = "";
        int i = output.length() - 1;
        while (output[i] != ' ')
        {
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

void MainWindow::on_pushButton_TileSize_Inc_released()
{
    TileSize = ui->lineEdit_TileSize->text().QString::toInt()*2;
    if (TileSize == 0) TileSize = 32;
    else if (TileSize == 65536)
        ui->pushButton_TileSize_Inc->setEnabled(0);
    ui->pushButton_TileSize_Dec->setEnabled(1);
    ui->lineEdit_TileSize->setText(QString::number(TileSize, 10));
}

void MainWindow::on_pushButton_TileSize_Dec_released()
{
    TileSize = ui->lineEdit_TileSize->text().QString::toInt()/2;
    if (TileSize == 16)
    {
        TileSize = 0;
        ui->pushButton_TileSize_Dec->setEnabled(0);
    }
    ui->pushButton_TileSize_Inc->setEnabled(1);
    ui->lineEdit_TileSize->setText(QString::number(TileSize, 10));
}

void MainWindow::on_comboBox_Model_currentIndexChanged(int index)
{
    ui->comboBox_Ver->clear();
    setEnabled_Denoise(0);
    setEnabled_SyncGap(0);
    if (ui->comboBox_Tool->currentIndex() == 0)
    {
        switch (index)
        {
        case 0:
            ui->comboBox_Ver->addItems(QStringList{"esrgan", "Photo-Conservative", "realesr-animevideov3", "RealESRGANv2-animevideo", "realesrgan-x4plus", "realesrgan-x4plus-anime", "realesr-general-x4v3", "realesr-general-wdn-x4v3", "realesrnet-x4plus", "BIGOLDIES", "LD-Anime_Skr_v1.0"});
            break;
        case 1:
            ui->comboBox_Ver->addItems(QStringList{"models-pro", "models-se"});
            setEnabled_Denoise(1);
            setEnabled_SyncGap(1);
            break;
        case 2:
            ui->comboBox_Ver->addItems(QStringList{"models-DF2K", "models-DF2K_JPEG"});
            break;
        }
    }
    else
    {
        switch (index)
        {
        case 0:
            ui->comboBox_Ver->addItems(QStringList{"rife", "rife-anime", "rife-HD", "rife-UHD", "rife-v2", "rife-v2.3", "rife-v2.4", "rife-v3.0", "rife-v3.1", "rife-v4", "rife-v4.1", "rife-v4.6"});
            ui->comboBox_Ver->setCurrentIndex(11);
            break;
        case 1:
            ui->comboBox_Ver->addItems(QStringList{"IFRNet_GoPro", "IFRNet_L_GoPro", "IFRNet_S_GoPro", "IFRNet_Vimeo90K", "IFRNet_L_Vimeo90K", "IFRNet_S_Vimeo90K"});
            ui->comboBox_Ver->setCurrentIndex(3);
            break;
        }
    }
}

void MainWindow::on_comboBox_Res_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        if (type != "dir")
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_" + ui->lineEdit_Res->text() + 'x'
                                         + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
        break;
    case 1:
        if (type == "dir")
            ui->lineEdit_Output->setText(ui->lineEdit_Output->text().removeLast() + '2');
        else
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_" + QString::number(res[0].QString::toInt()*2, 10)
                                         + 'x' + QString::number(res[1].QString::toInt()*2, 10) + '.' + fo.suffix());
        break;
    case 2:
        if (type == "dir")
            ui->lineEdit_Output->setText(ui->lineEdit_Output->text().removeLast() + '3');
        else
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_" + QString::number(res[0].QString::toInt()*3, 10)
                                         + 'x' + QString::number(res[1].QString::toInt()*3, 10) + '.' + fo.suffix());
        break;
    case 3:
        if (type == "dir")
            ui->lineEdit_Output->setText(ui->lineEdit_Output->text().removeLast() + '4');
        else
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_" + QString::number(res[0].QString::toInt()*4, 10)
                                         + 'x' + QString::number(res[1].QString::toInt()*4, 10) + '.' + fo.suffix());
        break;
    }
}

void MainWindow::on_lineEdit_Res_editingFinished()
{
    if (ui->label_Res->isEnabled())     //When "type == dir" does not support custom resolution
    {
        ui->lineEdit_Res_2->setText(QString::number(
            res[1].QString::toInt()*ui->lineEdit_Res->text().QString::toInt()/res[0].QString::toInt(), 10));
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_" + ui->lineEdit_Res->text() + 'x'
                                     + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
        ui->comboBox_Res->setCurrentIndex(0);
    }
}

void MainWindow::on_lineEdit_Res_2_editingFinished()
{
    if (ui->label_Res->isEnabled())     //When "type == dir" does not support custom resolution
    {
        ui->lineEdit_Res->setText(QString::number(
            res[0].QString::toInt()*ui->lineEdit_Res_2->text().QString::toInt()/res[1].QString::toInt(), 10));
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_" + ui->lineEdit_Res->text() + 'x'
                                     + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
        ui->comboBox_Res->setCurrentIndex(0);
    }
}

QStringList getGPUID(QString output)
{
    QStringList list = output.split("\n[");
    for (int i = 0; i < list.length(); i ++)
    {
        list[i] = list[i].remove(list[i].indexOf(']'), list[i].length()).remove('[');
        list[i] = list[i].insert(0, '[');
        list[i] = list[i].insert(2, ']');
    }
    list.removeDuplicates();
    return list;
}

void MainWindow::on_pushButton_GpuID_released()
{
    ui->pushButton_GpuID->setEnabled(0);

    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/realesrgan-ncnn-vulkan/realesrgan-ncnn-vulkan.exe\" -i check.jpg -o checked.jpg";
    process->start(cmd);
    while (process->state() != 0) delay(100);
    QString output = readStdOutput(process);

    if (output.contains("invalid gpu device"))
    {
        QMessageBox msg(QMessageBox::Critical, "Warning!!!", "Please update your graphic driver!\n", QMessageBox::Ok);
        msg.setStyleSheet("QPushButton{background-color: rgb(66, 66, 66); border-radius: 7px; width: 50px; height: 25px}"
                          "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                          "QPushButton:pressed{background-color: rgb(60, 60, 60)}");
        msg.exec();
    }
    else
    {
        QStringList list = getGPUID(output);
        QMessageBox msg(QMessageBox::NoIcon, "GPU ID", '\n' + list.join("\t\n\n") + "\n\n[-1] Central Processing Unit\t\n",
                        QMessageBox::Close);
        msg.setStyleSheet("QPushButton{background-color: rgb(66, 66, 66); border-radius: 7px; width: 50px; height: 25px}"
                          "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                          "QPushButton:pressed{background-color: rgb(60, 60, 60)}");
        msg.exec();
        ui->comboBox_GpuID->clear();
        for (int i = 0; i < list.length(); i++)
            ui->comboBox_GpuID->addItem(list[i][1]);
        ui->comboBox_GpuID->addItem("-1");
    }
    ui->comboBox_GpuID->setEnabled(1);
    ui->pushButton_GpuID->setEnabled(1);
}

void MainWindow::on_comboBox_Fps_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        if (ui->lineEdit_Fps->text().toFloat() > fps.toFloat())
            ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_"
                                         + ui->lineEdit_Fps->text() + "FPS." + fo.suffix());
        break;
    case 1:
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_"
                                     + QString::number(fps.QString::toFloat()*2, 'f', 2) + "FPS." + fo.suffix());
        break;
    case 2:
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_"
                                     + QString::number(fps.QString::toFloat()*3, 'f', 2) + "FPS." + fo.suffix());
        break;
    case 3:
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_"
                                     + QString::number(fps.QString::toFloat()*4, 'f', 2) + "FPS." + fo.suffix());
        break;
    }
}

void MainWindow::on_lineEdit_Fps_editingFinished()
{
    if (ui->lineEdit_Fps->text().toFloat() > fps.toFloat())
    {
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.baseName() + "_"
                                     + ui->lineEdit_Fps->text() + "FPS." + fo.suffix());
        ui->comboBox_Fps->setCurrentIndex(0);
    }
}

void MainWindow::on_lineEdit_Output_textChanged(const QString text)
{
    fo = QFileInfo(text);
}

bool ConfirmToStop()
{
    QMessageBox msg(QMessageBox::Warning, "Confirmation", "Do you want to stop current process?\n", QMessageBox::Yes | QMessageBox::No);
    msg.setStyleSheet("QPushButton{background-color: rgb(66, 66, 66); border-radius: 7px; width: 50px; height: 25px}"
                     "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                     "QPushButton:pressed{background-color: rgb(60, 60, 60)}");
    if(msg.exec() == QMessageBox::Yes)
        return true;
    else return false;
}

QString secToString(int sec)
{
    return QTime(sec / 3600, (sec % 3600) / 60, sec % 60).toString("hh:mm:ss");
}

void MainWindow::setTimeTaken()
{
    ui->label_TimeTaken->setText("Time taken: " + secToString(int(toSec(QTime::currentTime().toString("hh:mm:ss").split(':'))
                                                               - toSec(startTime.toString("hh:mm:ss").split(':')))));

}

void MainWindow::setRemaining(QTime startTime)
{
    if (ui->progressBar->value() != 0)
    {
        float remaining = (toSec(QTime::currentTime().toString("hh:mm:ss.zzz").split(':'))
                        - toSec(startTime.toString("hh:mm:ss.zzz").split(':'))) * (float(100) / ui->progressBar->value() - 1);
        if (ui->label_Remaining->text() == "Remaining: unknown" ||
                                                        remaining < toSec(ui->label_Remaining->text().remove("Remaining: ").split(':')))
            ui->label_Remaining->setText("Remaining: " + secToString(qRound(remaining)));
    }
    else ui->label_Remaining->setText("Remaining: unknown");
}

int MainWindow::Spliting()
{
    setTimeTaken();

    if (ui->lineEdit_Segment->text() != "" && ui->lineEdit_Segment->text().QString::toInt() < dur)
    {
        QDir srcDir(fi.absolutePath());
        QDir splitedDir(fi.absolutePath() + '/' + fi.baseName() + "_splited");
        if (splitedDir.exists())
            splitedDir.removeRecursively();
        srcDir.mkdir(fi.baseName() + "_splited");

        QProcess *process = new QProcess;
        QString cmd = "\"" + currentPath + "/ffmpeg.exe\" -i \"" + fi.filePath() + "\" -c copy -map 0 -f segment -segment_time " + ui->lineEdit_Segment->text() + " -reset_timestamps 1 \"" + splitedDir.absolutePath() + "/%d." + fi.suffix() + "\" -hide_banner";
        process->start(cmd);
        while (process->state() != 0)
        {
            setTimeTaken();
            delay(100);
        }
        return splitedDir.count() - 2;
    }
    else return 0;
}

void MainWindow::setProgressBarVal(int newVal)
{
    for (int i = ui->progressBar->value(); i <= newVal; i++)
    {
        ui->progressBar->setValue(i);
        delay(3);
    }
}

int MainWindow::getNumFrameFinished(QProcess *process)
{
    QString output = readStdOutput(process);
    if (output.contains("frame="))
        output = output.remove(output.indexOf("fps"), output.length()).remove("frame=");
    return output.toInt();
}

float MainWindow::getDuration(QFileInfo file)
{
    setTimeTaken();

    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg.exe\" -i \"" + file.filePath() + "\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0)
    {
        setTimeTaken();
        delay(100);
    }
    QString output = readStdOutput(process);

    output = output.remove(0, output.indexOf("Duration") + 10);
    output = output.remove(output.indexOf(','), output.length());

    return toSec(output.split(':'));
}

int MainWindow::Decoding(QFileInfo file, int numFrame)
{
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QDir srcDir(file.absolutePath());
    QDir frameDir(file.absolutePath() + '/' + file.baseName() + "_frames");
    if (frameDir.exists())
        frameDir.removeRecursively();
    srcDir.mkdir(file.baseName() + "_frames");

    ui->progressBar->setValue(0);
    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg.exe\" -i \"" + file.filePath() + "\" \"" + frameDir.absolutePath() + "/%8d.png\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0)
    {
        setProgressBarVal(int(getNumFrameFinished(process) * 100 / numFrame));
        setTimeTaken();
        setRemaining(startTime);

        delay(100);

        if(ui->pushButton_Stop->isDown())
        {
            if (ConfirmToStop())
            {
                process->kill();
                state = "Stopped";
            }
        }
    }
    if (state != "Stopped") setProgressBarVal(100);

    return frameDir.count() - 2;
}

QString MainWindow::getParameter(QString para)
{
    if (para == "ver")
    {
        switch (ui->comboBox_Model->currentIndex())
        {
        case 0:
            switch (ui->comboBox_Ver->currentIndex())
            {
            case 0 ... 1:
                if (ui->comboBox_Res->currentText() == "x4") resizingNeeded = false;
                else resizingNeeded = true;

                return " -n " + ui->comboBox_Ver->currentText() + "-x4";
                break;
            case 2:
            {
                QString x;
                if (ui->comboBox_Res->currentText() == "--")
                {
                    resizingNeeded = true;
                    if (res[0].toInt()*2 > ui->lineEdit_Res->text().toInt()) x = " -s 2";
                    else if (res[0].toInt()*3 > ui->lineEdit_Res->text().toInt()) x = " -s 3";
                }
                else
                {
                    resizingNeeded = false;
                    x = " -s " + ui->comboBox_Res->currentText().remove('x');
                }

                return " -n " + ui->comboBox_Ver->currentText() + x;
                break;
            }
            case 3:
            {
                QString x;
                if (ui->comboBox_Res->currentText() == "--")
                {
                    resizingNeeded = true;
                    if (res[0].toInt()*2 > ui->lineEdit_Res->text().toInt()) x = "x2 -s 2";
                    else x = "x4";
                }
                else if (ui->comboBox_Res->currentText() == "x3")
                {
                    resizingNeeded = true;
                    x = "x4";
                }
                else
                {
                    resizingNeeded = false;
                    x = ui->comboBox_Res->currentText() + " -s " + ui->comboBox_Res->currentText().remove('x');
                }

                return " -n " + ui->comboBox_Ver->currentText() + "-xs" + x;
                break;
            }
            case 9 ... 10:
                if (ui->comboBox_Res->currentText() == "x2") resizingNeeded = false;
                else resizingNeeded = true;

                return " -n " + ui->comboBox_Ver->currentText() + "-x2 -s 2";
                break;
            default:
                if (ui->comboBox_Res->currentText() == "x4") resizingNeeded = false;
                else resizingNeeded = true;

                return " -n " + ui->comboBox_Ver->currentText();
            }
            break;
        case 1:
        {
            QString s;
            if (ui->comboBox_Res->currentText() == "--")
            {
                resizingNeeded = true;
                if (res[0].toInt()*2 > ui->lineEdit_Res->text().toInt()) s = " -s 2";
                else if (res[0].toInt()*3 > ui->lineEdit_Res->text().toInt()) s = " -s 3";
                else s = " -s 4";
            }
            else
            {
                resizingNeeded = false;
                s = " -s " + ui->comboBox_Res->currentText().remove('x');
            }

            return " -m " + ui->comboBox_Ver->currentText() + s;
            break;
        }
        case 2:
            if (ui->comboBox_Res->currentText() == "x4") resizingNeeded = false;
            else resizingNeeded = true;

            return " -m " + ui->comboBox_Ver->currentText();
            break;
        }
    }
    else if (para == "gpuid")
    {
        if (ui->comboBox_GpuID->currentText() != "")
            return " -g " + ui->comboBox_GpuID->currentText();
        else return "";
    }
    else if (para == "thread")
    {
        if (ui->lineEdit_Thread->text() != "")
            return " -j " + ui->lineEdit_Thread->text();
        else return "";
    }
    else
    {
        if (ui->comboBox_Model->currentIndex() == 1)
            return " -n " + QString::number(ui->comboBox_Denoise->currentIndex() - 1)
                 + " -c " + QString::number(ui->comboBox_SyncGap->currentIndex());
        else return "";
    }
}

int MainWindow::getTargetNumFrame(int numFrame)
{
    setTimeTaken();

    if (ui->comboBox_Fps->currentIndex() == 0)
    {
        return qRound(numFrame * ui->lineEdit_Fps->text().toFloat() / fps.QString::toFloat());
    }
    else return numFrame * (ui->comboBox_Fps->currentIndex() + 1);
}

void MainWindow::dealWithInterpolatingErr()
{
    if (ui->lineEdit_Thread->text() != "")
    {
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
    else state = "Stopped";
}

void MainWindow::Interpolating(QFileInfo file, int numFrame)
{
    retryInterpolating:;
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QDir srcDir(file.absolutePath());
    QDir frameDir(file.absolutePath() + '/' + file.baseName() + "_interpolated");
    if (frameDir.exists())
        frameDir.removeRecursively();
    srcDir.mkdir(file.baseName() + "_interpolated");

    QString targetNumFrame;     // Only rife-v4 support target num frame
    if ((ui->comboBox_Model->currentIndex() == 0 && ui->comboBox_Ver->currentIndex() < 9) ||
        (ui->comboBox_Model->currentIndex() == 1 && ui->comboBox_Ver->currentIndex() > 2)) targetNumFrame = "";
    else targetNumFrame = " -n " + QString::number(numFrame, 10);

    QString UHD;
    if (res[1] >= "2160") UHD = " -u";
    else UHD = "";

    ui->progressBar->setValue(0);
    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/" + ui->comboBox_Model->currentText().toLower() + "/" + ui->comboBox_Model->currentText().toLower() + ".exe\" -i \"" + file.absolutePath() + '/' + file.baseName() + "_frames" + "\" -o \"" + frameDir.absolutePath() + "\"" + targetNumFrame + " -m " + ui->comboBox_Ver->currentText() + getParameter("gpuid") + getParameter("thread") + UHD;
    process->start(cmd);
    while (process->state() != 0)
    {
        setProgressBarVal((QDir(frameDir.absolutePath()).count() - 2) * 100 / numFrame);
        setTimeTaken();
        setRemaining(startTime);

        delay(100);

        if(ui->pushButton_Stop->isDown())
        {
            if (ConfirmToStop())
            {
                process->kill();
                state = "Stopped";
            }
        }

        if (readStdOutput(process).contains("failed"))
        {
            process->kill();
            dealWithInterpolatingErr();
            if (state != "Stopped") goto retryInterpolating;
        }
    }

    if (state != "Stopped")
    {
        setProgressBarVal(100);
        QDir(file.absolutePath() + '/' + file.baseName() + "_frames").removeRecursively();
    }
}

QString MainWindow::getNewFps()
{
    setTimeTaken();

    if (ui->comboBox_Fps->currentIndex() == 0)
    {
        return ui->lineEdit_Fps->text();
    }
    else return QString::number(fps.QString::toFloat() * (ui->comboBox_Fps->currentIndex() + 1), 'f', 2);
}

QString MainWindow::getVCodec()
{
    if (ui->comboBox_VCodec->currentIndex() == 0)
        return " -c:v libx264 -vtag avc1";
    else return " -c:v libx265 -vtag hvc1";
}

void MainWindow::Encoding(QFileInfo file, QString fps)
{
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QString suffix;
    if (ui->comboBox_Tool->currentIndex() == 0) suffix = "_upscaled";
    else suffix = "_interpolated";

    QDir frameDir(file.absolutePath() + '/' + file.baseName() + suffix);
    QString outputPath;
    if (numPart == 0) outputPath = ui->lineEdit_Output->text();
    else outputPath = frameDir.absolutePath() + '.' + fo.suffix();

    QString scale;
    if (resizingNeeded && ui->comboBox_Tool->currentIndex() == 0)
    {
        scale = " -vf scale=";
        if (ui->comboBox_Res->currentIndex() == 0) scale += ui->lineEdit_Res->text() + ':' + ui->lineEdit_Res_2->text();
        else if (ui->comboBox_Res->currentText() == "x2") scale += "iw*0.5:ih*0.5:flags=lanczos";
        else scale += "iw*0.75:ih*0.75:flags=lanczos";
    }

    ui->progressBar->setValue(0);
    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg.exe\" -r " + fps + " -i \"" + frameDir.absolutePath() + "/%8d.png\" -i \"" + file.filePath() + "\" -map 0:v -map 1:a -c:a copy -crf " + QString::number(ui->spinBox_CRF->value(), 10) + getVCodec() + scale + " -pix_fmt yuv420p -y \"" + outputPath + "\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0)
    {
        setProgressBarVal(int(getNumFrameFinished(process) * 100 / numFrame));
        setTimeTaken();
        setRemaining(startTime);

        delay(100);

        if(ui->pushButton_Stop->isDown())
        {
            if(ConfirmToStop())
            {
                process->kill();
                state = "Stopped";
            }
        }
    }

    if (state != "Stopped")
    {
        setProgressBarVal(100);
        frameDir.removeRecursively();
    }
}

void MainWindow::Joining(QDir dir)
{
    setTimeTaken();

    QFile list(dir.absolutePath() + "/list.txt");
    if (list.open(QIODevice::WriteOnly))
    {
        QString suffix;
        if (ui->comboBox_Tool->currentIndex() == 0)
        {
            if (resizingNeeded) suffix = "_resized";
            else suffix = "_upscaled";
        }
        else suffix = "_interpolated";

        QTextStream stream(&list);
        for (int i = 0; i < numPart; i++)
            stream << "file '" + dir.absolutePath() + "/" + QString::number(i, 10) + suffix + "." + fo.suffix() + "'\n";
    }

    ui->progressBar->setValue(0);
    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg.exe\" -f concat -safe 0 -i \"" + dir.absolutePath() + "/list.txt\" -c copy -y \"" + ui->lineEdit_Output->text() + "\" -hide_banner";
    process->start(cmd);
    while (process->state() != 0)
    {
        setTimeTaken();
        delay(100);
    }
    setProgressBarVal(100);

    list.close();
    dir.removeRecursively();
}

void MainWindow::dealWithUpscalingErr()
{
    if (TileSize > 256)
    {
        TileSize /= 2;
        ui->lineEdit_TileSize->setText(QString::number(TileSize));
    }
    else
    {
        TileSize = 0;
        ui->lineEdit_TileSize->setText(QString::number(TileSize));
    }

    if (ui->lineEdit_Thread->text() != "")
    {
        QStringList list = ui->lineEdit_Thread->text().split(':');
        int load = list[0].toInt(),
            proc = list[1].toInt(),
            save = list[2].toInt();
        if (load == 1 && proc == 1 && save == 1 && TileSize == 0) state = "Stopped";
        if (load > 1) load--;
        if (proc > 1) proc--;
        if (save > 1) save--;
        ui->lineEdit_Thread->setText(QString::number(load, 10) + ':' + QString::number(proc, 10) + ':' + QString::number(save, 10));
    }
    else if (TileSize == 0) state = "Stopped";
}

void MainWindow::Upscaling(QFileInfo input)
{
    retryUpscaling:;
    setTimeTaken();
    QTime startTime = QTime::currentTime();

    QString inputPath;
    if (type == "video") inputPath = input.absolutePath() + '/' + input.baseName() + "_frames";
    else inputPath = input.filePath();

    QString ver = getParameter("ver");
    QString outputPath;
    if (type == "video") outputPath = input.absolutePath() + '/' + input.baseName() + "_upscaled";
    else if (resizingNeeded)
    {
        if (type == "image") outputPath = input.absolutePath() + '/' + input.baseName() + "_upscaled." + fo.suffix();
        else outputPath = input.filePath() + "_upscaled";
    }
    else outputPath = ui->lineEdit_Output->text();

    if (type != "image")
    {
        numFrame = QDir(inputPath).count() - 2;

        QDir dir(outputPath);
        if (dir.exists()) dir.removeRecursively();
        QDir(QFileInfo(outputPath).absolutePath()).mkdir(outputPath);
    }

    ui->progressBar->setValue(0);
    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/" + ui->comboBox_Model->currentText().toLower() + "/" + ui->comboBox_Model->currentText().toLower() + ".exe\" -i \"" + inputPath + "\" -o \"" + outputPath + "\"" + ver + " -t " + ui->lineEdit_TileSize->text() + getParameter("noise_syncgap") + getParameter("gpuid") + getParameter("thread");
    process->start(cmd);
    while (process->state() != 0)
    {
        setTimeTaken();
        setRemaining(startTime);

        delay(100);

        if(ui->pushButton_Stop->isDown())
        {
            if(ConfirmToStop())
            {
                process->kill();
                state = "Stopped";
            }
        }

        QString output = readStdOutput(process);
        if (type != "image") setProgressBarVal((QDir(outputPath).count() - 2) * 100 / numFrame);
        else if (output.contains("%\r\n")) setProgressBarVal(qRound(output.remove("%\r\n").toFloat()));

        if (output.contains("alpha") && fo.suffix() == "jpg")
        {
            process->kill();
            state = "Stopped";
            QMessageBox msg(QMessageBox::Critical, "Conversion failed!", "Detected alpha channel!", QMessageBox::Close);
            msg.setStyleSheet("QPushButton{background-color: rgb(66, 66, 66); border-radius: 7px; width: 50px; height: 25px}"
                              "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                              "QPushButton:pressed{background-color: rgb(60, 60, 60)}");
            msg.exec();
        }

        if (output.contains("failed"))
        {
            process->kill();
            dealWithUpscalingErr();
            if (state != "Stopped") goto retryUpscaling;
        }
    }

    if (state != "Stopped")
    {
        setProgressBarVal(100);
        if (type == "video") QDir(inputPath).removeRecursively();
    }
}

void MainWindow::Resizing(QFileInfo file)
{
    setTimeTaken();

    QString scale, output;
    if (ui->comboBox_Res->currentIndex() == 0) scale = ui->lineEdit_Res->text() + ':' + ui->lineEdit_Res_2->text();
    else if (ui->comboBox_Ver->currentIndex() == 9 || ui->comboBox_Ver->currentIndex() == 10) {
        if (ui->comboBox_Res->currentText() == "x3") scale = "iw*1.5:ih*1.5";
        else scale = "iw*2:ih*2";
    }
    else if (ui->comboBox_Res->currentText() == "x2") scale = "iw*0.5:ih*0.5";
    else scale = "iw*0.75:ih*0.75";

    if (type == "dir") output = file.filePath().replace(file.absolutePath(), fo.filePath());
    else if (type == "image") output = fo.filePath();
    else output = file.filePath().replace("_upscaled", "_resized");

    QProcess *process = new QProcess;
    QString cmd = "\"" + currentPath + "/ffmpeg.exe\" -i \"" + file.filePath() + "\" -vf scale=" + scale + ":flags=lanczos -y \"" + output + "\" -hide_banner";
    process->start(cmd);

    if (type == "image")
    {
        while (process->state() != 0)
        {
            setTimeTaken();
            delay(100);
        }
    }
}

void MainWindow::on_pushButton_Start_released()
{
    startTime = QTime::currentTime();
    state = "Started";

    ui->pushButton_Start->setVisible(0);
    setVisible_ViewState(1);
    ui->groupBox_IO->setEnabled(0);
    ui->comboBox_Tool->setEnabled(0);
    ui->groupBox_Setting->setEnabled(0);

    switch (ui->comboBox_Tool->currentIndex())
    {
    case 0:
        if (type == "dir")
        {
            ui->progressBar->setFormat("%p% - Upscaling...");
            Upscaling(fi);
            if (state == "Stopped") goto end;

            if (resizingNeeded)
            {
                ui->progressBar->setFormat("%p% - Resizing...");
                ui->progressBar->setValue(0);

                QDir outDir(fo.filePath());
                if (outDir.exists()) outDir.removeRecursively();
                QDir(fo.absolutePath()).mkdir(fo.filePath());

                QDir inDir(fi.filePath() + "_upscaled");
                QStringList list = inDir.entryList();

                QTime startTime = QTime::currentTime();
                for (int i = 2; i < list.length(); i++)
                {
                    Resizing(QFileInfo(inDir.path() + "/" + list[i]));

                    delay(300);
                    setProgressBarVal((i - 1) * 100 / numFrame);
                    setRemaining(startTime);

                    if(ui->pushButton_Stop->isDown())
                    {
                        if(ConfirmToStop())
                        {
                            state = "Stopped";
                            goto end;
                        }
                    }
                }
                while (QDir(outDir.path()).count() - 2 < numFrame)
                {
                    setTimeTaken();
                    delay(100);
                }

                setProgressBarVal(100);

                inDir.removeRecursively();
            }
        }
        else if (type == "image")
        {
            ui->progressBar->setFormat("%p% - Upscaling...");
            Upscaling(fi);
            if (state == "Stopped") goto end;

            if (resizingNeeded)
            {
                ui->progressBar->setFormat("%p% - Resizing...");
                ui->progressBar->setValue(0);

                Resizing(QFileInfo(fi.absolutePath() + '/' + fi.baseName() + "_upscaled." + fo.suffix()));

                setProgressBarVal(100);

                QFile(fi.absolutePath() + '/' + fi.baseName() + "_upscaled." + fo.suffix()).remove();
            }
        }
        else
        {
            numPart = Spliting();

            if (numPart == 0)
            {
                ui->progressBar->setFormat("%p% - Decoding...");
                numFrame = Decoding(fi, int(dur * fps.QString::toFloat()));
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - Upscaling...");
                Upscaling(fi);
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - Encoding...");
                Encoding(fi, fps);
                if (state == "Stopped") goto end;
            }
            else
            {
                for (int i = 0; i < numPart; i++)
                {
                    QFileInfo file = QFileInfo(fi.absolutePath() + '/' + fi.baseName() + "_splited/"
                                               + QString::number(i, 10) + '.' + fi.suffix());
                    dur = getDuration(file);

                    ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Decoding...");
                    numFrame = Decoding(file, int(dur * fps.QString::toFloat()));
                    if (state == "Stopped") goto end;

                    ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Upscaling...");
                    Upscaling(file);
                    if (state == "Stopped") goto end;

                    ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Encoding...");
                    Encoding(file, fps);
                    if (state == "Stopped") goto end;
                }
                ui->progressBar->setFormat("%p% - Joining splited parts...");
                Joining(QDir(fi.absolutePath() + '/' + fi.baseName() + "_splited"));
            }
        }
        break;
    case 1:
        numPart = Spliting();

        if (numPart == 0)
        {
            ui->progressBar->setFormat("%p% - Decoding...");
            numFrame = Decoding(fi, int(dur * fps.QString::toFloat()));
            if (state == "Stopped") goto end;

            ui->progressBar->setFormat("%p% - Interpolating...");
            int targetNumFrame = getTargetNumFrame(numFrame);
            Interpolating(fi, targetNumFrame);
            if (state == "Stopped") goto end;

            ui->progressBar->setFormat("%p% - Encoding...");
            QString newFps = getNewFps();
            Encoding(fi, newFps);
            if (state == "Stopped") goto end;
        }
        else
        {
            for (int i = 0; i < numPart; i++)
            {
                QFileInfo file = QFileInfo(fi.absolutePath() + '/' + fi.baseName() + "_splited/"
                                           + QString::number(i, 10) + '.' + fi.suffix());
                dur = getDuration(file);

                ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Decoding...");
                numFrame = Decoding(file, int(dur * fps.QString::toFloat()));
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Interpolating...");
                int targetNumFrame = getTargetNumFrame(numFrame);
                Interpolating(file, targetNumFrame);
                if (state == "Stopped") goto end;

                ui->progressBar->setFormat("%p% - " + QString::number(i + 1) + " of " + QString::number(numPart) + ": Encoding...");
                QString newFps = getNewFps();
                Encoding(file, newFps);
                if (state == "Stopped") goto end;
            }
            ui->progressBar->setFormat("%p% - Joining splited parts...");
            Joining(QDir(fi.absolutePath() + '/' + fi.baseName() + "_splited"));
        }
        break;
    }

    state = "Finished";
    end:;
    QMessageBox msg(QMessageBox::Information, "Notification", state + "\n\n" + ui->label_TimeTaken->text() + '\n', QMessageBox::Close);
    msg.setStyleSheet("QPushButton{background-color: rgb(66, 66, 66); border-radius: 7px; width: 50px; height: 25px}"
                      "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                      "QPushButton:pressed{background-color: rgb(60, 60, 60)}");
    msg.exec();
    setVisible_ViewState(0);
    ui->pushButton_Start->setVisible(1);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("%p% - Preparing...");
    ui->groupBox_IO->setEnabled(1);
    if (type == "video") ui->comboBox_Tool->setEnabled(1);
    ui->groupBox_Setting->setEnabled(1);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msg(QMessageBox::Question, "Confirmation", "Do you want to close?\n", QMessageBox::Yes | QMessageBox::No);
    msg.setStyleSheet("QPushButton{background-color: rgb(66, 66, 66); border-radius: 7px; width: 50px; height: 25px}"
                      "QPushButton:hover{background-color: rgb(80, 80, 80)}"
                      "QPushButton:pressed{background-color: rgb(60, 60, 60)}");
    if(msg.exec() == QMessageBox::Yes)
    {
        QProcess *close = new QProcess;
        close->start("taskkill /im \"Enhancing Tool.exe\" /f /t");
        event->accept();
    }
    else event->ignore();
}

void MainWindow::on_comboBox_Ver_currentIndexChanged(int index)
{
    if (ui->comboBox_Tool->currentIndex() == 1 && type == "video")
    {
        if ((ui->comboBox_Model->currentIndex() == 0 && index < 9) || (ui->comboBox_Model->currentIndex() == 1 && index > 2))
        {
            ui->comboBox_Fps->setCurrentIndex(1);
            ui->lineEdit_Fps->setEnabled(0);
            ui->comboBox_Fps->setEnabled(0);
        }
        else
        {
            ui->lineEdit_Fps->setEnabled(1);
            ui->comboBox_Fps->setEnabled(1);
        }
    }
}

void MainWindow::rightClickMenu_TimeTaken(QPoint pos)
{
    QMenu Menu(this);
    QAction showRemaining("Show \"Remaining\"", this);
    connect(&showRemaining, SIGNAL(triggered()), this, SLOT(Remaining()));
    Menu.addAction(&showRemaining);
    Menu.exec(ui->label_TimeTaken->mapToGlobal(pos));
}

void MainWindow::rightClickMenu_Remaining(QPoint pos)
{
    QMenu Menu(this);
    QAction showTimeTaken("Show \"Time taken\"", this);
    connect(&showTimeTaken, SIGNAL(triggered()), this, SLOT(TimeTaken()));
    Menu.addAction(&showTimeTaken);
    Menu.exec(ui->label_Remaining->mapToGlobal(pos));
}
