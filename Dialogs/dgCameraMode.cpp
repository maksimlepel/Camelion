#include "dgCameraMode.h"
#include "ui_dgCameraMode.h"
#include "Utils/ConstInfo.h"
#include "Utils/FuncUtils.h"
#include <QDebug>
#include "Dialogs/CameraSelection.h"

dgCameraMode::dgCameraMode(ControllerState *controllerState, QWidget * parent)
    :QDialog(parent,Qt::Popup),
    ui(new Ui::dgCameraMode)
{
    ui->setupUi(this);
    this->parent = parent;
    this->controllerState = controllerState;

    QPoint p = parent->mapToGlobal(QPoint(0,0));
    move(p.x() + parent->width(), p.y());

    parent->setStyleSheet(FuncUtils::getBackgroundColorStr(parent));
    setStyleSheet(FuncUtils::getQDialogColorStr());

    setCameraMode(controllerState->getCameraMode());

    mCameraName[CAMERA_1] = ConstInfo::CAMERA_1;
    mCameraName[CAMERA_2] = ConstInfo::CAMERA_2;
    mCameraName[CAMERA_3] = ConstInfo::CAMERA_3;
    mCameraName[CAMERA_4] = ConstInfo::CAMERA_4;
    mCameraName[CAMERA_5] = ConstInfo::CAMERA_5;
    //mCameraName[CAMERA_6] = ConstInfo::CAMERA_6;


    ui->btMono->setText(mCameraName.value(controllerState->cameraMono.getCamera()));
    ui->btBig->setText(mCameraName.value(controllerState->cameraPip.cBig.getCamera()));
    ui->btSmall->setText(mCameraName.value(controllerState->cameraPip.cSmall.getCamera()));
    ui->btTopLeft->setText(mCameraName.value(controllerState->cameraMosaic.cTopLeft.getCamera()));
    ui->btTopRight->setText(mCameraName.value(controllerState->cameraMosaic.cTopRight.getCamera()));
    ui->btBottomLeft->setText(mCameraName.value(controllerState->cameraMosaic.cBottomLeft.getCamera()));
    ui->btBottomRight->setText(mCameraName.value(controllerState->cameraMosaic.cBottomRight.getCamera()));

    ui->frPiP->setEnabled(true);
    ui->frPiP->setVisible(true);
    ui->frMosaic->setEnabled(false);
    ui->frMosaic->setVisible(false);
    setFixedHeight(130);

/*    fillComboBox(ui->cbMono, controllerState->cameraMono.getPossibleCamera());
    fillComboBox(ui->cbBig, controllerState->cameraPip.cBig.getPossibleCamera());
    fillComboBox(ui->cbSmall, controllerState->cameraPip.cSmall.getPossibleCamera());
    fillComboBox(ui->cbTopLeft, controllerState->cameraMosaic.cTopLeft.getPossibleCamera());
    fillComboBox(ui->cbTopRight, controllerState->cameraMosaic.cTopRight.getPossibleCamera());
    fillComboBox(ui->cbBottomLeft, controllerState->cameraMosaic.cBottomLeft.getPossibleCamera());
    fillComboBox(ui->cbBottomRight, controllerState->cameraMosaic.cBottomRight.getPossibleCamera());*/

//    ui->cbMono->setCurrentText(mCameraName.value(controllerState->cameraMono.getCamera()));
//    ui->cbBig->setCurrentText(mCameraName.value(controllerState->cameraPip.cBig.getCamera()));
//    ui->cbSmall->setCurrentText(mCameraName.value(controllerState->cameraPip.cSmall.getCamera()));
//    ui->cbTopLeft->setCurrentText(mCameraName.value(controllerState->cameraMosaic.cTopLeft.getCamera()));
//    ui->cbTopRight->setCurrentText(mCameraName.value(controllerState->cameraMosaic.cTopRight.getCamera()));
//    ui->cbBottomLeft->setCurrentText(mCameraName.value(controllerState->cameraMosaic.cBottomLeft.getCamera()));
//    ui->cbBottomRight->setCurrentText(mCameraName.value(controllerState->cameraMosaic.cBottomRight.getCamera()));

}

dgCameraMode::~dgCameraMode()
{
    delete ui;
}

/*void dgCameraMode::fillComboBox(QComboBox* cb, QList<Camera> list)
{
    for (int i = 0; i < list.size(); ++i) {
        cb->addItem(mCameraName.value(list[i]));
    }
}*/

void dgCameraMode::closeEvent(QCloseEvent *event)
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
}

void dgCameraMode::reject()
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
    QDialog::reject();
}

