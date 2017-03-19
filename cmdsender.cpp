#include "cmdsender.h"
#include "oglwidget.h"




CmdSender::CmdSender()
{

}


bool CmdSender::setSemaphore(QSemaphore * _semaphore){


    if(_semaphore!=NULL)
    {
        this->cmd_over = _semaphore;
    }
    else
     {
        qErrnoWarning("semaphore NULL pointer");
        return false ;
    }
}



bool CmdSender::pushCmd(OGLWidget::para_Def cmdPosition){
    //OGLWidget::para_Def *_pointer = new OGLWidget::para_Def; //新生成一个结构体

//    if(_pointer!=NULL)
//    memcpy(_pointer,&cmdPosition,sizeof(OGLWidget::para_Def)); //拷贝数据
//    else return false;
    commands.enqueue(cmdPosition);
    return true;
}

OGLWidget::para_Def CmdSender::popCmd(){

    OGLWidget::para_Def nullPointer;
    nullPointer.Main_Axis = -1;
    if(commands.count()!=0)
    return commands.dequeue();
    else
    return nullPointer;
}

bool startChecking = false;
void CmdSender::run(void){
 OGLWidget::para_Def cmd;

 qDebug("ThreadStart");
 qDebug("commands count%d",commands.count());
 qDebug("semaphore count%d",cmd_over->available());
// startChecking = true;

         StartWaitForFinish = true;

         cmd_over->acquire();
    while(commands.count()!=0){


        mutex.lock();
        qDebug("New Cycle");
        cmd = commands.dequeue();
        qDebug("Sleep time%d",cmd.DelayTime);
        emit uart_send(&cmd);
        emit para_display(cmd);
        qDebug("Main %f,Horizontal %f,Vertial%f",cmd.Main_Axis,cmd.Horizontal_Axis,cmd.Vertial_Axis);

        mutex.unlock();

        cmd_over->acquire();
        qDebug("收到运动结束信号准备进入延时");
        sleep(cmd.DelayTime);
        StartWaitForFinish = true;




    }
 // cmd_over->acquire();
  qDebug("Commands Finished Thread Exit");
  sleep(3);
  qDebug("next box....");
  //emit one_box_finish();
}
