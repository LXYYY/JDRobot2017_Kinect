#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMouseEvent"
#include "iostream"
#include "QTableWidgetItem"
using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //
    ui->setupUi(this);

    connect(ui->openGLWidget,SIGNAL(update_mat_info(float*,int )),this,SLOT(update_mat_display(float*,int )));

    //setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{

    ui->openGLWidget->joint1.rotate_y = value;
   // ui->openGLWidget->repaint();
    ui->lcdNumber_3->display(ui->openGLWidget->joint1.rotate_y);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->openGLWidget->joint2.trans_x = value;
    ui->openGLWidget->repaint();
   // ui->chuizhi->display(value);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->openGLWidget->Vx = value;
    //ui->openGLWidget->repaint();
    ui->lcdNumber->display(ui->openGLWidget->Vx);
}


void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->openGLWidget->joint2.trans_y = value;
   // ui->openGLWidget->repaint();
    ui->shuiping->display(value);
}

void MainWindow::update_mat_display(float  mat[16],int index ){

    // ui->tableWidget->setItem(1,1,new QTableWidgetItem("123"));

    for(int i=0;i<4;i++){

        for(int j=0;j<4;j++){

                ui->tableWidget->setItem(i+index*4,j,new QTableWidgetItem(QString().sprintf("%4.2f",mat[i*4+j])));
        }

    }


}
void MainWindow::repaint(){

    ui->tableWidget->horizontalHeader()->setFixedWidth(ui->tableWidget->width()/4);
    qDebug("Repaint");
}

