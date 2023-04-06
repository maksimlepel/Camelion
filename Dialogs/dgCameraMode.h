#ifndef DGCAMERAMODE_H
#define DGCAMERAMODE_H

#include <qdialog.h>
#include "ui_dgCameraMode.h"
#include "Utils/ControllerState.h"
#include "Utils/ModeTypes.h"
#include <QMap>

class dgCameraMode : public QDialog
{
    Q_OBJECT
public:
    explicit dgCameraMode(ControllerState *controllerState, QWidget * parent = 0);
    ~dgCameraMode();

private:
    Ui::dgCameraMode *ui;
    QWidget * parent;
    ControllerState *controllerState;
    QMap<Camera, QString> mCameraName;
    QPushButton *activeButton;

    void setCameraMode(CameraMode cameraMode);
    void setEnabledMono(bool enable);
    void setEnabledPiP(bool enable);
    void setEnabledMosaic(bool enable);


    //void fillComboBox(QComboBox* cb, QList<Camera> list);
protected :
    void closeEvent(QCloseEvent *event);
    void reject();

private slots:
    void on_chbMono_clicked();
    void on_chbMosaic_clicked();
    void on_chbPip_clicked();

    void setCamera(Camera camera);
    void on_btMono_clicked();
    void on_btBig_clicked();
    void on_btSmall_clicked();
    void on_btTopLeft_clicked();
    void on_btTopRight_clicked();
    void on_btBottomLeft_clicked();
    void on_btBottomRight_clicked();
signals:
    void sendToRobot();
    void dgSecondVideoShow();
    void dgSecondVideoDelete();

};

#endif // DGCAMERAMODE_H
