#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMouseEvent"
#include "iostream"
#include "QTableWidgetItem"
#include "math.h"
#include "QTimer"
#include "dialog.h"
#include "clientclass.h"
#include "QTcpServer"
#include "qtcpsocket.h"
#include "globject.h"
#include "Threads/Threads.h"
#include "cmdsender.h"
#include "qsemaphore.h"
#include "calibrator.h"
using namespace std;
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
Dialog *w;

CmdSender csender;
QSemaphore cmdsemaphore(0);
QSerialPort *serial;
QTcpServer *server;


static uint8_t cmd_id = 0;

void MainWindow:: acceptConnection()
{
    QTcpSocket *_tmpSocket= server->nextPendingConnection();

    ClientClass *client = new ClientClass(_tmpSocket);


    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    connect(_tmpSocket, SIGNAL(readyRead()), client, SLOT(readClient()));
    connect(client,SIGNAL(drawPoint(float*,uint8_t)),this,SLOT(draw1Point(float*,uint8_t)));
    //connect(_tmpSocket,SIGNAL(disconnected()),client,SLOT(ClientDisConnected()));
    //connect(client,SIGNAL(updateText(QString)),this,SLOT(updateTextEdit(QString)));
    //connect(client,SIGNAL(updateUI(Car_Info*)),this,SLOT(carChange(Car_Info*)));

//    qDebug()<<_tmpSocket->peerAddress().toString();

}

void MainWindow::pushRgbd(unsigned char* imageData,int cols,int rows,int bytesPerLine)
{
//    cout<<"rgbd:"<<bytesPerLine<<endl;
    rgbd=QImage(imageData,cols,rows,bytesPerLine,QImage::Format_RGB32);
    ui->image1->clear();
    ui->image1->setPixmap(QPixmap::fromImage(rgbd));
//    ui->image1->resize(ui->image1->pixmap()->size());
}

void MainWindow::pushDepth(unsigned char* imageData,int cols,int rows,int bytesPerLine)
{
//    cout<<"depth:"<<bytesPerLine<<endl;
    depth=QImage(imageData,cols,rows,bytesPerLine,QImage::Format_RGB888);
    ui->image2->clear();
    ui->image2->setPixmap(QPixmap::fromImage(depth));
//    ui->image2->resize(ui->image2->pixmap()->size());
}

void MainWindow::pushContours(unsigned char* imageData,int cols,int rows,int bytesPerLine){

}

void MainWindow::pushProj(unsigned char* imageData,int cols,int rows,int bytesPerLine){


}

HOM_Vect ori = {0,0,0,1};
float  world[5] = {0,0,0,0,0};

float frameRotate;
float x_offset = 0, y_offset =660;
Position_Para MainWindow::FrameConvert(Position_Para *data){

    Position_Para _world;
    float ori[3];
    frameRotate = (90-ui->openGLWidget->Real_Para.Main_Axis)/180.f*M_PI;
    ori[0]=data->x+x_offset;
    ori[1]=data->y;
    ori[2]=data->z;
    _world.color = data->color;
  //  qDebug("x_Offset %f,Y_Offset %f",x_offset,y_offset);
//    qDebug("color ori%d",data->color);
    _world.x =  ori[0]*cos(frameRotate)+(ori[1]+y_offset)*sin(frameRotate);
    _world.y= -ori[0]*sin(frameRotate)+(ori[1]+y_offset)*cos(frameRotate);
    _world.z =  data->z;
    _world.vx = -(90-(data->vx)/M_PI*180-ui->openGLWidget->Real_Para.Main_Axis);
    ui->openGLWidget->p1[0]=_world.x;
    ui->openGLWidget->p1[1]=_world.y;
    ui->openGLWidget->p1[2]=_world.z;
    ui->openGLWidget->needRepaint = true;

    return _world;

}





void MainWindow::drawPoint(ReadData_Transform* data){

}


