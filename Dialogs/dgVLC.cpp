#include <QMessageBox>
#include "dgVLC.h"
#include "ui_dgVLC.h"
#include "Utils/ConstInfo.h"
#include "Utils/FuncUtils.h"
#include <libvlc-qt/include/VLCQtCore/Common.h>
#include <libvlc-qt/include/VLCQtCore/Instance.h>
#include <libvlc-qt/include/VLCQtCore/Media.h>
#include <libvlc-qt/include/VLCQtCore/MediaPlayer.h>

dgVLC::dgVLC(QWidget * parent)
    :QDialog(parent, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint),
    ui(new Ui::dgVLC)
{
    ui->setupUi(this);
   // this->isTwoJoystick=isTwoJoystick;
    this->parent = parent;

//    Qt::WindowFlags wf = windowFlags();
//    wf = Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint;
//    setWindowFlags(wf);


    QPoint p = parent->mapToGlobal(QPoint(0,0));
    move(p.x() - 310, p.y());
//    parent->setStyleSheet(FuncUtils::getBackgroundColorStr(parent));
//    setStyleSheet(FuncUtils::getQDialogColorStr());
   _instance2 = new VlcInstance(VlcCommon::args(), this);
    _player2 = new VlcMediaPlayer(_instance2);
    _player2->setVideoWidget(ui->widgetVideo);
}

dgVLC::~dgVLC()
{
    delete ui;
    delete _player2;
    delete  _instance2;
}

//void dgVLC::closeEvent(QCloseEvent *event)
//{
//    parent->setStyleSheet(ConstInfo::NO_COLOR);
//}

void dgVLC::show_video( VlcMedia  *_media)
{
   _player2->open(_media);
}

//void dgVLC::reject()
//{
//    parent->setStyleSheet(ConstInfo::NO_COLOR);
//    QDialog::reject();
//}



