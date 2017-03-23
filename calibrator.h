#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class Calibrator;
}

class Calibrator : public QDialog
{
    Q_OBJECT

public:
    explicit Calibrator(QWidget *parent = 0);
    ~Calibrator();
    MainWindow *mainWindow;

 signals:
    void setBackGround();
    void calibrate();
    void SendPara(para_Def *);
    void readParam();
    void Setoffset(float,float);
private slots:
    void on_SetBackground_Button_clicked();
    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_2_clicked();

    void on_y_offset_min_clicked();

    void on_x_offset_min_clicked();

    void on_pushButton_5_clicked();

public slots:
    void setProgressbarValue(int value);
private:
    Ui::Calibrator *ui;

    int x_offset;
    int y_offset;

};

#endif // CALIBRATOR_H
