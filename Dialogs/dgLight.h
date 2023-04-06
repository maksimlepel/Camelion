#ifndef DGLIGHT_H
#define DGLIGHT_H
#include <qdialog.h>
#include "ui_dgLight.h"

class DgLight : public QDialog
{
    Q_OBJECT
public:
    explicit DgLight(QWidget * parent = 0, bool isIRLight = false);
    ~DgLight();

private:
    Ui::dgLight *ui;
    QWidget * parent;
    bool isIRLight;
protected :
    void closeEvent(QCloseEvent *event);
    void reject();

private slots:
    void on_btAllOff_clicked();
    void on_btFront_clicked();
    void on_btRear_clicked();
    void on_btArm1_clicked();
    void on_btArm2_clicked();

    void settingLight(int idLight, int valueLight, bool isIRLight);

signals:
    void lightOff(bool isIRLight);
    void setLight(int lightId, int lightValue,bool isIRLight);
};

#endif // DGLIGHT_H