void MainWindow::GetVisionPoint(unsigned char id, float *point){

    Position_Para _visionPoint;
    _visionPoint.x = point[0];
    _visionPoint.y = point[1];
    _visionPoint.z = point[2];
    _visionPoint.vx = atan2(point[4],point[3]);
    _visionPoint.color = point[5];

    //qDebug("EMIT%f,%f,%f,%f",_visionPoint.x,_visionPoint.y,_visionPoint.z,)

    World_Pos = FrameConvert(&_visionPoint);
}



void MainWindow::setBackgroundColor(QWidget *widget,QColor color){

       QPalette _Palette = widget->palette();
       widget->setAutoFillBackground(true);
       _Palette.setColor(QPalette::Background,color);
       widget->setPalette(_Palette);
}




Calibrator * calibrator_window;
MyThread* myThread;
Communication* communicator;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //
    ui->setupUi(this);

    qRegisterMetaType<OGLWidget::para_Def>("OGLWidget::para_Def");
    //processer = new cvProcesser();

   // connect(&csender,SIGNAL(para_display(OGLWidget::para_Def)),this,SLOT(para_display(OGLWidget::para_Def)));
    //connect(&csender,SIGNAL(uart_send(OGLWidget::para_Def*)),this,SLOT(Uart_Send(OGLWidget::para_Def*)));
    //connect(&csender,SIGNAL(one_box_finish()),this,SLOT(One_Box_Finish()));
    w = new Dialog(this);
   // server = new QTcpServer();
   // server->listen(QHostAddress::Any, 6000);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
//    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        ui->comboBox->addItem(serial.portName());
        ui->startPort_Button->setEnabled(true);
    }

    //ui->openGLWidget->object.push_back(gLObject::DrawPoint(1,1,1));
    //w->setWindowFlags(Qt::Widget);
    //setMouseTracking(true);

    myThread= new MyThread();
//    communicator =new Communication();
    calibrator_window = new Calibrator(this);
    connect(calibrator_window,SIGNAL(setBackGround()),myThread,SLOT(setBackGround()));
    connect(myThread,SIGNAL(setProgressbarValue(int)),calibrator_window,SLOT(setProgressbarValue(int)));
    connect(calibrator_window,SIGNAL(calibrate()),myThread,SLOT(calibrate()));
    connect(calibrator_window,SIGNAL(SendPara(OGLWidget::para_Def*)),this,SLOT(Uart_Send(OGLWidget::para_Def*)));
    connect(calibrator_window,SIGNAL(readParam()),myThread,SLOT(readParam()));
    //connect(myThread,SIGNAL(sendPoint(unsigned char,float*)),communicator,SLOT(sendPoint(unsigned char,float*)));

     connect(myThread,SIGNAL(sendPoint(unsigned char,float*)),this,SLOT(GetVisionPoint(unsigned char,float*)));
     cout<<"test"<<endl;

    //connect(myThread,SIGNAL(drawPoints(float*,size_t)),this,SLOT(draw1Point(float*,size_t)));
   // connect(myThread,SIGNAL(drawPoints(float*,int)),this,SLOT(draw1Point(float*,int)));
   // connect(this,SIGNAL(setBackGround()),myThread,SLOT(setBackGround()));
    connect(myThread,SIGNAL(pushRgbd(unsigned char*,int,int,int)),this,SLOT(pushRgbd(unsigned char*,int,int,int)));
    connect(myThread,SIGNAL(pushDepth(unsigned char*,int,int,int)),this,SLOT(pushDepth(unsigned char*,int,int,int)));
    connect(myThread,SIGNAL(pushContours(unsigned char*,int,int,int)),this,SLOT(pushContours(unsigned char*,int,int,int)));
    connect(myThread,SIGNAL(pushProj(unsigned char*,int,int,int)),this,SLOT(pushProj(unsigned char*,int,int,int)));
    connect(w,SIGNAL(SendPara(OGLWidget::para_Def*)),this,SLOT(Uart_Send(OGLWidget::para_Def*)));
    myThread->start();
