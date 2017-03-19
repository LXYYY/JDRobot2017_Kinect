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

private slots:
    void on_SetBackground_Button_clicked();

private:
    Ui::Calibrator *ui;
};

#endif // CALIBRATOR_H
