#include "oglwidget.h"
#include "math.h"
#include "QDebug"
#include "iostream"
#include "QPoint"
#include "QTimer"
using namespace std;


typedef float HOM_MAT[16];

HOM_MAT a;

void HOMs_Product( HOM_MAT (mat1),HOM_MAT(mat2),HOM_MAT (result)){

    for(int row=0;row<4;row++){
        for(int col = 0;col<4;col++){
                for(int i=0;i<4;i++){
                    result[row*4+col]+=mat1[row*4+i]*mat2[i*4+row];
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

void HOM_MAT_Trans(HOM_MAT mat){

    float tmp;

    for(int col=0;col<4;col++){

          for(int row = col;row<4;row++){
                if(row==0) continue;

                tmp = mat[col*4+row];
                mat[col*4+row] = mat[row*4+col];
                mat[row*4+col] = tmp;


          }

    }



}

void HOM_Print(HOM_MAT mat){


    qDebug("---------------------");


    for(int i=0;i<4;i++)
    {

          qDebug("%f %f %f %f",mat[i*4+0],mat[i*4+1],mat[i*4+2],mat[i*4+3]);

    }

        qDebug("---------------------");
}




void DrawSquare(void){
    glBegin(GL_POLYGON); //前表面
    glVertex3f(50.0f,50.0f,50.0f);
    glVertex3f(50.0f,-50.0f,50.0f);
    glVertex3f(-50.0f,-50.0f,50.0f);
    glVertex3f(-50.0f,50.0f,50.0f);
    glEnd();

    glBegin(GL_POLYGON); //后表面
    glVertex3f(50.0f,50.0f,-50.0f);
    glVertex3f(50.0f,-50.0f,-50.0f);
    glVertex3f(-50.0f,-50.0f,-50.0f);
    glVertex3f(-50.0f,50.0f,-50.0f);
    glEnd();


    glBegin(GL_POLYGON); //上表面
    glVertex3f(50.0f,50.0f,-50.0f);
    glVertex3f(50.0f,50.0f,50.0f);
    glVertex3f(-50.0f,50.0f,50.0f);
    glVertex3f(-50.0f,50.0f,-50.0f);
    glEnd();

    glBegin(GL_POLYGON); //下表面
    glVertex3f(50.0f,-50.0f,-50.0f);
    glVertex3f(50.0f,-50.0f,50.0f);
    glVertex3f(-50.0f,-50.0f,50.0f);
    glVertex3f(-50.0f,-50.0f,-50.0f);
    glEnd();

    glBegin(GL_POLYGON); //左表面
    glVertex3f(50.0f,50.0f,50.0f);
    glVertex3f(50.0f,50.0f,-50.0f);
    glVertex3f(50.0f,-50.0f,-50.0f);
    glVertex3f(50.0f,-50.0f,50.0f);
    glEnd();

    glBegin(GL_POLYGON); //右表面
    glVertex3f(-50.0f,50.0f,50.0f);
    glVertex3f(-50.0f,50.0f,-50.0f);
    glVertex3f(-50.0f,-50.0f,-50.0f);
    glVertex3f(-50.0f,-50.0f,50.0f);
    glEnd();
}
#define PI 3.1415926

float TransMat[3][3]={{1,2,3},
                      {4,5,6},
                      {0.5774,0.5774,0.5774}};
void TransVect(float x,float y,float z,float Trans[3][3],float out[3]){

    out[0] =x*Trans[0][0]+y*Trans[1][0]+z*Trans[2][0];
    out[1] =x*Trans[0][1]+y*Trans[1][1]+z*Trans[2][1];
    out[2] =x*Trans[0][2]+y*Trans[1][2]+z*Trans[2][2];


}
float input[3];
float output[3];
void DrawS(float length,float weight,float high,float Gx,float Gy,float Gz,float x,float y,float z){

     float AngleX,AngleY,AngleZ; //根据方向向量计算分别许要绕三个方向旋转的角度。

     glPushMatrix();

//     AngleZ =atan(Gy/Gx)/PI*180;
//     AngleY =atan(Gx/Gz)/PI*180;
//     AngleX =atan(Gz/Gy)/PI*180;
//    cout<<"Angle around X:"<<AngleX<<endl;
//    cout<<"Angle around Y:"<<AngleY<<endl;
//    cout<<"Angle around Z:"<<AngleZ<<endl;


    glRotatef(x,1,0,0);
    glRotatef(y,0,1,0);
    glRotatef(z,0,0,1);





    glBegin(GL_LINES);

    glVertex3f(0,0,0);



    glVertex3f(0,0,1000);

    glVertex3f(0,0,0);
    glVertex3f(0,1000,0);

    glVertex3f(0,0,0);

    glVertex3f(1000,0,0);

    glEnd();

    glBegin(GL_POLYGON); //前表面
    glVertex3f(length/2,high/2,weight/2);
    glVertex3f(length/2,-high/2,weight/2);
    glVertex3f(-length/2,-high/2,weight/2);
    glVertex3f(-length/2,high/2,weight/2);
    glEnd();

    glBegin(GL_POLYGON); //后表面
    glVertex3f(length/2 ,   high/2,-weight/2);
    glVertex3f(length/2 ,  -high/2,-weight/2);
    glVertex3f(-length/2,  -high/2,-weight/2);
    glVertex3f(-length/2,   high/2,-weight/2);
    glEnd();

    glBegin(GL_POLYGON); //上表面
    glVertex3f(length/2,high/2,-weight/2);
    glVertex3f(length/2,high/2,weight/2);
    glVertex3f(-length/2,high/2,weight/2);
    glVertex3f(-length/2,high/2,-weight/2);
    glEnd();

    glBegin(GL_POLYGON); //下表面
    glVertex3f(length/2,-high/2,-weight/2);
    glVertex3f(length/2,-high/2,weight/2);
    glVertex3f(-length/2,-high/2,weight/2);
    glVertex3f(-length/2,-high/2,-weight/2);
    glEnd();

    glBegin(GL_POLYGON); //右表面
    glVertex3f(length/2,high/2,weight/2);
    glVertex3f(length/2,high/2,-weight/2);
    glVertex3f(length/2,-high/2,-weight/2);
    glVertex3f(length/2,-high/2,weight/2);
    glEnd();

    glBegin(GL_POLYGON); //右表面
    glVertex3f(-length/2,high/2,weight/2);
    glVertex3f(-length/2,high/2,-weight/2);
    glVertex3f(-length/2,-high/2,-weight/2);
    glVertex3f(-length/2,-high/2,weight/2);
    glEnd();

    glPopMatrix();

}


void DrawArm(float length,float weight,float high){

    glBegin(GL_POLYGON); //前表面
    glVertex3f(length/2,high,weight/2);
    glVertex3f(length/2,0,weight/2);
    glVertex3f(-length/2,0,weight/2);
    glVertex3f(-length/2,high,weight/2);
    glEnd();

    glBegin(GL_POLYGON); //后表面
    glVertex3f(length/2 ,   high,-weight/2);
    glVertex3f(length/2 ,  0,-weight/2);
    glVertex3f(-length/2,  0,-weight/2);
    glVertex3f(-length/2,   high,-weight/2);
    glEnd();

    glBegin(GL_POLYGON); //上表面
    glVertex3f(length/2,high,-weight/2);
    glVertex3f(length/2,high,weight/2);
    glVertex3f(-length/2,high,weight/2);
    glVertex3f(-length/2,high,-weight/2);
    glEnd();

    glBegin(GL_POLYGON); //下表面
    glVertex3f(length/2,0,-weight/2);
    glVertex3f(length/2,0,weight/2);
    glVertex3f(-length/2,0,weight/2);
    glVertex3f(-length/2,0,-weight/2);
    glEnd();

    glBegin(GL_POLYGON); //右表面
    glVertex3f(length/2,high,weight/2);
    glVertex3f(length/2,high,-weight/2);
    glVertex3f(length/2,0,-weight/2);
    glVertex3f(length/2,0,weight/2);
    glEnd();

    glBegin(GL_POLYGON); //右表面
    glVertex3f(-length/2,high,weight/2);
    glVertex3f(-length/2,high,-weight/2);
    glVertex3f(-length/2,0,-weight/2);
    glVertex3f(-length/2,0,weight/2);
    glEnd();


}


void DrawCoordinates(){


        glLineWidth(2);

        glBegin(GL_LINES);



        glVertex3f(0,0,0);

        glVertex3f(0,0,1000);

        glVertex3f(0,0,0);
        glVertex3f(0,1000,0);
        glVertex3f(0,0,0);
        glVertex3f(1000,0,0);
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
        //this->repaint();
    }
    if(e->buttons()&Qt::MiddleButton){

        Vx -= lastX-e->x();
        Vy +=  lastY-e->y();
        this->repaint();
    }

    lastX = e->x();
    lastY = e->y();}


void OGLWidget::wheelEvent(QWheelEvent *event){


    scale+=event->delta()*0.0005;

    cout<<scale<<endl;
   // repaint();

}

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
        test = 0;
        QTimer *testTimer = new QTimer(this);

        connect(testTimer,SIGNAL(timeout()),this,SLOT(TimeBasedPaint()));

        testTimer->start(1000/20);

}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
   // glEnable(GL_LIGHT0);
  //  glEnable(GL_LIGHTING);
   // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   // glEnable(GL_COLOR_MATERIAL);
}



void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //加载初始坐标系
    glTranslated(Vx,Vy,Vz); //先进行平移操作。
    glRotatef(value2,1,0,0);
    glRotatef(value3,0,1,0); //再以此以三个轴位中心做三次旋转
    glScalef(scale,scale,scale);

    float mat[16];// get the modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);



    glPushMatrix();


    //节点关节1
    DrawArm(20,20,300);
    glTranslated(0,320,0);

    //节点关节2
    glRotated(joint1.rotate_x,1,0,0);
    glRotated(joint1.rotate_y,0,1,0);
    glRotated(joint1.rotate_z,0,0,1);

    glRotated(90,0,0,1);

    DrawArm(20,20,300);

    glTranslated(joint2.trans_x,joint2.trans_y,0);

    glRotated(90,0,0,1);

    DrawArm(20,20,300);

    glTranslated(0,320,0);

    glRotated(90,0,0,1);

    DrawArm(20,20,30);

    glGetFloatv(GL_MODELVIEW_MATRIX, mat);


    //HOM_Print(mat);
     HOM_MAT tmp,res;

    emit update_mat_info(mat,0);

    HOM_MAT_Trans(mat);

    copyMat(mat,tmp);

    HOM_MAT_Trans(mat);

    HOMs_Product(mat,tmp,res);

    emit update_mat_info(res,1);

    glPopMatrix();

    DrawCoordinates();



    glFlush();
}
#include "iostream"
using namespace std;
void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    QString s = QString::number(w)+","+QString::number(h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glEnable(GL_LIGHTING);          //启用光照
    glEnable(GL_LIGHT0);            //打开第一个灯光

    glOrtho (-w/2, w/2, -h/2, h/2, -100000, 10000);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    scale=0.5;

}

void OGLWidget:: TimeBasedPaint(){

   // value3+=10;

     repaint();

}