//    communicator->start();
}

USB_Trans_TypeDef USB_trans;
void MainWindow::Uart_Send(OGLWidget::para_Def *_para){



    qDebug("Uart Trans Main %f,Horizontal %f,Vertial%f",_para->Main_Axis,_para->Horizontal_Axis,_para->Vertial_Axis);

    MAX_LIMMIT(_para->END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(_para->END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(_para->END_EFFECTOR_YAW,180);
    MIN_LIMMIT(_para->END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(_para->Horizontal_Axis,713);
    MIN_LIMMIT(_para->Horizontal_Axis,300);
    MAX_LIMMIT(_para->Vertial_Axis,0);
    MIN_LIMMIT(_para->Vertial_Axis,-430);

    USB_trans.head1 = 0xa5;
    USB_trans.head2 = 0x5a;
    USB_trans.END_EFFECTOR_Pitch = (float)_para->END_EFFECTOR_Pitch;
    USB_trans.END_EFFECTOR_YAW = (float)_para->END_EFFECTOR_YAW;
    USB_trans.Main_Axis_Rotate =(float) _para->Main_Axis;
    USB_trans.Horizontal_Axis = (float)_para->Horizontal_Axis;
    USB_trans.Vertial_Axis = (float)_para->Vertial_Axis;
    USB_trans.status.Horizontal_Axis = 1;
    USB_trans.status.Vertial_Axis = 1;
    USB_trans.status.Main_Axis_Rotate = 1;
    USB_trans.PUMB = _para->pumb;
    USB_trans.VALVE = _para->valve;
    USB_trans.CMD_ID = cmd_id++;
    qDebug("Send CMD ID%d",USB_trans.CMD_ID);
    serial->write((const char *)&USB_trans,sizeof(USB_Trans_TypeDef));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::repaint(){

    qDebug("FUcckkkkkkk");

}





OGLWidget::para_Def MainWindow::caculateInvers(float x,float y,float z,float rotate){

   OGLWidget::para_Def _para;

   qDebug("World Position %f,%f,%f,%f",x,y,z,rotate);

   _para.Main_Axis =atan2(y,x)/3.1415926*180.f;

   if(_para.Main_Axis>180)
       _para.Main_Axis = _para.Main_Axis-360;
  // if(_para.Main_Axis<0)
   _para.END_EFFECTOR_YAW =((rotate)-_para.Main_Axis);
//   else
//   _para.END_EFFECTOR_YAW = ((rotate-_para.Main_Axis));

   if(_para.END_EFFECTOR_YAW<0)
      _para.END_EFFECTOR_YAW+=180;
   else if(_para.END_EFFECTOR_YAW>180)
       _para.END_EFFECTOR_YAW-=180;

   qDebug("Caculate Main_Axis %f",_para.Main_Axis);
   //ui->doubleSpinBox_4->setValue(ui->openGLWidget->Aim_Para.END_EFFECTOR_YAW);
   _para.Horizontal_Axis  = sqrt(x*x + y*y);
   _para.Vertial_Axis  =  z-450;
   ui->openGLWidget->needRepaint = true;
   qDebug("x:%f,y:%f,Z:%f,Angle:%f",x,y,z,_para.END_EFFECTOR_YAW);
   qDebug("Calculate Horizontal_Axis:%f,Vertial_Axis:%f,Main_Axis:%f",ui->openGLWidget->Aim_Para.Horizontal_Axis,ui->openGLWidget->Aim_Para.Vertial_Axis,ui->openGLWidget->Aim_Para.Main_Axis);
   qDebug("Yaw:%f",_para.END_EFFECTOR_YAW);
   return _para;
}


//void MainWindow::on_Move_X_valueChanged(int value)
//{
//    caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());

//}

//void MainWindow::on_Move_Y_valueChanged(int value)
//{
//    caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());

//}

//void MainWindow::on_Move_z_valueChanged(int value)
//{
//    caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());

//}

typedef struct{

    uint8_t Blue;
    uint8_t Yellow;
    uint8_t Green;

}BOX_Count_Def;
BOX_Count_Def box_count;




void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{

  OGLWidget::para_Def testdef;
    testdef = caculateInvers(World_Pos.x,World_Pos.y,World_Pos.z,World_Pos.vx);
    testdef.Vertial_Axis+=10;
    Uart_Send(&testdef);

}

//static OGLWidget::para_Def MainWindow::GetRealPara(){

//    return ui->openGLWidget->Real_Para;

//}

void MainWindow::on_startPort_Button_clicked()
{
    if(ui->startPort_Button->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->comboBox->currentText());
        //打开串口
        if(serial->open(QIODevice::ReadWrite)==false)
            return ;
        //设置波特率
        serial->setBaudRate(115200);
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);
        //设置奇偶校验
        serial->setParity(QSerialPort::NoParity);
        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //关闭设置菜单使能
        ui->startPort_Button->setText(tr("关闭串口"));
        // ui->sendButton->setEnabled(true);
        // //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
        ui->openGLWidget->serial = serial;
        ui->openGLWidget->serialReady = true;
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->comboBox->setEnabled(true);
        ui->startPort_Button->setText(tr("打开串口"));
        ui->openGLWidget->serialReady = false;

    }

}

USB_Trans_TypeDef ReceiveBuffer2;
void MainWindow::Read_Data(){

    static unsigned int HeadErrCount=0,SumErrCount=0;
       char *data = (char*)&ReceiveBuffer2;
       serial->read(data,sizeof(USB_Trans_TypeDef));
       if(((unsigned char*)data)[0]!=0xa5||((unsigned char*)data)[1]!=0x5a){
          // qDebug("Head Error");
            HeadErrCount++;
      //      ui->Box_Info_2->setText(QString().sprintf("Head Err:%d,SumErr:%d",HeadErrCount,SumErrCount));
           return ;
       }
        uint8_t sum = 0;
        sum = 0;
       for(int i=1;i<(sizeof(USB_Trans_TypeDef)-1);i++){
        sum+= ((unsigned char*)data)[i];
       }
       if(sum!=ReceiveBuffer2.sum){

          // qDebug("SUM Error %d",sum);
           SumErrCount++;
           //ui->Box_Info_2->setText(QString().sprintf("Head Err:%d,SumErr:%d",HeadErrCount,SumErrCount));
           return ;

       }

       ui->openGLWidget->Real_Para.Horizontal_Axis = ReceiveBuffer2.Horizontal_Axis;
       ui->openGLWidget->needRepaint =true;
       ui->Horizontal_Axis->display(ui->openGLWidget->Real_Para.Horizontal_Axis);
       ui->openGLWidget->Real_Para.Vertial_Axis = ReceiveBuffer2.Vertial_Axis;
       ui->Vertical_Axis_Display->display(ui->openGLWidget->Real_Para.Vertial_Axis);
       ui->openGLWidget->Real_Para.END_EFFECTOR_Pitch = ReceiveBuffer2.END_EFFECTOR_Pitch;
       ui->END_EFFECTOR_Pitch->display(ui->openGLWidget->Real_Para.END_EFFECTOR_Pitch);
       ui->openGLWidget->Real_Para.END_EFFECTOR_YAW = ReceiveBuffer2.END_EFFECTOR_YAW;
       ui->END_EFFECTOR_YAW->display( ui->openGLWidget->Real_Para.END_EFFECTOR_YAW);
       ui->openGLWidget->Real_Para.Main_Axis = ReceiveBuffer2.Main_Axis_Rotate;
       ui->Main_Axis->display( ui->openGLWidget->Real_Para.Main_Axis);
       (ReceiveBuffer2.status.Main_Axis_Rotate)? setBackgroundColor(ui->Main_Axis,QColor(255,0,0,255)): setBackgroundColor(ui->Main_Axis,QColor(0,255,0,255));
       (ReceiveBuffer2.status.Horizontal_Axis)? setBackgroundColor(ui->Horizontal_Axis,QColor(255,0,0,255)): setBackgroundColor(ui->Horizontal_Axis,QColor(0,255,0,255));
       (ReceiveBuffer2.status.Vertial_Axis)? setBackgroundColor(ui->Vertical_Axis_Display,QColor(255,0,0,255)): setBackgroundColor(ui->Vertical_Axis_Display,QColor(0,255,0,255));
       (ReceiveBuffer2.status.END_EFFECTOR_YAW)? setBackgroundColor(ui->END_EFFECTOR_YAW,QColor(255,0,0,255)): setBackgroundColor(ui->END_EFFECTOR_YAW,QColor(0,255,0,255));
       (ReceiveBuffer2.status.END_EFFECTOR_Pitch)? setBackgroundColor(ui->END_EFFECTOR_Pitch,QColor(255,0,0,255)): setBackgroundColor(ui->END_EFFECTOR_Pitch,QColor(0,255,0,255));
       uint8_t check=0;
        for(int i=0;i<5;i++){
           check+=((uint8_t *)(&ReceiveBuffer2.status))[i];
        }
        if(check==0){

            if(cmdsemaphore.available()==0&&((cmd_id-1)==ReceiveBuffer2.CMD_ID)&&csender.StartWaitForFinish)
            {

                if(csender.mutex.tryLock()){
                csender.StartWaitForFinish = false;
                qDebug("Trans CMD_ID %d Receive CMD_ID %d",cmd_id-1,ReceiveBuffer2.CMD_ID);
                qDebug("sendsemaphore count %d",cmdsemaphore.available());
                cmdsemaphore.release();
                csender.mutex.unlock();
                }
            }
        }
}


void MainWindow::GeneCmd(){

    OGLWidget::para_Def oricmd,sendCmd;

    memset(&sendCmd,0,sizeof(OGLWidget::para_Def));

    if(World_Pos.color==0)
      {
        NoBoxCount++;
        qDebug("No Box count%d",NoBoxCount);
        if(NoBoxCount==2){
            qDebug("Next Angle");
            NoBoxCount=0;
            SerchAngle+=30;
            sendCmd.Horizontal_Axis = 713;
            sendCmd.Vertial_Axis = 0;
            sendCmd.Main_Axis = SerchAngle;
            csender.pushCmd(sendCmd);

        }

        return ;}

    oricmd =  caculateInvers(World_Pos.x,World_Pos.y,World_Pos.z,World_Pos.vx);
    //先设置所有值都为0
    //开启气泵 关闭电磁阀，首先旋转主轴，水平轴 并且把垂直轴下降到目标高度的0.8倍位置
    sendCmd.pumb = 1;
    sendCmd.valve = 0;
    sendCmd.Main_Axis = oricmd.Main_Axis;
    sendCmd.Vertial_Axis = oricmd.Vertial_Axis*0.8;
    sendCmd.Horizontal_Axis = oricmd.Horizontal_Axis;
    sendCmd.END_EFFECTOR_Pitch = 0;
    sendCmd.END_EFFECTOR_YAW = oricmd.END_EFFECTOR_YAW;
    sendCmd.DelayTime = 1;
    csender.pushCmd(sendCmd);

    //所有位置就绪后，移动垂直轴到箱子表面
    sendCmd.pumb = 0;
    sendCmd.Vertial_Axis = oricmd.Vertial_Axis;
    sendCmd.DelayTime = 0.5;
    csender.pushCmd(sendCmd);
    //箱子提起
   // sendCmd.Main_Axis  = 0;
   // sendCmd.Horizontal_Axis = 713;
    sendCmd.Vertial_Axis = -50;
    sendCmd.DelayTime = 0.5;
    csender.pushCmd(sendCmd);
    //判断颜色决定放在哪个盒子里
    int color = World_Pos.color;
   // qDebug("Color %d",color);
    switch(color){
        case 1:
        switch(box_count.Green%4){
        case 0:  oricmd = caculateInvers(150,-380,190+10*(box_count.Green%4),90); break;
        case 1:  oricmd = caculateInvers(250,-380,190+10*(box_count.Green%4),90);break;
        case 2:  oricmd = caculateInvers(150,-580,190+10*(box_count.Green%4),90);break;
        case 3:  oricmd = caculateInvers(250,-580,190+10*(box_count.Green%4),90);break;
        }
        box_count.Green++;
        break;
        case 2:
        box_count.Blue++;
        oricmd = caculateInvers(0,-(370+200*(box_count.Blue%2)),280,90);break;
        case 3  :
        box_count.Yellow++;
        oricmd = caculateInvers(-200,-485,310,90);break;
        default: oricmd = caculateInvers(455,0,0,0);break;
    }

    //ui->Box_Info->setText(QString().sprintf("Blue:%d,Green:%d,Yellow:%d",box_count.Blue,box_count.Green,box_count.Yellow));

    sendCmd.Main_Axis = oricmd.Main_Axis;
    sendCmd.Horizontal_Axis = oricmd.Horizontal_Axis;
    sendCmd.END_EFFECTOR_YAW = oricmd.END_EFFECTOR_YAW;
    sendCmd.Vertial_Axis = -20;
    sendCmd.DelayTime = 0.8;
    csender.pushCmd(sendCmd);

    //稍稍深入盒子中一点点 准备放下箱子

    sendCmd.Main_Axis = oricmd.Main_Axis;
    sendCmd.Horizontal_Axis = oricmd.Horizontal_Axis;
    sendCmd.END_EFFECTOR_YAW = oricmd.END_EFFECTOR_YAW;
    sendCmd.Vertial_Axis =oricmd.Vertial_Axis;
    sendCmd.DelayTime = 0;
    csender.pushCmd(sendCmd);


    //打开电磁阀关闭气泵，升起垂直轴，扔下箱子
    sendCmd.pumb = 1;
    sendCmd.valve = 1;
    sendCmd.DelayTime = 1;
    //sendCmd.Vertial_Axis = -20;
    csender.pushCmd(sendCmd);


    sendCmd.pumb = 1;
    sendCmd.valve = 1;
    sendCmd.DelayTime = 0;
    sendCmd.Vertial_Axis = -20;
    csender.pushCmd(sendCmd);

    //打开气泵，关闭电磁阀，回到原来位置，准备抓取下一个箱子

    sendCmd.DelayTime = 0;
    sendCmd.Main_Axis = SerchAngle;
    sendCmd.Horizontal_Axis = 713;
    csender.pushCmd(sendCmd);


}


void MainWindow::para_display(OGLWidget::para_Def _para){

    ui->Main_Axis->display(_para.Main_Axis);
    ui->Horizontal_Axis->display(_para.Horizontal_Axis);
    ui->Vertical_Axis_Display->display(_para.Vertial_Axis);
    ui->END_EFFECTOR_YAW->display(_para.END_EFFECTOR_YAW);
    ui->END_EFFECTOR_Pitch->display(_para.END_EFFECTOR_Pitch);

}



void MainWindow::One_Box_Finish(){

    on_pushButton_2_clicked();
    qDebug("Calculate Finish Prepare To move Be careful");

    if(SerchAngle<=180)
    {
        csender.start();
    }
    else {
     //Beep(1000,1000);
    }
}


void MainWindow::on_calibrate_button_clicked()
{


calibrator_window->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    w->show();
}
