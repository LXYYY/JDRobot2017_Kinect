#include "Communication.h"

Communication communicator;

Communication::Communication() {
}


Communication::~Communication() {
}

bool Communication::setFrame(unsigned char id, float x,float y,float z) {
    frame.Head1 = HEAD1;
    frame.Head2 = HEAD2;
    frame.Id = id;
    frame.x=x;
    frame.y=y;
    frame.z=z;
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
            && frame_fifo[Tail] == TAIL) {
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

bool Communication::sendFrame(unsigned char id, float x,float y,float z) {
    setFrame(id, x,y,z);
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
