#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);



}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::SetText(){

    //ui->pushButton->setText("string");

}

void Dialog::on_pushButton_clicked()
{
    this->close();
}


void Dialog::on_Pitch_min_clicked()
{
    Manual_Position.END_EFFECTOR_Pitch--;
    ui->END_EFFECTOR_Pitch_3->display( Manual_Position.END_EFFECTOR_Pitch);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(Manual_Position.Horizontal_Axis,713);
    MIN_LIMMIT(Manual_Position.Horizontal_Axis,300);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);

}

void Dialog::on_hor_min_clicked()
{
    Manual_Position.Horizontal_Axis--;
    ui->Horizontal_Axis_3->display( Manual_Position.Horizontal_Axis);
    MAX_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(Manual_Position.Horizontal_Axis,713);
    MIN_LIMMIT(Manual_Position.Horizontal_Axis,300);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);
}

void Dialog::on_Main_min_clicked()
{
    Manual_Position.Main_Axis--;
    ui->Main_Axis_3->display( Manual_Position.Main_Axis);
    MAX_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(Manual_Position.Horizontal_Axis,713);
    MIN_LIMMIT(Manual_Position.Horizontal_Axis,300);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);
}

void Dialog::on_YAW_min_clicked()
{
    Manual_Position.END_EFFECTOR_YAW--;
    ui->END_EFFECTOR_YAW_3 ->display( Manual_Position.END_EFFECTOR_YAW);
    MAX_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(Manual_Position.Horizontal_Axis,713);
    MIN_LIMMIT(Manual_Position.Horizontal_Axis,300);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);
}

void Dialog::on_Ver_min_clicked()
{
    Manual_Position.Vertial_Axis--;
    ui->Vertical_Axis_Display_3->display( Manual_Position.Vertial_Axis);
    MAX_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(Manual_Position.Horizontal_Axis,713);
    MIN_LIMMIT(Manual_Position.Horizontal_Axis,300);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);
}

void Dialog::on_Pitch_pls_clicked()
{
    Manual_Position.END_EFFECTOR_Pitch++;
    ui->END_EFFECTOR_Pitch_3->display( Manual_Position.END_EFFECTOR_Pitch);
    MAX_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(Manual_Position.Horizontal_Axis,713);
    MIN_LIMMIT(Manual_Position.Horizontal_Axis,300);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);
}

void Dialog::on_hor_pls_clicked()
{
    Manual_Position.Horizontal_Axis++;
    ui->Horizontal_Axis_3->display( Manual_Position.Horizontal_Axis);
    MAX_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,90);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_Pitch,-90);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);
//  MAX_LIMMIT(Aim_Para.Main_Axis,180);
//  MIN_LIMMIT(Aim_Para.Main_Axis,-180);
    MAX_LIMMIT(Manual_Position.Horizontal_Axis,713);
    MIN_LIMMIT(Manual_Position.Horizontal_Axis,300);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);
}

void Dialog::on_Main_pls_clicked()
{
    Manual_Position.Main_Axis++;
    ui->Main_Axis_3->display( Manual_Position.Main_Axis);

//  MAX_LIMMIT(Manual_Position.Main_Axis,400);
//  MIN_LIMMIT(Manual_Position.Main_Axis,-180);

}

void Dialog::on_YAW_pls_clicked()
{
    Manual_Position.END_EFFECTOR_YAW++;
    ui->END_EFFECTOR_YAW_3->display( Manual_Position.END_EFFECTOR_YAW);

    MAX_LIMMIT(Manual_Position.END_EFFECTOR_YAW,180);
    MIN_LIMMIT(Manual_Position.END_EFFECTOR_YAW,-0);

}

void Dialog::on_ver_pls_clicked()
{
    Manual_Position.Vertial_Axis++;
    ui->Vertical_Axis_Display_3 ->display( Manual_Position.Vertial_Axis);
    MAX_LIMMIT(Manual_Position.Vertial_Axis,0);
    MIN_LIMMIT(Manual_Position.Vertial_Axis,-430);
}

void Dialog::on_pushButton_6_clicked()
{
    emit SendPara(&Manual_Position);
}

void Dialog::on_pushButton_7_clicked()
{
    Manual_Position.Horizontal_Axis = 713;
    Manual_Position.Vertial_Axis = 0;

        emit SendPara(&Manual_Position);
}
