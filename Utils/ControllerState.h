#ifndef CONTROLLERSTATE_H
#define CONTROLLERSTATE_H

#include <QByteArray>
#include <QHash>
#include "Utils/ModeTypes.h"

class ControllerState
{
public:

    ControllerState();

     void setButArm(short  butArm);
     void setButSpeed(double  butSpeed);
     void setAxisForwardBackward(qint8 axisXval);
     void setAxisLeftRight(qint8 axisYval);
     void setAxisRotation(qint8 axisZval);
     void setAxisArmForwardBackward(qint8 axisRXval);
     void setAxisArmLeftRight(qint8 axisRYval);
     void setAxisArmRotation(qint8 axisRZval);
     void setArmMode(ArmMode armMode);
     void setMoveMode(MoveMode moveMode);
     void setCameraMode(CameraMode cameraMode);

     void setLight (int lightId, int lightValue,bool isIRLight);
     void setAllLightOff(bool isIRLight);

     short getButArmVal();
     double getButSpeedVal();
     qint8 getAxisForwardBackward();
     qint8 getAxisLeftRight();
     qint8 getAxisRotation();
     qint8 getAxisArmForwardBackward();
     qint8 getAxisArmLeftRight();
     qint8 getAxisArmRotation();
     ArmMode getArmMode();
     MoveMode getMoveMode();
     CameraMode getCameraMode();

     int getLightVal(int lightId);

     QByteArray getMotionPackage(bool isTwoJoystick);     //Формирование пакета для отправки команд для модулей мобильной платформы 0хВ1
     QByteArray getMotionPackageArm();   //Формирование пакета для отправки команд для модулей манипулятора 0хВ2
     QByteArray getMotionPackageArm2();   //Формирование пакета для отправки команд для модулей манипулятора 0хВ2,
                                          //при управлении правым джойстиком (режим работы два джойстика)

     QByteArray pkgHead(int deviceId, short autoInc, qint8 pkgType);// формирование шапки пакета (общая для всех пакетов)

     void nextArmMode();
     void nextMoveMode();

     CameraMono cameraMono;
     CameraPip cameraPip;
     CameraMosaic cameraMosaic;
private:

    short indexPackage; // автоинкремент пакета базовых команд (для мобильной платформы)

    // значения по осям
    qint8 axisForwardBackward;
    qint8 axisLeftRight;
    qint8 axisRotation;
    qint8 axisArmForwardBackward;
    qint8 axisArmLeftRight;
    qint8 axisArmRotation;

    // значения кнопки манипулятора
    short butArmVal;

    // режим движения робота
    double butSpeedVal;

    // свет
    QHash<int, int> lightMap;

    // свет ИК
    QHash<int, int> lightMapIR;

    ArmMode armMode; // режим управления рукой

    MoveMode moveMode; // режим движения платформы

    CameraMode cameraMode; // режим камеры


    int pipBigCameraNum;
    int pipSmallCameraMode;
//    int mosaicTopLeftCameraNum;
//    int mosaicTopRightCameraNum;
//    int mosaicBottomLeftCameraNum;
//    int mosaicBottomRightCameraNum;

};

#endif // CONTROLLERSTATE_H
