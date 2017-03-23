#include "mainwindow.h"
#include "oglwidget.h"
#include "math.h"
#include "QDebug"
#include "iostream"
#include "QPoint"
#include "QTimer"
#include "GL/gl.h"
using namespace std;


HOM_MAT a;
#define PI 3.1415926
void HOMs_Product( HOM_MAT mat1,HOM_MAT mat2,HOM_MAT result){

    for(int row=0;row<4;row++){
        for(int col = 0;col<4;col++){
                for(int i=0;i<4;i++){
                    result[row*4+col]+=mat1[row*4+i]*mat2[i*4+col];
                }
        }
    }
}

void copyMat(HOM_MAT res,HOM_MAT dst){

    for(int row=0;row<4;row++){
        for(int col = 0;col<4;col++){
            dst[row*4+col] = res[row*4+col];
        }
    }

}

void VectHomOpera(HOM_Vect vect,HOM_MAT hom,HOM_Vect res){

    for(int i=0;i<4;i++){
        res[i]=0;
        for(int j=0;j<4;j++){
                res[i] += vect[j]*hom[j*4+i];
        }
    }
}

void HOM_MAT_Scale(HOM_MAT mat,float scale){
    for(int i=0;i<15;i++){
        mat[i]*=scale;
    }
}

void HOM_MAT_Invers(HOM_MAT mat){

    float tmp;

    //对旋转矩阵求逆

    for(int col=0;col<3;col++){
          for(int row = col;row<3;row++){
                if(row==0) continue;
                tmp = mat[col*4+row];
                mat[col*4+row] = mat[row*4+col];
                mat[row*4+col] = tmp;
          }
    }
    float tmpx[3];
    for(int i = 0;i<3;i++){
        for(int j=0;j<3;j++){
            tmpx[i]+= mat[j*4+i]*mat[12+j];
        }
    }
    for(int i=0;i<3;i++){
        mat[12+i] = -tmpx[i];
    }

    //qDebug("%f  %f  %f",tmpx[0],tmpx[1],tmpx[2]);

}

void HOM_Print(HOM_MAT mat){


    qDebug("---------------------");


    for(int i=0;i<4;i++)
    {

//          qDebug("%f   %f   %f   %f",mat[i*4+0],mat[i*4+1],mat[i*4+2],mat[i*4+3]);

    }

//        qDebug("---------------------");
}

float input[3];
float output[3];

void DrawArm(float length,float weight,float high){
    glLineWidth(1);
    glBegin(GL_LINES); //前表面
    glColor3f(1,1.0,1.0);     // 设置当前色为红色
    glVertex3f(length/2,high,weight/2);
    glVertex3f(length/2,0,weight/2);
    glVertex3f(-length/2,0,weight/2);
    glVertex3f(-length/2,high,weight/2);
    glEnd();
    glBegin(GL_LINES); //后表面
    glColor3f(1.0,1.0,1.0);     // 设置当前色为红色
    glVertex3f(length/2 ,   high,-weight/2);
    glVertex3f(length/2 ,  0,-weight/2);
    glVertex3f(-length/2,  0,-weight/2);
    glVertex3f(-length/2,   high,-weight/2);
    glEnd();
    glBegin(GL_LINES); //上表面
    glVertex3f(length/2,high,-weight/2);
    glVertex3f(length/2,high,weight/2);
    glVertex3f(-length/2,high,weight/2);
    glVertex3f(-length/2,high,-weight/2);
    glEnd();
    glBegin(GL_LINES); //下表面
    glVertex3f(length/2,0,-weight/2);
    glVertex3f(length/2,0,weight/2);
    glVertex3f(-length/2,0,weight/2);
    glVertex3f(-length/2,0,-weight/2);
    glEnd();
    glBegin(GL_LINES); //右表面
    glVertex3f(length/2,high,weight/2);
    glVertex3f(length/2,high,-weight/2);
    glVertex3f(length/2,0,-weight/2);
    glVertex3f(length/2,0,weight/2);
    glEnd();
    glBegin(GL_LINES); //右表面
    glVertex3f(-length/2,high,weight/2);
    glVertex3f(-length/2,high,-weight/2);
    glVertex3f(-length/2,0,-weight/2);
    glVertex3f(-length/2,0,weight/2);
    glEnd();
}


void DrawCoordinates(float length){
        glLineWidth(2);
        glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(length,0,0);
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,length,0);
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,length);
        glEnd();
}


void OGLWidget::mousePressEvent(QMouseEvent *event){

    MouseStart.setX(event->x());
    MouseStart.setY(event->y());

        if(event->buttons()& Qt::LeftButton){
        }

}

void OGLWidget::mouseReleaseEvent(QMouseEvent *event){

        nowAngle.setX(value3);
        nowAngle.setY(value2);

}




