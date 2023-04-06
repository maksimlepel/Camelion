#include <QMessageBox>
#include "dgArm.h"
#include "ui_dgArm.h"
#include "Utils/ConstInfo.h"
#include "Utils/FuncUtils.h"

dgArm::dgArm(bool isTwoJoystick,QWidget * parent)
    :QDialog(parent,Qt::Popup),
    ui(new Ui::dgArm)
{
    ui->setupUi(this);
    this->isTwoJoystick=isTwoJoystick;
    this->parent = parent;
    QPoint p = parent->mapToGlobal(QPoint(0,0));
    move(p.x() + parent->width(), p.y());
    parent->setStyleSheet(FuncUtils::getBackgroundColorStr(parent));
    setStyleSheet(FuncUtils::getQDialogColorStr());
}

dgArm::~dgArm()
{
    delete ui;
}

void dgArm::closeEvent(QCloseEvent *event)
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
}

void dgArm::reject()
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
    QDialog::reject();
}



void dgArm::on_bt1Joystick_clicked()
{
    emit OneJoystick();
    close();
}

void dgArm::on_bt2Joystick_clicked()
{
//     if (!isTwoJoystick)
//       {
//         QMessageBox::warning(
//                      this,
//                     "Предупреждение",
//                     "Переключение в режим двух джойстиков допускается только в режиме останова (активна кнопка Движение)!" );
//       }
//     else
//       {
//         emit TwoJoystick();
//         close();
//        }

     emit TwoJoystick();
     close();
}
