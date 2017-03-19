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
void MainWindow::drawPoint(ReadData_Transform* data){
        frameRotate = (90-ui->openGLWidget->Real_Para.Main_Axis)/180.f*M_PI;
        ori[0]=data->x;
        ori[1]=data->y;
        ori[2]=data->z;
        world[4] = data->color;
        world[0] =  ori[0]*cos(frameRotate)+(ori[1]+660)*sin(frameRotate);
        world[1] = -ori[0]*sin(frameRotate)+(ori[1]+660)*cos(frameRotate);
        world[2] =  data->z;
        world[3] = -(90-(data->vx)/M_PI*180-ui->openGLWidget->Real_Para.Main_Axis);
        ui->openGLWidget->p1[0]=world[0];
        ui->openGLWidget->p1[1]=world[1];
        ui->openGLWidget->p1[2]=world[2];
        ui->openGLWidget->needRepaint = true;
}



void MainWindow::setBackgroundColor(QWidget *widget,QColor color){

       QPalette _Palette = widget->palette();
       widget->setAutoFillBackground(true);
       _Palette.setColor(QPalette::Background,color);
       widget->setPalette(_Palette);
}





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
    server = new QTcpServer();
    server->listen(QHostAddress::Any, 6000);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

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
    communicator =new Communication();
    connect(myThread,SIGNAL(sendPoint(unsigned char,float*)),communicator,SLOT(sendPoint(unsigned char,float*)));
    //connect(myThread,SIGNAL(drawPoints(float*,size_t)),this,SLOT(draw1Point(float*,size_t)));
    connect(myThread,SIGNAL(drawPoints(float*,int)),this,SLOT(draw1Point(float*,int)));
    connect(this,SIGNAL(setBackGround()),myThread,SLOT(setBackGround()));
    connect(myThread,SIGNAL(pushRgbd(unsigned char*,int,int,int)),this,SLOT(pushRgbd(unsigned char*,int,int,int)));
    connect(myThread,SIGNAL(pushDepth(unsigned char*,int,int,int)),this,SLOT(pushDepth(unsigned char*,int,int,int)));
    connect(myThread,SIGNAL(pushContours(unsigned char*,int,int,int)),this,SLOT(pushContours(unsigned char*,int,int,int)));
    connect(myThread,SIGNAL(pushProj(unsigned char*,int,int,int)),this,SLOT(pushProj(unsigned char*,int,int,int)));
    connect(this,SIGNAL(changeMode()),myThread,SLOT(changeMode()));
    connect(this,SIGNAL(setOrigin()),myThread,SLOT(setOrigin()));
    connect(this,SIGNAL(shutDownKinect()),myThread,SLOT(shutDownKinect()));
    connect(this,SIGNAL(readParam()),myThread,SLOT(readParam()));
    connect(this,SIGNAL(reconnect()),communicator,SLOT(reconnect()));
    myThread->start();
    communicator->start();
}

