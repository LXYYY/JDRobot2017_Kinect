#include "mainwindow.h"
#include <QApplication>
#include "Threads/Threads.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
//    MyThread myThread;
//    myThread.run();
//    return 0;
}
