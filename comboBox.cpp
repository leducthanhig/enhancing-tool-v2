#include "mainwindow.h"

void MainWindow::on_comboBox_Presets_currentIndexChanged(int index) {
    if (ui->comboBox_Tool->currentIndex() == 0) { // If the combo box is currently Upscaling
        ui->comboBox_Engine->setCurrentIndex(0); // RealESRGAN-NCNN-Vulkan
        if (ui->comboBox_Res->isEnabled()) ui->comboBox_Res->setCurrentIndex(0); // Reset the option
        switch (index) {
        case 0: // HD animes
            if (ui->comboBox_Res->isEnabled()) ui->comboBox_Res->setCurrentIndex(2); // x2
            ui->comboBox_Model->setCurrentText("AnimeJaNai-HD-V3-Super");
            ui->lineEdit_TileSize->setText("1024");
            if (ui->lineEdit_Thread->isEnabled()) ui->lineEdit_Thread->setText("2:2:2");
            break;
        case 1: // SD animes
            if (ui->comboBox_Res->isEnabled()) ui->comboBox_Res->setCurrentIndex(2); // x2
            ui->comboBox_Model->setCurrentText("AniScale2S-Compact");
            ui->lineEdit_TileSize->setText("1024");
            if (ui->lineEdit_Thread->isEnabled()) ui->lineEdit_Thread->setText("3:3:3");
            break;
        case 2: // Animated arts
            if (ui->comboBox_Res->isEnabled()) ui->comboBox_Res->setCurrentIndex(4); // x4
            ui->comboBox_Model->setCurrentText("NMKD-UltraYandere");
            ui->lineEdit_TileSize->setText("256");
            if (ui->lineEdit_Thread->isEnabled()) ui->lineEdit_Thread->setText("2:2:2");
            break;
        case 3: // Realistic or AI generated images
            if (ui->comboBox_Res->isEnabled()) ui->comboBox_Res->setCurrentIndex(4); // x4
            ui->comboBox_Model->setCurrentText("DF2K_JPEG");
            ui->lineEdit_TileSize->setText("256");
            if (ui->lineEdit_Thread->isEnabled()) ui->lineEdit_Thread->setText("2:2:2");
            break;
        default: // Text pictures
            if (ui->comboBox_Res->isEnabled()) ui->comboBox_Res->setCurrentIndex(4); // x4
            ui->comboBox_Model->setCurrentText("realesrnet-x4plus");
            ui->lineEdit_TileSize->setText("256");
            if (ui->lineEdit_Thread->isEnabled()) ui->lineEdit_Thread->setText("2:2:2");
        }
    }
    else { // If the combo box is currently Interpolating
        if (ui->comboBox_Fps->isEnabled()) {
            ui->comboBox_Fps->setCurrentIndex(0); // Reset the option
            ui->comboBox_Fps->setCurrentIndex(1); // x2
        }
        ui->lineEdit_Thread->setText("2:2:2");
        if (index == 0) { // Fast but low quality
            ui->comboBox_Engine->setCurrentIndex(0); // RIFE-NCNN-Vulkan
            ui->comboBox_Model->setCurrentText("rife-v4.6");
        }
        else { // Slow but high quality
            ui->comboBox_Engine->setCurrentIndex(1); // IFRNet-NCNN-Vulkan
            ui->comboBox_Model->setCurrentText("IFRNet_Vimeo90K");
        }
    }
}

void MainWindow::on_comboBox_Tool_currentIndexChanged(int index) {
    ui->comboBox_Engine->clear();
    ui->comboBox_Presets->clear();

    if (index == 0) {
        ui->comboBox_Engine->addItems(QStringList{ "RealESRGAN-NCNN-Vulkan", "RealCUGAN-NCNN-Vulkan" });
        ui->comboBox_Presets->addItems(QStringList{ "HD animes", "SD animes", "Animated arts", "Realistic or AI generated images", "Text pictures" });
        ui->comboBox_Fps->setCurrentIndex(0);

        if (type == "") {
            setEnabled_Res(0);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(0);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }
        else if (type == "dir") {
            setEnabled_Res(0);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(1);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }
        else if (type == "image") {
            setEnabled_Res(1);
            setEnabled_Segment(0);
            setEnabled_TileSize(1);
            setEnabled_Thread(0);
            setEnabled_VCodec(0);
            setEnabled_CRF(0);
            setEnabled_Fps(0);
        }
        else {
            setEnabled_Res(1);
            setEnabled_Segment(1);
            setEnabled_TileSize(1);
            setEnabled_Thread(1);
            setEnabled_VCodec(1);
            setEnabled_CRF(1);
            setEnabled_Fps(0);
        }
        // Reset the option
        ui->comboBox_Presets->setCurrentIndex(1);
        ui->comboBox_Presets->setCurrentIndex(0);
    }
    else {
        ui->comboBox_Engine->addItems(QStringList{ "RIFE-NCNN-Vulkan", "IFRNet-NCNN-Vulkan" });
        ui->comboBox_Presets->addItems(QStringList{ "Fast but low quality", "Slow but high quality" });
        ui->comboBox_Res->setCurrentIndex(0);
        setEnabled_Res(0);
        setEnabled_Segment(1);
        setEnabled_TileSize(0);
        setEnabled_Thread(1);
        setEnabled_VCodec(1);
        setEnabled_CRF(1);
        if (fi.filePath() != "") {
            setEnabled_Fps(1);
        }
        // Reset the option
        ui->comboBox_Presets->setCurrentIndex(1);
        ui->comboBox_Presets->setCurrentIndex(0);
    }
}

