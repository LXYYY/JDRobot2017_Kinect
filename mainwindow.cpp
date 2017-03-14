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
#include "globject.h"
#include "Threads/Threads.h"
using namespace std;

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
Dialog *w;

QSerialPort *serial;
QTcpServer *server;
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
    ui->image1->resize(ui->image1->pixmap()->size());
}

void MainWindow::pushDepth(unsigned char* imageData,int cols,int rows,int bytesPerLine)
{
//    cout<<"depth:"<<bytesPerLine<<endl;
    depth=QImage(imageData,cols,rows,bytesPerLine,QImage::Format_RGB888);
    ui->image2->clear();
    ui->image2->setPixmap(QPixmap::fromImage(depth));
    ui->image2->resize(ui->image2->pixmap()->size());
}

void MainWindow::pushContours(unsigned char* imageData,int cols,int rows,int bytesPerLine){

}

void MainWindow::pushProj(unsigned char* imageData,int cols,int rows,int bytesPerLine){


}

void MainWindow::draw1Point(float *points, int size){
//    uint32_t size = ui->openGLWidget->points.size();
//    for(int i=0;i<(int32_t)(id-size+1);i++){
//        float* _tmp = new float[3];
//        ui->openGLWidget->points.push_back(_tmp);
//        qDebug("ID:%d,i:%d point size:%d",id,i,size);
//    }
//    qDebug("draw points");
//    cout<<"size="<<size<<endl;
    ui->openGLWidget->points.clear();
    for(size_t i=0;i<size;i++){
        float *tpoints = new float[3];
        tpoints[0]=points[(i-1)*3+0];
        tpoints[1]=points[(i-1)*3+1];
        tpoints[2]=points[(i-1)*3+2];
        ui->openGLWidget->points.push_back(tpoints);
//        delete tpoints;
////        if(tpoints[0]==0&&tpoints[1]==0){
////            cout<<"drawPoints:"<<tpoints[0]<<","<<tpoints[1]<<","<<tpoints[2]<<endl;
////        }
    }
//    cout<<"drawPointsize:"<<ui->openGLWidget->points.size()<<endl;
//    for(size_t i=0;i<ui->openGLWidget->points.size();i++){
//        cout<<ui->openGLWidget->points.at(i)[0]<<","
//             <<ui->openGLWidget->points.at(i)[1]<<","
//            <<ui->openGLWidget->points.at(i)[2]<<","<<endl;
//    }
    ui->openGLWidget->needRepaint = true;
}
MyThread* myThread;
Communication* communicator;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //
    ui->setupUi(this);
    //processer = new cvProcesser();
    connect(ui->openGLWidget,SIGNAL(update_mat_info(float*,int )),this,SLOT(update_mat_display(float*,int )));

    w = new Dialog(this);
    server = new QTcpServer();
    server->listen(QHostAddress::Any, 6000);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(serial.portName());
            serial.close();
            ui->startPort_Button->setEnabled(true);
        }
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
    myThread->start();
    communicator->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::update_mat_display(float  mat[16],int index ){

//    // ui->tableWidget->setItem(1,1,new QTableWidgetItem("123"));

//    for(int i=0;i<4;i++){

//        for(int j=0;j<4;j++){

//            ui->tableWidget->setItem(i+index*5,j,new QTableWidgetItem(QString().sprintf("%4.2f",mat[i*4+j])));
//        }

//    }


}
void MainWindow::repaint(){

//    ui->tableWidget->horizontalHeader()->setFixedWidth(ui->tableWidget->width()/4);
//    qDebug("Repaint");
}


void MainWindow::on_Main_Axis_ADJ_valueChanged(int value)
{
    ui->openGLWidget->joint1.rotate_y = value;
    ui->openGLWidget->needRepaint =true;
    ui->Main_Axis->display(value);
}

void MainWindow::on_Horizontal_Axis_ADJ_valueChanged(int value)
{
    ui->openGLWidget->joint2.trans_x = value;
    ui->openGLWidget->needRepaint =true;
    ui->Horizontal_Axis->display(value);
}

void MainWindow::on_Vertical_Axis_ADJ_valueChanged(int value)
{
    ui->openGLWidget->joint2.trans_y = value;
    ui->openGLWidget->needRepaint =true;
    ui->Vertical_Axis_Display->display(value);
}

void MainWindow::on_END_Effecter_ADJ1_valueChanged(int value)
{
    ui->openGLWidget->joint3.rotate_y = value;
    ui->openGLWidget->needRepaint =true;
    ui->End_Effect_Rotate_1->display(value);
}

void MainWindow::on_END_Effecter_ADJ2_valueChanged(int value)
{

    ui->openGLWidget->joint4.rotate_z = value;
    ui->openGLWidget->needRepaint =true;
    ui->End_Effect_Rotate_2->display(value);
}

void MainWindow::caculateInvers(float x,float y,float z){

    float tx,tz;
    tx = x;
    tz = z;

    //   x = tx*sin(ui->Rotate->value()/180.f*3.1415926)+tz*cos(ui->Rotate->value()/180.f*3.1415926);
    //   z = tx*cos(ui->Rotate->value()/180.f*3.1415926)-tz*sin(ui->Rotate->value()/180.f*3.1415926);

    x-=cos(ui->Rotate->value()/180.f*3.1415926)*108.33;
    z+=sin(ui->Rotate->value()/180.f*3.1415926)*108.33;

    ui->openGLWidget->joint1.rotate_y = 2*atan((x + sqrt(x*x + z*z))/z)/3.1415926*180.f;
    ui->openGLWidget->joint3.rotate_y = ui->Rotate->value()-ui->openGLWidget->joint1.rotate_y;
    ui->openGLWidget->joint2.trans_x  = sqrt(x*x + z*z);
    ui->openGLWidget->joint2.trans_y  = 71317/100 - y;
    ui->openGLWidget->needRepaint = true;


//    qDebug("x:%f,y:%f,Z:%f",x,y,z);

//    qDebug("a2:%f",2*atan(sqrt(x + (x*x + z*z))/z));

    ui->End_Effect_Rotate_1->display(ui->openGLWidget->joint3.rotate_y);
    ui->Vertical_Axis_Display->display(ui->openGLWidget->joint2.trans_y);
    ui->Horizontal_Axis->display(ui->openGLWidget->joint2.trans_x);
    ui->Main_Axis->display(ui->openGLWidget->joint1.rotate_y);


}


void MainWindow::on_Move_X_valueChanged(int value)
{
    caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());

}

void MainWindow::on_Move_Y_valueChanged(int value)
{
    caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());

}

void MainWindow::on_Move_z_valueChanged(int value)
{
    caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());

}

void MainWindow::timeup(){

    static float x,y,z;


    caculateInvers(cos(x+=0.1)*500+500,0,sin(y+=0.1)*500+500);

}

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
        //           ui->PortBox->setEnabled(false);
        //           ui->BaudBox->setEnabled(false);
        //           ui->BitNumBox->setEnabled(false);
        //           ui->ParityBox->setEnabled(false);
        //           ui->StopBox->setEnabled(false);
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



USB_Trans_TypeDef ReceiveBuffer;
void MainWindow::Read_Data(){
    char *data = (char*)&ReceiveBuffer;
    serial->read(data,sizeof(USB_Trans_TypeDef));


//    qDebug("%f,%f",ReceiveBuffer.Servo_Motor_Pitch_Angle,ReceiveBuffer.Servo_Motor_Yaw_Angle);

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
    caculateInvers(ui->Move_X->value(),ui->Move_Y->value(),ui->Move_z->value());
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
