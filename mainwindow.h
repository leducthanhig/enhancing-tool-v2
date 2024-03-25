#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEventLoop>
#include <QFileDialog>
#include <QFileInfo>
#include <QImage>
#include <QMainWindow>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QMimeDatabase>
#include <QMimeType>
#include <QProcess>
#include <QSharedMemory>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QList<int>  res{0, 0};
    QFileInfo   fi, fo;
    QProcess*   process;
    QString     type, state;
    QString     currentPath = qApp->applicationDirPath();
    QTime       startTime, DecodingTime, InterpolatingTime, UpscalingTime, EncodingTime;
    double      dur, fps;
    bool        resizingNeeded, customOutputFileName;
    int         numFrame, numPart, scale;

    int     getTargetNumFrame(int numFrame);
    int     Spliting();
    int     Decoding(QFileInfo file, int numFrame);
    void    Interpolating(QFileInfo file, int numFrame);
    void    Upscaling(QFileInfo input, int numFrame = 1);
    void    Resizing(QFileInfo file);
    void    Encoding(QFileInfo file, double fps);
    void    Joining(QDir dir);
    void    dealWithInterpolatingErr();
    void    dealWithUpscalingErr();
    void    setEnabled_Res(int para);
    void    setEnabled_OutFormat(int para);
    void    setEnabled_Segment(int para);
    void    setEnabled_TileSize(int para);
    void    setEnabled_Thread(int para);
    void    setEnabled_VCodec(int para);
    void    setEnabled_CRF(int para);
    void    setEnabled_Fps(int para);
    void    setEnabled_Denoise(int para);
    void    setEnabled_SyncGap(int para);
    void    setVisible_ViewState(int para);
    void    setProgressBarVal(int newVal);
    void    setTimeTaken();
    void    setRemaining(QTime startTime);
    void    setup();
    void    readModelName();
    void    getMetadata();
    double  getNewFps();
    QString getVCodec();
    QString getScaleFilter();
    QString getParameter(QString para);

private slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);
    void on_comboBox_Tool_currentIndexChanged(int index);
    void on_comboBox_Res_currentIndexChanged(int index);
    void on_comboBox_Engine_currentIndexChanged(int index);
    void on_comboBox_Model_currentIndexChanged(int index);
    void on_comboBox_Fps_currentIndexChanged(int index);
    void on_comboBox_Denoise_currentIndexChanged(int index);
    void on_comboBox_Presets_currentIndexChanged(int index);
    void on_lineEdit_Output_textChanged(QString text);
    void on_lineEdit_Output_textEdited();
    void on_lineEdit_Res_textEdited(QString text);
    void on_lineEdit_Res_2_textEdited(QString text);
    void on_lineEdit_OutFormat_textEdited(QString suffix);
    void on_lineEdit_Fps_textEdited(QString text);
    void on_pushButton_Input_released();
    void on_pushButton_Output_released();
    void on_pushButton_TileSize_Inc_released();
    void on_pushButton_TileSize_Dec_released();
    void on_pushButton_GpuID_released();
    void on_pushButton_Start_released();
    void on_pushButton_Info_released();
    void rightClickMenu_TimeTaken(QPoint pos);
    void rightClickMenu_Remaining(QPoint pos);
    inline void TimeTaken() {ui->label_TimeTaken->setVisible(1); ui->label_Remaining->setVisible(0);}
    inline void Remaining() {ui->label_TimeTaken->setVisible(0); ui->label_Remaining->setVisible(1);}

private:
    Ui::MainWindow *ui;
};

int     getNumFrameFinished(QProcess* process);
void    setTheme();
void    delay(int time);
bool    ConfirmToStop();
bool    containsNonAnsi(QString s);
double  getDuration(QFileInfo file);
QString readStdOutput(QProcess* process);
QStringList getGPUID(QString output);

#endif