void dgCameraMode::setCameraMode(CameraMode cameraMode)
{
    switch (cameraMode) {
    case cmMono:{
        setEnabledMono(true);
        setEnabledPiP(false);
        setEnabledMosaic(false);
        break;
    }
    case cmPiP:{
        setEnabledMono(false);
        setEnabledPiP(true);
        setEnabledMosaic(false);
        break;
    }
    case cmMosaic:{
        setEnabledMono(false);
        setEnabledPiP(false);
        setEnabledMosaic(true);
        break;
    }
    default:
        break;
    }
}

void dgCameraMode::setEnabledMono(bool enabled)
{
    ui->chbMono->setChecked(enabled);
    ui->btMono->setEnabled(enabled);
}

void dgCameraMode::setEnabledPiP(bool enabled)
{
    ui->chbPip->setChecked(enabled);
    ui->btBig->setEnabled(enabled);
    ui->btSmall->setEnabled(enabled);
}

void dgCameraMode::setEnabledMosaic(bool enabled)
{
    ui->chbMosaic->setChecked(enabled);
    ui->btTopLeft->setEnabled(enabled);
    ui->btTopRight->setEnabled(enabled);
    ui->btBottomLeft->setEnabled(enabled);
    ui->btBottomRight->setEnabled(enabled);
}


void dgCameraMode::on_chbMono_clicked()
{
    setCameraMode(cmMono);
    controllerState->setCameraMode(cmMono);
    emit sendToRobot();
    emit dgSecondVideoDelete();
    raise();
    activateWindow();
}

void dgCameraMode::on_chbPip_clicked()
{
    setCameraMode(cmPiP);
    controllerState->setCameraMode(cmPiP);
    emit sendToRobot();
    emit dgSecondVideoShow();
    raise();
    activateWindow();
}

void dgCameraMode::on_chbMosaic_clicked()
{
    setCameraMode(cmMosaic);
    controllerState->setCameraMode(cmMosaic);
    emit sendToRobot();
}

void dgCameraMode::setCamera(Camera camera)
{
    if (activeButton == ui->btMono) {
        controllerState->cameraMono.setCurrentCamera(camera);
        ui->btMono->setText(mCameraName.value(controllerState->cameraMono.getCamera()));
        emit sendToRobot();
        return;
    }
    if (activeButton == ui->btSmall) {
        controllerState->cameraPip.cSmall.setCurrentCamera(camera);
        ui->btSmall->setText(mCameraName.value(controllerState->cameraPip.cSmall.getCamera()));
        if (!controllerState->cameraPip.checkSmall()) {
            ui->btBig->setText(mCameraName.value(controllerState->cameraPip.cBig.getCamera()));
        }
        emit sendToRobot();
        return;
    }
    if (activeButton == ui->btBig) {
        controllerState->cameraPip.cBig.setCurrentCamera(camera);
        ui->btBig->setText(mCameraName.value(controllerState->cameraPip.cBig.getCamera()));
        if (!controllerState->cameraPip.checkBig()) {
            ui->btSmall->setText(mCameraName.value(controllerState->cameraPip.cSmall.getCamera()));
        }
        emit sendToRobot();
        return;
    }
    if (activeButton == ui->btTopLeft) {
        controllerState->cameraMosaic.cTopLeft.setCurrentCamera(camera);
        ui->btTopLeft->setText(mCameraName.value(controllerState->cameraMosaic.cTopLeft.getCamera()));
        if (!controllerState->cameraMosaic.checkTopLeft()) {
            ui->btTopRight->setText(mCameraName.value(controllerState->cameraMosaic.cTopRight.getCamera()));
            ui->btBottomLeft->setText(mCameraName.value(controllerState->cameraMosaic.cBottomLeft.getCamera()));
            ui->btBottomRight->setText(mCameraName.value(controllerState->cameraMosaic.cBottomRight.getCamera()));
        }
        emit sendToRobot();
        return;
    }
    if (activeButton == ui->btTopRight) {
        controllerState->cameraMosaic.cTopRight.setCurrentCamera(camera);
        ui->btTopRight->setText(mCameraName.value(controllerState->cameraMosaic.cTopRight.getCamera()));
        if (!controllerState->cameraMosaic.checkTopRight()) {
            ui->btTopLeft->setText(mCameraName.value(controllerState->cameraMosaic.cTopLeft.getCamera()));
            ui->btBottomLeft->setText(mCameraName.value(controllerState->cameraMosaic.cBottomLeft.getCamera()));
            ui->btBottomRight->setText(mCameraName.value(controllerState->cameraMosaic.cBottomRight.getCamera()));
        }
        emit sendToRobot();
        return;
    }
    if (activeButton == ui->btBottomLeft) {
        controllerState->cameraMosaic.cBottomLeft.setCurrentCamera(camera);
        ui->btBottomLeft->setText(mCameraName.value(controllerState->cameraMosaic.cBottomLeft.getCamera()));
        if (!controllerState->cameraMosaic.checkBottomLeft()) {
            ui->btTopLeft->setText(mCameraName.value(controllerState->cameraMosaic.cTopLeft.getCamera()));
            ui->btTopRight->setText(mCameraName.value(controllerState->cameraMosaic.cTopRight.getCamera()));
            ui->btBottomRight->setText(mCameraName.value(controllerState->cameraMosaic.cBottomRight.getCamera()));
        }
        emit sendToRobot();
        return;
    }
    if (activeButton == ui->btBottomRight) {
        controllerState->cameraMosaic.cBottomRight.setCurrentCamera(camera);
        ui->btBottomRight->setText(mCameraName.value(controllerState->cameraMosaic.cBottomRight.getCamera()));
        if (!controllerState->cameraMosaic.checkBottomRight()) {
            ui->btTopLeft->setText(mCameraName.value(controllerState->cameraMosaic.cTopLeft.getCamera()));
            ui->btTopRight->setText(mCameraName.value(controllerState->cameraMosaic.cTopRight.getCamera()));
            ui->btBottomLeft->setText(mCameraName.value(controllerState->cameraMosaic.cBottomLeft.getCamera()));
        }
        emit sendToRobot();
        return;
    }
}

