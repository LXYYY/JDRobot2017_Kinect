#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "QMouseEvent"
#include "QWheelEvent"
#include "QPoint"

typedef struct {

    int id;
    float rotate_x;
    float rotate_y;
    float rotate_z;
    float trans_x;
    float trans_y;
    float trans_z;

}joint;


 class OGLWidget : public QOpenGLWidget
{
Q_OBJECT
signals:

    void update_mat_info(float*,int);


public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();
    double test;
    double value2;
    double value3;
    double Vx,Vy,Vz;
    double MoveX,MoveY,MoveZ;
    double scale;
    joint joint1,joint2,joint3;

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
