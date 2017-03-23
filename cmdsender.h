#ifndef CMDSENDER_H
#define CMDSENDER_H

#include <QMainWindow>
#include <QWidget>
#include "qthread.h"
#include "qsemaphore.h"
#include "qserialport.h"
#include"oglwidget.h"
#include "vector"
#include "QQueue"
#include "QMutex"

class CmdSender : public QThread
{
Q_OBJECT
public:
    CmdSender();
    QSemaphore *cmd_over;
    QSerialPort *port;
    bool StartWaitForFinish;
  //  std::vector<para_Def *> commands;
    QQueue<para_Def> commands;
    bool setSemaphore(QSemaphore*);
    bool setPort(QSerialPort*);
    bool pushCmd(para_Def );
    QMutex mutex;

    para_Def popCmd(void);
    void run(void);
signals:
    void para_display(para_Def );
    void uart_send(para_Def *);
    void one_box_finish();
//private:

private slots:


};

#endif // CMDSENDER_H