void MainWindow::on_comboBox_Engine_currentIndexChanged(int index) {
    ui->comboBox_Model->clear();
    setEnabled_Denoise(0);
    setEnabled_SyncGap(0);
    if (ui->comboBox_Tool->currentIndex() == 0) {
        if (index == 0) {
            ui->comboBox_Model->addItems(QStringList{ "realesr-animevideov3", "realesrgan-x4plus", "realesrgan-x4plus-anime", "realesr-general-x4v3", "realesr-general-wdn-x4v3", "realesrnet-x4plus" });
            readModelName();
        }
        else {
            ui->comboBox_Model->addItems(QStringList{ "models-pro", "models-se" });
            setEnabled_Denoise(1);
            setEnabled_SyncGap(1);
        }
    }
    else {
        if (index == 0) {
            ui->comboBox_Model->addItems(QStringList{ "rife", "rife-anime", "rife-HD", "rife-UHD", "rife-v2", "rife-v2.3", "rife-v2.4", "rife-v3.0", "rife-v3.1", "rife-v4", "rife-v4.1", "rife-v4.6" });
        }
        else {
            ui->comboBox_Model->addItems(QStringList{ "IFRNet_GoPro", "IFRNet_L_GoPro", "IFRNet_S_GoPro", "IFRNet_Vimeo90K", "IFRNet_L_Vimeo90K", "IFRNet_S_Vimeo90K" });
        }
    }
}

void MainWindow::on_comboBox_Res_currentIndexChanged(int index) {
    if (index == 0) {
        if (type == "dir") ui->comboBox_Res->setCurrentIndex(1);
        else ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Res->text() + 'x' + ui->lineEdit_Res_2->text() + '.' + fo.suffix());
    }
    else {
        if (type == "dir") ui->lineEdit_Output->setText(ui->lineEdit_Output->text().removeLast() + QString::number(index));
        else ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + QString::number(res[0].QString::toInt() * index) + 'x' + QString::number(res[1].QString::toInt() * index) + '.' + fo.suffix());
    }

    if ((index == 3 || index == 4) && (ui->comboBox_Denoise->currentIndex() == 2 || ui->comboBox_Denoise->currentIndex() == 3) && ui->comboBox_Engine->currentIndex() == 1) {
        QMessageBox msg(QMessageBox::Critical, "Warning!!!", "This denoise level is not supported!\n", QMessageBox::Close);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
        ui->comboBox_Denoise->setCurrentIndex(0);
    }
}

void MainWindow::on_comboBox_Fps_currentIndexChanged(int index) {
    if (index == 0) {
        if (ui->lineEdit_Fps->text().toFloat() > fps.toFloat()) ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + ui->lineEdit_Fps->text() + "FPS." + fo.suffix());
    }
    else {
        ui->lineEdit_Output->setText(fo.absolutePath() + "/" + fi.completeBaseName() + "_" + QString::number(fps.QString::toFloat() * (index + 1), 'f', 2) + "FPS." + fo.suffix());
    }
}

void MainWindow::on_comboBox_Model_currentIndexChanged(int index) {
    if (ui->comboBox_Tool->currentIndex() == 1 && type == "video") {
        if ((ui->comboBox_Engine->currentIndex() == 0 && index < 9) || (ui->comboBox_Engine->currentIndex() == 1 && index > 2)) {
            ui->comboBox_Fps->setCurrentIndex(1);
            ui->lineEdit_Fps->setEnabled(0);
            ui->comboBox_Fps->setEnabled(0);
        }
        else {
            ui->lineEdit_Fps->setEnabled(1);
            ui->comboBox_Fps->setEnabled(1);
        }
    }
}

void MainWindow::on_comboBox_Denoise_currentIndexChanged(int index) {
    if ((index == 2 || index == 3) && (ui->comboBox_Res->currentIndex() == 3 || ui->comboBox_Res->currentIndex() == 4) && ui->comboBox_Engine->currentIndex() == 1) {
        QMessageBox msg(QMessageBox::Critical, "Warning!!!", "This denoise level is not supported!\n", QMessageBox::Close);
        msg.setStyleSheet("QPushButton{height: 25px}");
        msg.exec();
        ui->comboBox_Denoise->setCurrentIndex(0);
    }
}
