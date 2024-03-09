#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    QStringList res;
    QFileInfo   fi, fo;
    QString     currentPath = qApp->applicationDirPath();
    QString     fps, type, state;
    QTime       startTime;
    float       dur;
    bool        resizingNeeded;
    int         TileSize, numFrame, numPart;

    int     Spliting();
    int     Decoding(QFileInfo file, int numFrame);
    int     getNumFrameFinished(QProcess *process);
    int     getTargetNumFrame(int numFrame);
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
    void    readMetadata();
    void    setup();
    void    setProgressBarVal(int newVal);
    void    Interpolating(QFileInfo file, int numFrame);
    void    Encoding(QFileInfo file, QString fps);
    void    setTimeTaken();
    void    setRemaining(QTime startTime);
    void    dealWithInterpolatingErr();
    void    Joining(QDir dir);
    void    Upscaling(QFileInfo input);
    void    Resizing(QFileInfo file);
    void    dealWithUpscalingErr();
    float   getDuration(QFileInfo file);
    QString getParameter(QString para);
    QString getNewFps();
    QString getVCodec();

    ~MainWindow();

private slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);
    void on_comboBox_Tool_currentIndexChanged(int index);
    void on_comboBox_Res_currentIndexChanged(int index);
    void on_comboBox_Model_currentIndexChanged(int index);
    void on_comboBox_Ver_currentIndexChanged(int index);
    void on_comboBox_Fps_currentIndexChanged(int index);
    void on_lineEdit_Output_textChanged(const QString text);
    void on_lineEdit_Res_editingFinished();
    void on_lineEdit_Res_2_editingFinished();
    void on_lineEdit_OutFormat_textChanged(const QString suffix);
    void on_lineEdit_Fps_editingFinished();
    void on_pushButton_Input_released();
    void on_pushButton_Output_released();
    void on_pushButton_TileSize_Inc_released();
    void on_pushButton_TileSize_Dec_released();
    void on_pushButton_GpuID_released();
    void on_pushButton_Start_released();
    void rightClickMenu_TimeTaken(QPoint pos);
    void rightClickMenu_Remaining(QPoint pos);
    inline void TimeTaken() {ui->label_TimeTaken->setVisible(1); ui->label_Remaining->setVisible(0);}
    inline void Remaining() {ui->label_TimeTaken->setVisible(0); ui->label_Remaining->setVisible(1);}

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