void dgCameraMode::on_btMono_clicked()
{
    activeButton = ui->btMono;
    dgCameraSelection *dg = new dgCameraSelection(ui->frMono, ui->btMono, controllerState->cameraMono.getPossibleCamera());
    connect(dg, SIGNAL(setCamera(Camera)), this, SLOT(setCamera(Camera)));
    dg->exec();
}

void dgCameraMode::on_btBig_clicked()
{
    activeButton = ui->btBig;
    dgCameraSelection *dg = new dgCameraSelection(ui->frPiP, ui->btBig, controllerState->cameraPip.cBig.getPossibleCamera());
    connect(dg, SIGNAL(setCamera(Camera)), this, SLOT(setCamera(Camera)));
    //dg->setGeometry(555, 585, 275, 360);
    dg->setGeometry(30, 30, 275, 360);
    dg->exec();
}

void dgCameraMode::on_btSmall_clicked()
{
    activeButton = ui->btSmall;
    dgCameraSelection *dg = new dgCameraSelection(ui->frPiP, ui->btSmall, controllerState->cameraPip.cSmall.getPossibleCamera());
    connect(dg, SIGNAL(setCamera(Camera)), this, SLOT(setCamera(Camera)));
    qDebug() << "setCamera № : " << mCameraName;
    //dg->setGeometry(1275, 585, 275, 360);
    dg->setGeometry(30, 30, 275, 360);
    dg->exec();
}

void dgCameraMode::on_btTopLeft_clicked()
{
    activeButton = ui->btTopLeft;
    dgCameraSelection *dg = new dgCameraSelection(ui->frMosaic, ui->btTopLeft, controllerState->cameraMosaic.cTopLeft.getPossibleCamera());
    connect(dg, SIGNAL(setCamera(Camera)), this, SLOT(setCamera(Camera)));
    dg->exec();
}

void dgCameraMode::on_btTopRight_clicked()
{
    activeButton = ui->btTopRight;
    dgCameraSelection *dg = new dgCameraSelection(ui->frMosaic, ui->btTopRight, controllerState->cameraMosaic.cTopRight.getPossibleCamera());
    connect(dg, SIGNAL(setCamera(Camera)), this, SLOT(setCamera(Camera)));
    dg->exec(); \
}

void dgCameraMode::on_btBottomLeft_clicked()
{
    activeButton = ui->btBottomLeft;
    dgCameraSelection *dg = new dgCameraSelection(ui->frMosaic, ui->btBottomLeft, controllerState->cameraMosaic.cBottomLeft.getPossibleCamera());
    connect(dg, SIGNAL(setCamera(Camera)), this, SLOT(setCamera(Camera)));
    dg->exec();
}

void dgCameraMode::on_btBottomRight_clicked()
{
    activeButton = ui->btBottomRight;
    dgCameraSelection *dg = new dgCameraSelection(ui->frMosaic, ui->btBottomRight, controllerState->cameraMosaic.cBottomRight.getPossibleCamera());
    connect(dg, SIGNAL(setCamera(Camera)), this, SLOT(setCamera(Camera)));
    dg->exec();
}
