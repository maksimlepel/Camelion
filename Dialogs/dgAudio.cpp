#include "dgAudio.h"
#include "ui_dgAudio.h"
#include "Utils/ConstInfo.h"
#include "Utils/FuncUtils.h"

dgAudio::dgAudio(QWidget * parent)
    :QDialog(parent,Qt::Popup),
    ui(new Ui::dgAudio)
{
    ui->setupUi(this);
    this->parent = parent;
    QPoint p = parent->mapToGlobal(QPoint(0,0));
    move(p.x() + parent->width(), p.y());

    parent->setStyleSheet(FuncUtils::getBackgroundColorStr(parent));
    setStyleSheet(FuncUtils::getQDialogColorStr());
}

dgAudio::~dgAudio()
{
    delete ui;
}

void dgAudio::closeEvent(QCloseEvent *event)
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
}

void dgAudio::reject()
{
    parent->setStyleSheet(ConstInfo::NO_COLOR);
    QDialog::reject();
}



void dgAudio::on_btTalk_clicked()
{
    emit Talk();
    close();
}

void dgAudio::on_btListen_clicked()
{
    emit Listen();
    close();
}
