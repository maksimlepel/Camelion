#ifndef DGAUDIO_H
#define DGAUDIO_H
#include <qdialog.h>
#include "ui_dgAudio.h"


class dgAudio : public QDialog
{
    Q_OBJECT
public:
    explicit dgAudio(QWidget * parent = 0);
    ~dgAudio();

private:
    Ui::dgAudio *ui;
    QWidget * parent;

protected :
    void closeEvent(QCloseEvent *event);
    void reject();

private slots:
    void on_btTalk_clicked();
    void on_btListen_clicked();
signals:
     void Talk();
     void Listen();
};

#endif // DGAUDIO_H