void OGLWidget::mouseMoveEvent ( QMouseEvent  * e )//鼠标移动事件响应
{
    //1.获取局部鼠标位置
   // e->accept();
    static int lastX,lastY;
    if(e->buttons()&Qt::LeftButton){
    value3 = nowAngle.x()+(e->x()-MouseStart.x())*0.5;
    value2= nowAngle.y()+(e->y()-MouseStart.y())*0.5;
         needRepaint =true;
    }
    if(e->buttons()&Qt::MiddleButton){

        Vx -= lastX-e->x();
        Vy +=  lastY-e->y();
         needRepaint =true;
    }
    repaint();
    lastX = e->x();
    lastY = e->y();}


void OGLWidget::wheelEvent(QWheelEvent *event){


    scale+=event->delta()*0.0005;
    repaint();
    cout<<scale<<endl;
    needRepaint =true;

}

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
        test = 0;
        QTimer *testTimer = new QTimer(this);

        connect(testTimer,SIGNAL(timeout()),this,SLOT(TimeBasedPaint()));
        Vx = 0;
        Vy = 0;
        Vz = 0;

        testTimer->start(1000/20);
}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::initializeGL()
{
    //glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);



}



//void OGLWidget::DrawStruct(HOM_MAT AfterTrans,para_Def *_para,HOM_MAT RefMat,bool flag){

//    HOM_MAT OriMat,_RefMat,solve,OriMat_Remove_View;



//    glPushMatrix();
//    //节点关节1
//    //DrawArm(20,20,821.5);

//    glBegin(GL_LINES);
//    glLineWidth(30);
//    glVertex3f(0,0,0);
//    glVertex3f(0,0,821.5);
//    glEnd();
//    glTranslatef(0,0,821.5);


//    DrawCoordinates(100);


//    glGetFloatv(GL_MODELVIEW_MATRIX, OriMat);

//    //HOM_Print(OriMat);
//    //HOM_Print(AfterTrans);

//    //HOMs_Product(OriMat,AfterTrans,OriMat_Remove_View);

//    HOM_MAT_Invers(OriMat);


////    if(flag)
////    emit update_mat_info(AfterTrans,0);


////    if(flag)
////    emit update_mat_info(OriMat_Remove_View,1);


//   //节点关节2
//    glRotated(_para->Main_Axis,0,0,1);
//   // DrawCoordinates(100);
//    glBegin(GL_LINES);
//    glLineWidth(10);
//    glVertex3f(0,0,0);
//    glVertex3f(_para->Horizontal_Axis,0,0);
//    glEnd();
//    glTranslated(_para->Horizontal_Axis,0,0);
//    glRotated(-90,0,0,1);
//    glColor3f(1,0,0);
//    glPointSize(10);
//    glBegin(GL_POINTS);
//    glVertex3f(100,100,100);
//    glEnd();
////    glGetFloatv(GL_MODELVIEW_MATRIX, _RefMat);
////    HOMs_Product(_RefMat,OriMat,RefMat);
////    if(flag)
////    emit update_mat_info(RefMat,2);
////    HOM_Print(RefMat);
//   // DrawCoordinates(30);
//   // DrawArm(20,20,670);
//    glLineWidth(10);
//    glBegin(GL_LINES);
//    glVertex3f(0,0,0);
//    glVertex3f(0,0,_para->Vertial_Axis);
//    glEnd();
//    glTranslated(0,0,_para->Vertial_Axis);
//   // HOM_MAT_Scale(solve,1/scale/scale);

//    DrawCoordinates(30);
//    //DrawArm(20,20,30);
//    glRotated(_para->END_EFFECTOR_YAW,0,0,1);

//    //DrawCoordinates(30);
//   // DrawArm(20,20,30);
//    glRotated(_para->END_EFFECTOR_Pitch,1,0,0);

//    glTranslated(0,0,-108.33);

//    DrawCoordinates(30);
//    //DrawArm(20,20,20);
//    glPopMatrix();
//}




void OGLWidget::paintGL()
{

    glMatrixMode(GL_MODELVIEW);
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //加载初始坐标系
    glTranslated(Vx,Vy,Vz); //先进行平移操作。
    glRotated(-90,1,0,0);
   // glRotated(90,0,0,1);
    glRotatef(value2,1,0,0);
    glRotatef(value3,0,0,1); //再以此以三个轴位中心做三次旋转
    glScalef(scale,scale,scale);
    qDebug("%f,%f,%f",Vx,Vy,Vz);
   // DrawCoordinates(1000);



    glPointSize(3);
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glVertex3f(p1[0],p1[1],p1[2]);
    glEnd();




    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1000,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,1000,0);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1000);
    glEnd();

    qDebug("Draw");

}
#include "iostream"
using namespace std;
void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glEnable(GL_COLOR_MATERIAL );
    glOrtho (-w, w, -h, h, -100000, 10000);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    scale=1;

}

void OGLWidget:: TimeBasedPaint(){

  //  value3+=10;
    if(needRepaint)
    {
        repaint();
        needRepaint = false;
    }
}
