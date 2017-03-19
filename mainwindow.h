#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QString"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <iostream>
#include "clientclass.h"
#include "oglwidget.h"
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage rgbd,depth,contours,proj;
    uint8_t data[1];

    enum changeImageE{
        PAGE1=0,
        PAGE2=1
    }Page;

private slots:

    void  acceptConnection();

   // void update_mat_display(float*,int index);

    void repaint();

    void drawPoint(ReadData_Transform* data);

    void para_display(OGLWidget::para_Def);

    void Uart_Send(OGLWidget::para_Def *);

    OGLWidget::para_Def caculateInvers(float x,float y,float z,float rotate);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_startPort_Button_clicked();

    void on_pubm_Contorl_Button_clicked();

    void on_solid_control_button_clicked();

    void Read_Data();

    void on_Rotate_valueChanged(int value);

    void tttest();

    void pushRgbd(unsigned char* imageData,int cols,int rows,int bytesPerLine);
    void pushDepth(unsigned char* imageData,int cols,int rows,int bytesPerLine);
    void pushContours(unsigned char* imageData,int cols,int rows,int bytesPerLine);
    void pushProj(unsigned char* imageData,int cols,int rows,int bytesPerLine);
    void on_chageImages_clicked();

    void on_setOrigin_clicked();

    void on_shutDownKinect_clicked();

    void on_readParam_clicked();

    void on_reconnect_clicked();

signals:
    void setBackGround();
    void changeMode();
    void setOrigin();
    void shutDownKinect();
    void readParam();
    void reconnect();
private:

    void setBackgroundColor(QWidget *widget,QColor color);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
