#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QString"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <iostream>
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

    void update_mat_display(float*,int index);

    void repaint();

    void draw1Point(float* points, int size);

    void on_Main_Axis_ADJ_valueChanged(int value);

    void on_Horizontal_Axis_ADJ_valueChanged(int value);

    void on_Vertical_Axis_ADJ_valueChanged(int value);

    void on_END_Effecter_ADJ1_valueChanged(int value);

    void on_END_Effecter_ADJ2_valueChanged(int value);

    void on_Move_X_valueChanged(int value);

    void on_Move_Y_valueChanged(int value);

    void on_Move_z_valueChanged(int value);

    void caculateInvers(float x,float y,float z);

    void on_pushButton_clicked();

    void timeup();
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

signals:
    void setBackGround();
    void changeMode();
    void setOrigin();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
