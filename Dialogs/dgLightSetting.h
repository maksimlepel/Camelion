#ifndef DGLIGHTSETTING_H
#define DGLIGHTSETTING_H
#include <qdialog.h>
#include "ui_dgLightSetting.h"
//#include "mainwindow.h"

class dgLightSetting: public QDialog
{
    Q_OBJECT
public:
    explicit dgLightSetting(QWidget * parent, int lightId, bool isIRLight);
    ~dgLightSetting();

private:
    Ui::dgLightSetting *ui;
    QWidget * parent;

    int lightId;
    bool isIRLight;

protected :
    void closeEvent(QCloseEvent *event);
    void reject();

private slots:
    void on_btOff_clicked();
    void on_bt25Persent_clicked();
    void on_bt50Persent_clicked();
    void on_bt75Persent_clicked();
    void on_bt100Persent_clicked();
signals:
    void setLight(int lightId, int lightValue,bool isIRLight);
};

#endif // DGLIGHTSETTING_H
