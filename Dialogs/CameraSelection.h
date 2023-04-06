#ifndef CAMERASELECTION_H
#define CAMERASELECTION_H
#include <qdialog.h>
#include "ui_dgCameraSelection.h"
#include "Utils/ModeTypes.h"
#include <QPushButton>

class dgCameraSelection: public QDialog
{
    Q_OBJECT
public:
    explicit dgCameraSelection(QWidget * parent, QPushButton * button, QList<Camera> possibleCamera);
    ~dgCameraSelection();
private:
    Ui::dgCameraSelection *ui;
    QWidget * parent;
    QPushButton * button;
    QList<Camera> possibleCamera;

    void init();

protected :
    void closeEvent(QCloseEvent *event);
    void reject();

private slots:
    void on_btCamera1_clicked();

    void on_btCamera2_clicked();

    void on_btCamera3_clicked();

    void on_btCamera4_clicked();

    void on_btCamera5_clicked();

    void on_btCamera6_clicked();

signals:
    void setCamera(Camera camera);
};

#endif // CAMERASELECTION_H
