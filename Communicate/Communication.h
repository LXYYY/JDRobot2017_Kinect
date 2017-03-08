#pragma once

#include "SocketClass/SocketClass.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
//.1.125

#define HEAD1 0x01
#define HEAD2 0x02
#define TAIL 0x55

class Communication {
private:
    union BitConverterU
    {
        unsigned char byte[4];
        float value;
    } BitConverter;

    bool setFrame(unsigned char id, float x,float y,float z);

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
        PointID=0,
        PointX=1,
        PointY=2,
        PointZ=3,
        nDataTypes=4
    };

    SocketClass socket;

    float MsgFromMach[nDataTypes];
    vector< pair<int,Point3d> > PtsToSend;

    Communication();

    ~Communication();

    bool connect(void);

    bool sendFrame(unsigned char id, float x,float y,float z);

    void FrameAnalysis(unsigned char buff[],int buff_len);
};

extern Communication communicator;

