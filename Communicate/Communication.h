#pragma once

#include "SocketClass/SocketClass.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QThread>
using namespace std;
using namespace cv;
//.1.125

#define HEAD1 0x01
#define HEAD2 0x02
#define TAIL1 0x55
#define TAIL2 0x54

class Communication : public QThread {
    Q_OBJECT
private:
    union BitConverterU
    {
        unsigned char byte[4];
        float value;
    } BitConverter;

    bool setFrame(unsigned char id, float x,float y,float z,float dirX,float dirY,unsigned char color);

    bool sendMsg(unsigned char* msg);

public:

    ///////protocal////////
    struct FrameStructS
    {
        unsigned char Head1;
        unsigned char Head2;
        unsigned char Id;
        float x;
        float y;
        float z;
        float dir;
        unsigned char color;
//        unsigned char Tail1;
//        unsigned char Tail2;
    }  __attribute__((packed)) frame;

    enum FrameStructE {
        Head1 ,
        Head2 ,
        dataType,
        data0,
        data1,
        data2,
        data3,
        Tail,
        sumCheck,
        FrameLen
    };
//    unsigned char frame[FrameLen];
    enum dataTypeE
    {
        TgtBox=0,
        TgtBoxDir,
        TgtCase,

        SetBackGnd,
        SetBackGndEnd,
        SetOrigin,
        SetOriginEnd,
        InitCases,
        InitCasesEnd,
        SearchBox,
        SearchCase,
        Pause,
        nDataTypes
    };

    SocketClass socket;

    float MsgFromMach[nDataTypes];
    Point3d PtsToSend;

    Communication();

    ~Communication();

    void run();

    bool connect(void);

    bool sendFrame(unsigned char id, float x,float y,float z,float dirX,float dirY,unsigned char color);

    bool receiveMsg(unsigned char* buff,int buff_len);

    void FrameAnalysis(unsigned char buff[],int buff_len);

public slots:
    void sendPoint(unsigned char id, float* point);
};
