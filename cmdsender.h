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
  //  std::vector<OGLWidget::para_Def*> commands;
    QQueue<OGLWidget::para_Def> commands;
    bool setSemaphore(QSemaphore*);
    bool setPort(QSerialPort*);
    bool pushCmd(OGLWidget::para_Def);
    QMutex mutex;

    OGLWidget::para_Def popCmd(void);
    void run(void);
signals:
    void para_display(OGLWidget::para_Def);
    void uart_send(OGLWidget::para_Def*);
    void one_box_finish();
//private:

private slots:


};

#endif // CMDSENDER_H
