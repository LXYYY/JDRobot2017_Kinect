#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "oglwidget.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();



public slots:

    void SetText();

private slots:
    void on_pushButton_clicked();

    void on_Pitch_min_clicked();

    void on_hor_min_clicked();

    void on_Main_min_clicked();

    void on_YAW_min_clicked();

    void on_Ver_min_clicked();

    void on_Pitch_pls_clicked();

    void on_hor_pls_clicked();

    void on_Main_pls_clicked();

    void on_YAW_pls_clicked();

    void on_ver_pls_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

signals:

    void SendPara(OGLWidget::para_Def *);

private:
    Ui::Dialog *ui;
    OGLWidget::para_Def Manual_Position;
};

#endif // DIALOG_H
