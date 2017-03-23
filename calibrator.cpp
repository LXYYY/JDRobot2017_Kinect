#include "calibrator.h"
#include "ui_calibrator.h"


para_Def Calibrator_Position;


Calibrator::Calibrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Calibrator)
{
    ui->setupUi(this);
//    connect(this->ui->SetBackground_Button,SIGNAL(clicked(bool)),mainWindow,SLOT(repaint()));
    ui->progressBar->setValue(0);


    x_offset = 0;
    y_offset = 660;

}

Calibrator::~Calibrator()
{
    delete ui;
}

void Calibrator::on_SetBackground_Button_clicked()
{
    emit setBackGround();
}

void Calibrator::setProgressbarValue(int value){
    ui->progressBar->setValue(value);
}

void Calibrator::on_pushButton_9_clicked()
{
    emit calibrate();
}

void Calibrator::on_pushButton_10_clicked()
{
    this->close();
}

void Calibrator::on_pushButton_6_clicked()
{

   // OGLWidget::para_Def _realPara = MainWindow::GetRealPara();
     Calibrator_Position.Main_Axis = 0;

    if(ui->pushButton_6->text()==tr("step1"))
   {
    Calibrator_Position.pumb = 0;
    Calibrator_Position.END_EFFECTOR_YAW = 90;
    Calibrator_Position.END_EFFECTOR_Pitch = -30;
    Calibrator_Position.Horizontal_Axis = 700;
    Calibrator_Position.Vertial_Axis = -100;
    ui->pushButton_6->setText((tr("step2")));
   }
    else if(ui->pushButton_6->text()==tr("step2")){
        Calibrator_Position.Horizontal_Axis = 700;
        Calibrator_Position.Vertial_Axis = -380;
        ui->pushButton_6->setText((tr("step3")));
    }
    else if(ui->pushButton_6->text()==tr("step3")){
        Calibrator_Position.Horizontal_Axis = 450;
        Calibrator_Position.Vertial_Axis = -380;
        ui->pushButton_6->setText((tr("Origin")));
    }
    else if(ui->pushButton_6->text()==tr("Origin")){

        Calibrator_Position.Horizontal_Axis = 713;
        Calibrator_Position.Vertial_Axis = 0;
        Calibrator_Position.pumb = 1;
        Calibrator_Position.END_EFFECTOR_Pitch = 0;
        Calibrator_Position.END_EFFECTOR_YAW = 0;
        ui->pushButton_6->setText("step1");
    }

    SendPara(&Calibrator_Position);
}



void Calibrator::on_pushButton_8_clicked()
{
    emit readParam();
}

void Calibrator::on_pushButton_2_clicked()
{
    y_offset+=10;
    emit Setoffset(x_offset,y_offset);
}

void Calibrator::on_y_offset_min_clicked()
{
    y_offset-=10;
    emit Setoffset(x_offset,y_offset);
}

void Calibrator::on_x_offset_min_clicked()
{
    x_offset-=10;
    emit Setoffset(x_offset,y_offset);
}

void Calibrator::on_pushButton_5_clicked()
{
    x_offset+=10;
    emit Setoffset(x_offset,y_offset);
}
