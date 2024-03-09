#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
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
    readModelName();
}

MainWindow::~MainWindow() {
    delete ui;
}
