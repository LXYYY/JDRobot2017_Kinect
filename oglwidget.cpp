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

    lastX = e->x();
    lastY = e->y();}


void OGLWidget::wheelEvent(QWheelEvent *event){


    scale+=event->delta()*0.0005;

    cout<<scale<<endl;
    needRepaint =true;

}

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
        test = 0;
        QTimer *testTimer = new QTimer(this);

        connect(testTimer,SIGNAL(timeout()),this,SLOT(TimeBasedPaint()));

        testTimer->start(1000/50);
        serialReady = false;
}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::initializeGL()
{
    //glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);



}



void OGLWidget::paintGL()
{

    glMatrixMode(GL_MODELVIEW);
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //加载初始坐标系
    glTranslated(Vx,Vy,Vz); //先进行平移操作。
    glRotatef(value2,1,0,0);
    glRotatef(value3,0,1,0); //再以此以三个轴位中心做三次旋转
    glScalef(scale,scale,scale);

    //glLoadIdentity();

    float mat_aftercodtrans[16];// get the modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX, mat_aftercodtrans);

    emit update_mat_info(mat_aftercodtrans,0);

//    qDebug("mat_aftercod");

    //HOM_Print(mat_aftercodtrans);

    glPushMatrix();
    //节点关节1
    DrawArm(20,20,821.5);
    glTranslatef(0,821.5,0);
    DrawCoordinates(100);

    //节点关节2
    glRotated(joint1.rotate_y,0,1,0);

    DrawCoordinates(100);
    glPushMatrix();
    glRotated(90,0,0,1);
    DrawArm(20,20,670);
    glPopMatrix();
    glTranslated(-joint2.trans_x,0,0);

    DrawCoordinates(30);
    DrawArm(20,20,670);
    glTranslated(0,-joint2.trans_y,0);

    DrawCoordinates(30);
    DrawArm(20,20,30);
    glRotated(joint3.rotate_y,0,1,0);

    DrawCoordinates(30);
   // DrawArm(20,20,30);
    glRotated(joint4.rotate_z,0,0,1);

    glTranslated(0,-108.33,0);

    DrawCoordinates(30);
    DrawArm(20,20,20);
    HOM_MAT res={},mat={};






   // copyMat(mat_aftercodtrans,mat);

    HOM_MAT_Invers(mat_aftercodtrans);

  //  emit update_mat_info(mat,0);

    //emit update_mat_info(mat_aftercodtrans ,1);

    glGetFloatv(GL_MODELVIEW_MATRIX, mat);


    //HOM_MAT_Scale(mat,1/scale);
    emit update_mat_info(mat ,1);

    //qDebug("invers");



    //HOM_MAT_Scale(mat_aftercodtrans,1/scale);



    HOMs_Product(mat,mat_aftercodtrans,res);

    HOM_MAT_Scale(res,1/scale/scale);

    emit update_mat_info(res ,2);
      HOM_Print(res);

    HOM_Vect Point_O={0,10,0,1},Point_END_EFFECTS;

    //VectHomOpera(Point_O,res,Point_END_EFFECTS);

   // emit update_mat_info(Point_END_EFFECTS,3);

   // qDebug("aa %f  %f   %f",Point_END_EFFECTS[0],Point_END_EFFECTS[1],Point_END_EFFECTS[2]);

    glPopMatrix();

    DrawCoordinates(1000);

//    glPointSize(5);

    for(int i=0;i<points.size();i++){

        glPointSize(1);
        glBegin(GL_POINTS);
        glColor3f(1,1,1);
        glVertex3f(points.at(i)[0],points.at(i)[2],points.at(i)[1]);
//        glColor3f(0,1,0);
//        glVertex3f(points.at(i)[0],0,points.at(i)[1]);
        glEnd();
        }

    glColor3f(1,1,1);
    glLineWidth(5);
    glBegin(GL_LINES);

//    HOM_Vect o={0,0,0,1};
//    HOM_Vect x={100,0,0,1};
//    HOM_Vect y={0,100,0,1};
//    HOM_Vect z={0,0,100,1};
//    HOM_Vect axis_after={0,0,0,1};

    glColor3f(1,0,0);

    //VectHomOpera(o,res,axis_after);
    //glVertex3f(axis_after[0],axis_after[1],axis_after[2]);

    glVertex3f(0,0,0);
    glVertex3f(res[0]*100,res[1]*100,res[2]*100);

//    VectHomOpera(x,res,axis_after);
//    glVertex3f(axis_after[0],axis_after[1],axis_after[2]);


    glColor3f(0,1,0);
//    VectHomOpera(o,res,axis_after);
//    glVertex3f(axis_after[0],axis_after[1],axis_after[2]);


//    VectHomOpera(y,res,axis_after);
//    glVertex3f(axis_after[0],axis_after[1],axis_after[2]);


    glVertex3f(0,0,0);
    glVertex3f(res[0+4]*100,res[1+4]*100,res[2+4]*100);


    glColor3f(0,0,1);

//    VectHomOpera(o,res,axis_after);
//    glVertex3f(axis_after[0],axis_after[1],axis_after[2]);

//    VectHomOpera(z,res,axis_after);
//    glVertex3f(axis_after[0],axis_after[1],axis_after[2]);


    glVertex3f(0,0,0);
    glVertex3f(res[0+8]*100,res[1+8]*100,res[2+8]*100);

    glEnd();




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

   // value3+=10;
    if(needRepaint)
    {
        repaint();
        needRepaint = false;
    }


}
