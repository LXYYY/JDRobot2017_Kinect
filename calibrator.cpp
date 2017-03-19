#include "calibrator.h"
#include "ui_calibrator.h"

Calibrator::Calibrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Calibrator)
{
    ui->setupUi(this);
    connect(this->ui->SetBackground_Button,SIGNAL(clicked(bool)),mainWindow,SLOT(repaint()));

}

Calibrator::~Calibrator()
{
    delete ui;
}

void Calibrator::on_SetBackground_Button_clicked()
{
    emit setBackGround();
}
