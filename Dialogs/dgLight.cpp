#include "dgLight.h"
#include "ui_dgLight.h"
#include "Utils/ConstInfo.h"
#include "Utils/FuncUtils.h"
#include "Dialogs/dgLightSetting.h"
#include "QDebug"

DgLight::DgLight(QWidget * parent, bool isIRLight)
    :QDialog(parent,Qt::Popup),
    ui(new Ui::dgLight)
{
    ui->setupUi(this);
    this->parent = parent;
    this->isIRLight = isIRLight;
    QPoint p = parent->mapToGlobal(QPoint(0,0));
    move(p.x() + parent->width(), p.y());

    parent->setStyleSheet(FuncUtils::getBackgroundColorStr(parent));
    setStyleSheet(FuncUtils::getQDialogColorStr());
}

DgLight::~DgLight()
{
    delete ui;
}

void DgLight::closeEvent(QCloseEvent *event)
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
}

void DgLight::reject()
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
    QDialog::reject();
}


void DgLight::on_btAllOff_clicked()
{
    emit lightOff(isIRLight);
    close();
}

void DgLight::on_btFront_clicked()
{
    dgLightSetting *dg = new dgLightSetting(ui->btFront, ConstInfo::FRONT_LIGHT, isIRLight);
    connect(dg, SIGNAL(setLight(int,int,bool)), this, SLOT(settingLight(int,int,bool)));
    dg->exec();
}

void DgLight::on_btRear_clicked()
{
    dgLightSetting *dg = new dgLightSetting(ui->btRear, ConstInfo::REAR_LIGHT, isIRLight);
    connect(dg, SIGNAL(setLight(int,int,bool)), this, SLOT(settingLight(int,int,bool)));
    dg->exec();
}

void DgLight::on_btArm1_clicked()
{
    dgLightSetting *dg = new dgLightSetting(ui->btArm1, ConstInfo::ARM1_LIGHT, isIRLight);
    connect(dg, SIGNAL(setLight(int,int,bool)), this, SLOT(settingLight(int,int,bool)));
    dg->exec();
}

void DgLight::on_btArm2_clicked()
{
    dgLightSetting *dg = new dgLightSetting(ui->btArm2, ConstInfo::ARM2_LIGHT, isIRLight);
    connect(dg, SIGNAL(setLight(int,int,bool)), this, SLOT(settingLight(int,int,bool)));
    dg->exec();
}


void DgLight::settingLight(int lightId, int lightValue,bool isIRLight)
{
    //qDebug() << "lightId : " << lightId << "lightValue : " << lightValue << "isIRLight : " << isIRLight;
    emit setLight(lightId, lightValue, isIRLight);
}
