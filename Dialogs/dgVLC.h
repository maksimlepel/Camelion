#ifndef DG_VLC_H
#define DG_VLC_H
#include <qdialog.h>
//#include <qwidget.h>
#include "ui_dgVLC.h"
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class dgVLC : public QDialog
{
    Q_OBJECT
public:
    explicit dgVLC(QWidget * parent = 0);
    void show_video( VlcMedia  *_media);
    ~dgVLC();
private slots:



private:
    Ui::dgVLC * ui;
    QWidget * parent;

    VlcInstance *_instance2;
    VlcMedia *_media2;
    VlcMediaPlayer *_player2;

signals:

//protected :
//    void closeEvent(QCloseEvent *event);
    //void reject();
};

#endif
