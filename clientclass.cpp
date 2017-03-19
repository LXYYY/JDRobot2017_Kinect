#include "clientclass.h"
#include "iostream"
#include "qtimer.h"
//#include <QSound>
#include "malloc.h"
#include "mainwindow.h"
QTimer *timer;
ClientClass::ClientClass(QTcpSocket *client)
{

    this->clientConnection = client;
    //this->car_info.shootSpeed = 0;
    //car_info.ShootCount = 0;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(PackageCheck()));
    //timer->start(2000);
}


void ClientClass::PackageCheck(){

    if(getNewPackage==false)
    {
        //updateText(CarName+QString::fromLocal8Bit("已经离线"));
        delete clientConnection;
        delete this;
        return;
    }
    else
        getNewPackage = false;

}

using namespace std;

QByteArray buffer;
QByteArray RemainData;

union FT_union
{
    char u8[4];
    float f;
}FT;

ReadData_Transform *rt;
float p1[3];
float p2[3];
float p3[3];
void ClientClass::readClient()
{
    buffer = clientConnection->readAll();
    unsigned char* pbuffer = (unsigned char*)buffer.data();
    int i=0;
    quint16 ReciveceLength= buffer.size();
    quint16 CurrentPackageLength=0;
    for(i = 0;i<ReciveceLength;i++)
    {
        if(pbuffer[i]==0x01&&pbuffer[i+1]==0x02)
        {
          rt = (ReadData_Transform*)&(pbuffer[i+3]);
          CurrentPackageLength = pbuffer[i+9];

                qDebug("point1: %f,%f,%f",rt->x,rt->y,rt->z);
                p1[0]=rt->x;
                p1[1]=rt->y;
                p1[2]=rt->z;
                emit drawPoint(p1,pbuffer[i+2]);

            i+=CurrentPackageLength;
            getNewPackage = true;
        }
    }
}

void ClientClass::ClientDisConnected(){
    //emit updateText(CarName+QString::fromLocal8Bit("退出登录\n"));
   // sound1.play();
}
