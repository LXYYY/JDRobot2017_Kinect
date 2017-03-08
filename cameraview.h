#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QOpenGLWidget>
#include "GL/gl.h"
#include "GL/glu.h"
class CameraView :public QOpenGLWidget
{
public:
    CameraView(QWidget *parent = 0);
    ~CameraView();
    double CameraX;
    double CameraY;
    double CameraZ;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

};

#endif // CAMERAVIEW_H
