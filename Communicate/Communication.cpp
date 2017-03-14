#include "Communication.h"

//Communication communicator;

Communication::Communication() {
}


Communication::~Communication() {
}

bool Communication::setFrame(unsigned char id, float x, float y, float z,float dirX,float dirY, unsigned char color) {
    frame.Head1 = HEAD1;
    frame.Head2 = HEAD2;
    frame.Id = id;
    frame.x=x;
    frame.y=y;
    frame.z=z;
//    cout<<x<<","<<y<<","<<z<<endl;
    frame.dir=atan(dirY/dirX);
    frame.color=color;
//    frame.Tail1=TAIL1;
//    frame.Tail2=TAIL2;
    return true;
}

void Communication::FrameAnalysis(unsigned char buff[], int buff_len) {
    unsigned char frame_fifo[FrameLen];
    for (int i = 0; i < buff_len; i++) {
        //FIFO??
        for (int j = 0; j < FrameLen - 1; j++) {
            frame_fifo[j] = frame_fifo[j + 1];
        }
        frame_fifo[FrameLen - 1] = buff[i];

        if (frame_fifo[Head1] == HEAD1
            && frame_fifo[Head2] == HEAD2
            && frame_fifo[Tail] == TAIL1) {
            unsigned char id = frame_fifo[dataType];
            BitConverter.byte[0] = frame_fifo[data0];
            BitConverter.byte[1] = frame_fifo[data1];
            BitConverter.byte[2] = frame_fifo[data2];
            BitConverter.byte[3] = frame_fifo[data3];

            if (frame_fifo[sumCheck] == (frame_fifo[data2] + frame_fifo[data3]))      ////sumcheck
                if (id <= nDataTypes) {
                    MsgFromMach[id] = BitConverter.value;
                }
        }
    }
}

bool Communication::sendMsg(unsigned char* msg) {
    socket.sendMsg(msg,sizeof(FrameStructS));
    return false;
}

bool Communication::sendFrame(unsigned char id, float x,float y,float z,float dirX,float dirY,unsigned char color) {
//    cout<<"sendframe"<<x<<","<<y<<","<<z<<endl;

    setFrame(id, x,y,z,dirX,dirY,color);
    if (sendMsg((unsigned char*)&frame)) {
        return true;
    } ///发送
    return false;
}

bool Communication::connect(void) {
    if(!socket.connetServer()){
        perror("commucation connect failed");
        return false;
    }
    return true;
}

void Communication::run(){
    connect();
}

void Communication::sendPoint(unsigned char id, float* point){
    sendFrame(id,point[0],point[1],point[2],point[3],point[4],point[5]);
//    cout<<"sendPoint"<<point[0]<<","<<point[1]<<","<<point[2]<<endl;
    delete[] point;
}

bool receiveMsg(unsigned char* buff,int buff_len){

}
