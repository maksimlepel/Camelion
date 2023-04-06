#include "CameraSelection.h"
#include "Utils/ConstInfo.h"
#include "Utils/FuncUtils.h"


dgCameraSelection::dgCameraSelection(QWidget * parent, QPushButton * button, QList<Camera> possibleCamera)
    :QDialog(parent,Qt::Popup),
    ui(new Ui::dgCameraSelection)
{
    ui->setupUi(this);
    this->parent = parent;
    this->button = button;
    this->possibleCamera = possibleCamera;

    QPoint p = parent->mapToGlobal(QPoint(0,0));
    move(p.x() + parent->width(), p.y());

    button->setStyleSheet(FuncUtils::getBackgroundColorStr(button));
    setStyleSheet(FuncUtils::getQDialogColorStr());

    init();
}

dgCameraSelection::~dgCameraSelection()
{
    delete ui;
}

void dgCameraSelection::init(){
    QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
    for (int i = 0; i < allPButtons.size(); i++) {
        allPButtons.at(i)->setVisible(false);
    }
    for (int i = 0; i < possibleCamera.size(); i++) { //(int i = 0; i < 4; i++)
        switch (possibleCamera.at(i)) {
        case CAMERA_1:
            ui->btCamera1->setVisible(true);
            break;
        case CAMERA_2:
            ui->btCamera2->setVisible(true);
            break;
        case CAMERA_3:
            ui->btCamera3->setVisible(true);
            break;
        case CAMERA_4:
            ui->btCamera4->setVisible(true);
            break;
        case CAMERA_5:
            ui->btCamera5->setVisible(false);
            break;
        case CAMERA_6:
            ui->btCamera6->setVisible(false);
            break;
        default:
            break;
        }
    }
    QRect r = this->geometry();
    r.setHeight((ui->btCamera1->height() + 6)* possibleCamera.size() + 10); //r.setHeight((ui->btCamera1->height() + 6)* possibleCamera.size() + 10);
    ui->verticalLayout->setGeometry(r);
    this->setGeometry(r);
}

void dgCameraSelection::closeEvent(QCloseEvent *event)
{
    button->setStyleSheet(ConstInfo::NO_COLOR);
}

void dgCameraSelection::reject()
{
    button->setStyleSheet(ConstInfo::NO_COLOR);
    QDialog::reject();
}

void dgCameraSelection::on_btCamera1_clicked()
{
    emit setCamera(CAMERA_1);
    close();
}

void dgCameraSelection::on_btCamera2_clicked()
{
    emit setCamera(CAMERA_2);
    close();
}

void dgCameraSelection::on_btCamera3_clicked()
{
    emit setCamera(CAMERA_3);
    close();
}

void dgCameraSelection::on_btCamera4_clicked()
{
    emit setCamera(CAMERA_4);
    close();
}

void dgCameraSelection::on_btCamera5_clicked()
{
    emit setCamera(CAMERA_5);
    close();
}

void dgCameraSelection::on_btCamera6_clicked()
{
    emit setCamera(CAMERA_6);
    close();
}
