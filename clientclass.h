#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H

#include <QtNetwork>




typedef enum{

    Strike0,
    Strike1,
    Strike2,
    Strike3,
    Strike4,
    Strike5,
    OverPower,
    OverShootFre,
    OverShootSpeed,
    ModuleOffline,
    None
}BloodReduceReason;//车扣血的原因
typedef enum{

     Alaska,
     Husky,
     Samoyed,
     Doge,
     GoldenRetriever

}Car_ID;

typedef struct{

    Car_ID id;//车Id
    unsigned int Blood;//车血量
    BloodReduceReason BloodChangeReason;
    int BloodChange;
    quint32 ShootCount;
    float Power;//功率
    float Votage;
    float Current;
    float shootFre;
    float shootSpeed;


}Car_Info;

class ClientClass:public QObject
{
    Q_OBJECT
public:
    ClientClass(QTcpSocket *client);
    QTcpSocket *clientConnection;
    QString IPAddress;
    QString CarName;
    Car_Info car_info;

    bool getNewPackage;

signals:

    void updateUI(Car_Info *info);
    void updateText(QString string);

    void drawPoint(float *p1,uint8_t id);
public slots:
       void readClient();
       void ClientDisConnected();
       void PackageCheck();
};

#endif // CLIENTCLASS_H
