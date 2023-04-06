#ifndef DG_ARM_H
#define DG_ARM_H
#include <qdialog.h>
#include "ui_dgArm.h"

class dgArm : public QDialog
{
    Q_OBJECT
public:
    explicit dgArm(bool isTwoJoystick,QWidget * parent = 0);
    ~dgArm();
private slots:

   // void on_btRecord_clicked();

  //  void on_btDelay_clicked();

    void on_bt1Joystick_clicked();

    void on_bt2Joystick_clicked();

private:
    Ui::dgArm *ui;
    QWidget * parent;
    bool isTwoJoystick;

signals:
        void OneJoystick();
        void TwoJoystick();
protected :
    void closeEvent(QCloseEvent *event);
    void reject();
};

#endif
