//
// Created by root on 3/2/17.
//

#include "SocketClass.h"
#include <iostream>

bool SocketClass::connectServer(void) {
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.103");  ///服务器ip

    ///连接服务器，成功返回true，错误返回false
    if (connect(sock_cli, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        return false;
    }

//    char sendbuf[BUFFER_SIZE];
//    char recvbuf[BUFFER_SIZE];
//    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
//    {
//        send(sock_cli, sendbuf, strlen(sendbuf),0); ///发送
//        if(strcmp(sendbuf,"exit\n")==0)
//            break;
//        recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收
//        fputs(recvbuf, stdout);
//
//        memset(sendbuf, 0, sizeof(sendbuf));
//        memset(recvbuf, 0, sizeof(recvbuf));
//    }

//    close(sock_cli);
    std::cout << "connect success" << std::endl;
    return true;
}

bool SocketClass::sendMsg(unsigned char *frame,size_t len) {
    if(send(sock_cli, frame, len, 0)==-1)
    {
//        connect(sock_cli, (struct sockaddr *) &servaddr, sizeof(servaddr));
        return true;
    } ///发送
    return false;
}

bool SocketClass::connectCheck(){
    int ret=listen(sock_cli,1);
    std::cout<<"connectCheck"<<ret<<std::endl;
    if(ret!=0){
        connectServer();
    }
}

