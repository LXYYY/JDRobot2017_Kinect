#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H

#include <QtNetwork>


typedef struct{

    float x;
    float y;
    float z;
    float vx;
    unsigned char color;
}ReadData_Transform;

class ClientClass:public QObject
{
    Q_OBJECT
public:
    ClientClass(QTcpSocket *client);
    QTcpSocket *clientConnection;

    bool getNewPackage;

signals:


    void updateText(QString string);

    void drawPoint(float *p1,uint8_t id);
public slots:
       void readClient();
       void ClientDisConnected();
       void PackageCheck();
};

#endif // CLIENTCLASS_H
