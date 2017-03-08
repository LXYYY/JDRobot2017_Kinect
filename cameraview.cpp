#include "cameraview.h"

CameraView::CameraView(QWidget *parent)
    : QOpenGLWidget(parent)
{

}
CameraView::~CameraView()
{

}

void CameraView::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}
#define PI 3.1415926
void CameraView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(value3,test,value2,0,0,0,0,1,0);

    glRotatef(CameraX,0,0,1);

    glRotatef(CameraY,1,0,0);

    glRotatef(CameraZ,0,1,0);

    glPointSize(10.0f);





    glFlush();
}
#include "iostream"
using namespace std;
void CameraView::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    QString s = QString::number(w)+","+QString::number(h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho (-0.2, 0.2, -0.2, 0.2, -10000, 10000);
    cout<<s.toStdString()<<endl;
   glOrtho (-100, 100, -100, 100, -100, 10000);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_DITHER);
   glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   // gluLookAt(0,0,5,0,0,0,0,1,0);



    GLfloat ambientLight[]  = {0.2f,  0.2f,  0.2f,  1.0f};//环境光
    GLfloat diffuseLight[]  = {0.9f,  0.9f,  0.9f,  1.0f};//漫反射
    GLfloat specularLight[] = {1.0f,  1.0f,  1.0f,  1.0f};//镜面光
    GLfloat lightPos[]      = {50.0f, 80.0f, 60.0f, 1.0f};//光源位置

    glEnable(GL_LIGHTING);          //启用光照
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambientLight);    //设置环境光源
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseLight);    //设置漫反射光源
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);   //设置镜面光源
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);        //设置灯光位置
    glEnable(GL_LIGHT0);            //打开第一个灯光

    glEnable(GL_COLOR_MATERIAL);    //启用材质的颜色跟踪
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  //指定材料着色的面
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight); //指定材料对镜面光的反应
    glMateriali(GL_FRONT, GL_SHININESS, 100);           //指定反射系数



}