USB_Trans_TypeDef USB_trans;
void MainWindow::Uart_Send(OGLWidget::para_Def *_para){



    qDebug("Uart Trans Main %f,Horizontal %f,Vertial%f",_para->Main_Axis,_para->Horizontal_Axis,_para->Vertial_Axis);

    MAX_LIMMIT(_para->END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(_para->END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(_para->END_EFFECTOR_YAW,90);
    MIN_LIMMIT(_para->END_EFFECTOR_YAW,-90);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(_para->Horizontal_Axis,713);
    MIN_LIMMIT(_para->Horizontal_Axis,293);
    MAX_LIMMIT(_para->Vertial_Axis,0);
    MIN_LIMMIT(_para->Vertial_Axis,-400);

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

//    ui->tableWidget->horizontalHeader()->setFixedWidth(ui->tableWidget->width()/4);
//    qDebug("Repaint");
}





OGLWidget::para_Def MainWindow::caculateInvers(float x,float y,float z,float rotate){

   OGLWidget::para_Def _para;

   qDebug("World Position %f,%f,%f,%f",x,y,z,rotate);

   _para.Main_Axis = 2*atan2((x + sqrt(x*x + y*y)),y)/3.1415926*180.f;

   if(_para.Main_Axis>180)
       _para.Main_Axis = _para.Main_Axis-360;

   _para.END_EFFECTOR_YAW =(180+(rotate)-_para.Main_Axis);
   if(_para.END_EFFECTOR_YAW>90)
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

void MainWindow::on_pushButton_clicked()
{
    emit setBackGround();
}

void MainWindow::on_pushButton_2_clicked()
{
    w->show();
}

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
        serial->setBaudRate(10000000);
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


void MainWindow::on_pubm_Contorl_Button_clicked()
{



    if(ui->pubm_Contorl_Button->text()==tr("开气泵"))
    {
        data[0] = 1;
        ui->pubm_Contorl_Button->setText(tr("停止气泵"));
    }
    else{
        data[0] = 2;
        ui->pubm_Contorl_Button->setText(tr("开气泵"));
    }

    serial->write((char*)data,1);

    // on_solid_control_button_clicked();

}

void MainWindow::on_solid_control_button_clicked()
{

    ui->openGLWidget->points.clear();

    //    if(ui->solid_control_button->text()==tr("开电磁阀"))
    //   {
    //    data[0] = 3;
    //    ui->solid_control_button->setText(tr("关电磁阀"));
    //   }
    //   else{
    //    data[0] = 4;
    //    ui->solid_control_button->setText(tr("开电磁阀"));
    //   }

    //    serial->write((char*)data,1);
}

void MainWindow::on_Rotate_valueChanged(int value)
{
    //caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());
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

//       ui->openGLWidget->Real_Para.Horizontal_Axis = ReceiveBuffer2.Horizontal_Axis;
//       ui->openGLWidget->needRepaint =true;
//       ui->Horizontal_Axis->display(ui->openGLWidget->Real_Para.Horizontal_Axis);
//       ui->openGLWidget->Real_Para.Vertial_Axis = ReceiveBuffer2.Vertial_Axis;
//       ui->Vertical_Axis_Display->display(ui->openGLWidget->Real_Para.Vertial_Axis);
//       ui->openGLWidget->Real_Para.END_EFFECTOR_Pitch = ReceiveBuffer2.END_EFFECTOR_Pitch;
//       ui->END_EFFECTOR_Pitch->display(ui->openGLWidget->Real_Para.END_EFFECTOR_Pitch);
//       ui->openGLWidget->Real_Para.END_EFFECTOR_YAW = ReceiveBuffer2.END_EFFECTOR_YAW;
//       ui->END_EFFECTOR_YAW->display( ui->openGLWidget->Real_Para.END_EFFECTOR_YAW);
//       ui->openGLWidget->Real_Para.Main_Axis = ReceiveBuffer2.Main_Axis_Rotate;
//       ui->Main_Axis->display( ui->openGLWidget->Real_Para.Main_Axis);
//       (ReceiveBuffer2.status.Main_Axis_Rotate)? setBackgroundColor(ui->Main_Axis,QColor(255,0,0,255)): setBackgroundColor(ui->Main_Axis,QColor(0,255,0,255));
//       (ReceiveBuffer2.status.Horizontal_Axis)? setBackgroundColor(ui->Horizontal_Axis,QColor(255,0,0,255)): setBackgroundColor(ui->Horizontal_Axis,QColor(0,255,0,255));
//       (ReceiveBuffer2.status.Vertial_Axis)? setBackgroundColor(ui->Vertical_Axis_Display,QColor(255,0,0,255)): setBackgroundColor(ui->Vertical_Axis_Display,QColor(0,255,0,255));
//       (ReceiveBuffer2.status.END_EFFECTOR_YAW)? setBackgroundColor(ui->END_EFFECTOR_YAW,QColor(255,0,0,255)): setBackgroundColor(ui->END_EFFECTOR_YAW,QColor(0,255,0,255));
//       (ReceiveBuffer2.status.END_EFFECTOR_Pitch)? setBackgroundColor(ui->END_EFFECTOR_Pitch,QColor(255,0,0,255)): setBackgroundColor(ui->END_EFFECTOR_Pitch,QColor(0,255,0,255));
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

void MainWindow::para_display(OGLWidget::para_Def _para){

//    ui->Main_Axis->display(_para.Main_Axis);
//    ui->Horizontal_Axis->display(_para.Horizontal_Axis);
//    ui->Vertical_Axis_Display->display(_para.Vertial_Axis);
//    ui->END_EFFECTOR_YAW->display(_para.END_EFFECTOR_YAW);
//    ui->END_EFFECTOR_Pitch->display(_para.END_EFFECTOR_Pitch);

}

void MainWindow:: tttest(){

//    qDebug("Fuck You");

}

void MainWindow::on_chageImages_clicked()
{
    emit changeMode();
}

void MainWindow::on_setOrigin_clicked()
{
    emit setOrigin();
}

void MainWindow::on_shutDownKinect_clicked()
{
    emit shutDownKinect();
}

void MainWindow::on_readParam_clicked()
{
    emit readParam();
}

void MainWindow::on_reconnect_clicked()
{
    emit reconnect();
}
