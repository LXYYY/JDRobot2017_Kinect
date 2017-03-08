#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include "GL/gl.h"
#include "GL/glu.h"
#include "QMouseEvent"
#include "QWheelEvent"
#include "QPoint"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
//#include "globject.h"
//#include "vector"



using namespace std;
typedef struct {

    int id;
    float rotate_x;
    float rotate_y;
    float rotate_z;
    float trans_x;
    float trans_y;
    float trans_z;

}joint;


typedef struct{

        float Servo_Motor_Pitch_Angle;
        float Servo_Motor_Yaw_Angle;
        uint16_t DC_Motor_H1_Distance;

}USB_Trans_TypeDef;


class gLObject;
#include "globject.h"
 class OGLWidget : public QOpenGLWidget
{
Q_OBJECT
signals:

    void update_mat_info(float*,int);


public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

    typedef struct{
        float x;
        float y;
        float z;
    }Draw_point;

    double test;
    double value2;
    double value3;
    double Vx,Vy,Vz;
    double MoveX,MoveY,MoveZ;
    double scale;
    vector<float*> points;
    joint joint1,joint2,joint3,joint4;
    bool needRepaint;
    QSerialPort *serial;
    bool serialReady;
    //std::vector<gLObject*> object;
    gLObject object[3];
    float  p1[3];
    float  p2[3];
    float  p3[3];
    //gLObject s;
    //QString s1;
    //gLObject s;
 private:

protected:

    QPoint MouseStart;
    QPoint nowAngle;
    QPoint nowPosition;
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseMoveEvent ( QMouseEvent  * e );
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

 private  slots:

    void TimeBasedPaint(void);


};

#endif // OGLWIDGET_H
