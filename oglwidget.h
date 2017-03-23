#ifndef OGLWIDGET_H
#define OGLWIDGET_H
#include "mainwindow.h"
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

typedef float HOM_MAT[16];
typedef float HOM_Vect[4];
#define MAX_LIMMIT(x,max)  x = ((x>max)?max:x)
#define MIN_LIMMIT(x,min)  x = ((x<min)?min:x)


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

    uint8_t Main_Axis_Rotate;

    uint8_t Horizontal_Axis;

    uint8_t Vertial_Axis;

    uint8_t END_EFFECTOR_YAW;

    uint8_t END_EFFECTOR_Pitch;

}InPlace_Flag;



typedef struct{


    unsigned char head1;
    unsigned char head2;

    uint8_t CMD_ID;

    float Main_Axis_Rotate;

    float Horizontal_Axis;

    float Vertial_Axis;

    float END_EFFECTOR_YAW;

    float END_EFFECTOR_Pitch;

    unsigned char PUMB;

    unsigned char VALVE;

    InPlace_Flag status;

    unsigned char sum;

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


    //para_Def Real_Para;

    //para_Def Aim_Para;

    //void DrawStruct(HOM_MAT AfterTrans,para_Def *_para,HOM_MAT RefMat,bool flag);

    double test;
    double value2;
    double value3;
    double Vx,Vy,Vz;
    double MoveX,MoveY,MoveZ;
    double scale;
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
