#include "dgLightSetting.h"
#include "ui_dgLightSetting.h"
#include "Utils/ConstInfo.h"
#include "Utils/FuncUtils.h"
#include "Windows.h"

dgLightSetting::dgLightSetting(QWidget * parent, int lightId, bool isIRLight)
    :QDialog(parent,Qt::Popup),
    ui(new Ui::dgLightSetting)
{
    ui->setupUi(this);
    this->parent = parent;
    this->lightId = lightId;
    this->isIRLight = isIRLight;
    QPoint p = parent->mapToGlobal(QPoint(0,0));
    move(p.x() + parent->width(), p.y());

    parent->setStyleSheet(FuncUtils::getBackgroundColorStr(parent));
    setStyleSheet(FuncUtils::getQDialogColorStr());

    //mw = new MainWindow(this);
}

dgLightSetting::~dgLightSetting()
{
    delete ui;
}

void dgLightSetting::closeEvent(QCloseEvent *event)
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
}

void dgLightSetting::reject()
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
    QDialog::reject();
}



void dgLightSetting::on_btOff_clicked()
{
    emit setLight(lightId, 0, isIRLight);
    //lb->setPixmap(QPixmap(":/Resources/light_level_0"));
    close();
}

void dgLightSetting::on_bt25Persent_clicked()
{
    emit setLight(lightId, 1, isIRLight);
    //lb->setPixmap(QPixmap(":/Resources/light_level_1"));
    close();
}

void dgLightSetting::on_bt50Persent_clicked()
{
    emit setLight(lightId, 2, isIRLight);
    //lb->setPixmap(QPixmap(":/Resources/light_level_1"));
    close();
}

void dgLightSetting::on_bt75Persent_clicked()
{
    emit setLight(lightId, 3, isIRLight);
    //lb->setPixmap(QPixmap(":/Resources/light_level_1"));
    close();
}

void dgLightSetting::on_bt100Persent_clicked()
{
    emit setLight(lightId, 4, isIRLight);
    //lb->setPixmap(QPixmap(":/Resources/light_level_1"));
    close();
}
