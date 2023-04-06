#include "ControllerState.h"
#include "Utils/ConstInfo.h"
#include <QDataStream>
#include "Utils/FuncUtils.h"
#include <QDebug>

ControllerState::ControllerState()
{
    axisForwardBackward = 0;
    axisLeftRight = 0;
    axisRotation = 0;
    axisArmForwardBackward = 0;
    axisArmLeftRight = 0;
    axisArmRotation = 0;

    //свет
    lightMap[ConstInfo::FRONT_LIGHT] = 0;
    lightMap[ConstInfo::REAR_LIGHT] = 0;
    lightMap[ConstInfo::ARM1_LIGHT] = 0;
    lightMap[ConstInfo::ARM2_LIGHT] = 0;
    lightMap[ConstInfo::ELBOW_LIGHT] =  0;

    // свет ИК
    lightMapIR[ConstInfo::FRONT_LIGHT] = 0;
    lightMapIR[ConstInfo::REAR_LIGHT] = 0;
    lightMapIR[ConstInfo::ARM1_LIGHT] = 0;
    lightMapIR[ConstInfo::ARM2_LIGHT] = 0;
    lightMapIR[ConstInfo::ELBOW_LIGHT] = 0;

    indexPackage = 0;
    butArmVal= 0;
    //cameraMode = cmMono;
    cameraMode = cmPiP;

    // Установить режим медленно
    butSpeedVal=ConstInfo::SLOW_MODE;

    pipBigCameraNum = 1;
    pipSmallCameraMode = 2;
//    mosaicTopLeftCameraNum = 1;
//    mosaicTopRightCameraNum = 2;
//    mosaicBottomLeftCameraNum = 3;
//    mosaicBottomRightCameraNum = 4;
}


// SET

void ControllerState::setButArm(short  butArm)
{
    butArmVal = butArm;
}

void ControllerState::setButSpeed(double  butSpeed)
{
    butSpeedVal = butSpeed;
}

void ControllerState::setAxisForwardBackward(qint8 axisXval)
{
    axisForwardBackward = axisXval;
}

void ControllerState::setAxisLeftRight(qint8 axisYval)
{
    axisLeftRight = axisYval;
}

void ControllerState::setAxisRotation(qint8 axisZval)
{
    axisRotation = axisZval;
}

void ControllerState::setAxisArmForwardBackward(qint8 axisRXval)
{
    axisArmForwardBackward = axisRXval;
}

void ControllerState::setAxisArmLeftRight(qint8 axisRYval)
{
    axisArmLeftRight = axisRYval;
}

void ControllerState::setAxisArmRotation(qint8 axisRZval)
{
    axisArmRotation = axisRZval;
}

void ControllerState::setLight (int lightId, int lightValue,bool isIRLight)
{
    if (isIRLight)
    {
        lightMapIR[lightId] = lightValue;
    }
     else
    {
        lightMap[lightId] = lightValue;
    }
}

void ControllerState::setAllLightOff(bool isIRLight)
{
    if (isIRLight)
    {
        for (QHash<int, int>::const_iterator i = lightMapIR.constBegin(); i != lightMapIR.constEnd(); i++)
        {
            lightMapIR[i.key()] = 0;
        }
    }
    else
    {
        for (QHash<int, int>::const_iterator i = lightMap.constBegin(); i != lightMap.constEnd(); i++)
        {
            lightMap[i.key()] = 0;
        }
    }
}

void ControllerState::setCameraMode(CameraMode cameraMode)
{
    this->cameraMode = cameraMode;
}

//void ControllerState::setMonoCameraNum(Camera camera)
//{
//    cameraMono = camera;
//}

// GET
short ControllerState::getButArmVal()
{
    return butArmVal;
}

double ControllerState::getButSpeedVal()
{
    return butSpeedVal;
}

qint8 ControllerState::getAxisForwardBackward()
{
    return axisForwardBackward;
}

qint8 ControllerState::getAxisLeftRight()
{
    return axisLeftRight;
}

qint8 ControllerState::getAxisRotation()
{
    return axisRotation;
}

qint8 ControllerState::getAxisArmForwardBackward()
{
    return axisArmForwardBackward;
}

qint8 ControllerState::getAxisArmLeftRight()
{
    return axisArmLeftRight;
}

qint8 ControllerState::getAxisArmRotation()
{
    return axisArmRotation;
}

int ControllerState::getLightVal(int lightId)
{
    return lightMap.value(lightId);
}

QByteArray ControllerState::getMotionPackage(bool isTwoJoystick)
{
    //Формирование пакета для отправки команд для модулей мобильной платформы 0хВ1
    QByteArray bMotionPackage;

    // формируем "шапку" пакета
    QByteArray bPkgHead;
    bPkgHead.resize(7);

    // изменяем аутоинкремент
    indexPackage++;
    if (indexPackage < 0)
    {
        indexPackage = 1;
    }

    bPkgHead = pkgHead(ConstInfo::ID_ENGINE, indexPackage, ConstInfo::PKG_TYPE_MOBILE_PLATFORM);

    bMotionPackage.append(bPkgHead);

    // формируем основную "начинку" пакета (данные)
    if (isTwoJoystick){
        qint8 tmp = 0;
        bMotionPackage.append(tmp); // движение взад-вперед
        bMotionPackage.append(tmp); // движение влево-вправо
        bMotionPackage.append(tmp); // движение доп. гусениц
    }
    else {
        bMotionPackage.append(axisForwardBackward); // движение взад-вперед
        bMotionPackage.append(axisLeftRight); // движение влево-вправо
        bMotionPackage.append(axisRotation); // движение доп. гусениц
    }

    // управление передней подсветкой шасси
    qint8 btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::FRONT_LIGHT),lightMapIR.value(ConstInfo::FRONT_LIGHT));
    bMotionPackage.append(btLight);

    // управление задней подсветкой шасси
    btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::REAR_LIGHT),lightMapIR.value(ConstInfo::REAR_LIGHT));
    bMotionPackage.append(btLight);

    // управление модулем коммутации сигналов
    // по простому камеры
    qint8 btCamera = 0;
    switch (cameraMode) {
    case cmMono: {
        btCamera |= (1 << ( 6 - cameraMono.getCamera()));
        break;
    }
    case cmPiP: {
        if (cameraPip.cBig.getCamera() < cameraPip.cSmall.getCamera()){
            // первой указана камера большой картинки
            btCamera |= (1 << 6);
        }
        else {
            // первой указана камера маленькой картинки
            btCamera |= (1 << 7);
        }
        btCamera |= (1 << ( 6 - cameraPip.cBig.getCamera()));
        //qDebug() << "getCamera : " << cameraPip.cBig.getCamera() << "   btCamera : " << btCamera;
        btCamera |= (1 << ( 6 - cameraPip.cSmall.getCamera()));
        //qDebug() << "getCamera : " << cameraPip.cSmall.getCamera() << "   btCamera : " << btCamera;
        break;
    }
    case cmMosaic: {
        btCamera |= (1 << 7);
        btCamera |= (1 << 6);
        btCamera |= (1 << ( 6 - cameraMosaic.cTopLeft.getCamera()));
        btCamera |= (1 << ( 6 - cameraMosaic.cTopRight.getCamera()));
        btCamera |= (1 << ( 6 - cameraMosaic.cBottomLeft.getCamera()));
        btCamera |= (1 << ( 6 - cameraMosaic.cBottomRight.getCamera()));
        break;
    }
    default:
        break;
    }

    bMotionPackage.append(btCamera);

    // принудительная команда модулю электропитания
    // ... TODO
    bMotionPackage.append(ConstInfo::NO_COMAND);
    bMotionPackage.append(ConstInfo::NO_COMAND);

    return  bMotionPackage;



    qint8 btCamera_2 = 0;
    switch (cameraMode) {
    case cmMono: {
        btCamera_2 |= (1 << ( 4 - cameraMono.getCamera()));
        break;
    }
    case cmPiP: {
        if (cameraPip.cBig.getCamera() < cameraPip.cSmall.getCamera()){
            // первой указана камера большой картинки
            btCamera_2 |= (1 << 4);
        }
        else {
            // первой указана камера маленькой картинки
            btCamera_2 |= (1 << 5);
        }
        btCamera_2 |= (1 << ( 4 - cameraPip.cBig.getCamera()));
        //qDebug() << "getCamera : " << cameraPip.cBig.getCamera() << "   btCamera_2 : " << btCamera_2;
        btCamera_2 |= (1 << ( 4 - cameraPip.cSmall.getCamera()));
        //qDebug() << "getCamera : " << cameraPip.cSmall.getCamera() << "   btCamera_2 : " << btCamera_2;
        break;
    }
    case cmMosaic: {
        btCamera_2 |= (1 << 5);
        btCamera_2 |= (1 << 4);
        btCamera_2 |= (1 << ( 4 - cameraMosaic.cTopLeft.getCamera()));
        btCamera_2 |= (1 << ( 4 - cameraMosaic.cTopRight.getCamera()));
        btCamera_2 |= (1 << ( 4 - cameraMosaic.cBottomLeft.getCamera()));
        btCamera_2 |= (1 << ( 4 - cameraMosaic.cBottomRight.getCamera()));
        break;
    }
    default:
        break;
    }

    bMotionPackage.append(btCamera_2);
}

QByteArray ControllerState::getMotionPackageArm()
{
    //Формирование пакета для отправки команд для модулей манипулятора 0хВ2
    QByteArray bMotionPackage;

    // формируем "шапку" пакета
    QByteArray bPkgHead;
    bPkgHead.resize(7);

    // изменяем аутоинкремент
    indexPackage++;
    if (indexPackage < 0)
    {
        indexPackage = 1;
    }

     bPkgHead = pkgHead(ConstInfo::ID_ENGINE, indexPackage, ConstInfo::PKG_TYPE_MANIPULATOR);

     bMotionPackage.append(bPkgHead);

     // формируем основную "начинку" пакета (данные)
     qint8 tmp = 0;
     //if (armMode == ARM_123/*butArmVal == ConstInfo::BUT_ARM123*/) {
       /*  bMotionPackage.append(axisArmRotation);  // движение манипулятора в шарнире 1 ( общий поворот)
         bMotionPackage.append(axisArmForwardBackward);  // движение манипулятора в шарнире 2 ( наклон плеча)
         bMotionPackage.append(axisArmLeftRight);  // движение манипулятора в шарнире 3 ( наклон локтя)
         bMotionPackage.append(tmp); // движение захватом манипулятора
         bMotionPackage.append(tmp); // движение манипулятора в шарнире 5( поворот кисти)
         bMotionPackage.append(tmp); // движение манипулятора в шарнире 4 ( наклон кисти)

     }*/
    // if (armMode == ARM_234/*butArmVal == ConstInfo::BUT_ARM234*/) {
         /*bMotionPackage.append(tmp);  // движение манипулятора в шарнире 1 ( общий поворот)
         bMotionPackage.append(axisArmForwardBackward);  // движение манипулятора в шарнире 2 ( наклон плеча)
         bMotionPackage.append(axisArmLeftRight);  // движение манипулятора в шарнире 3 ( наклон локтя)
         bMotionPackage.append(tmp); // движение захватом манипулятора
         bMotionPackage.append(tmp); // движение манипулятора в шарнире 5( поворот кисти)
         bMotionPackage.append(axisArmRotation); // движение манипулятора в шарнире 4 ( наклон кисти)

     }*/
    // if (armMode == ARM_456/*butArmVal == ConstInfo::BUT_ARM456*/)
     {
         bMotionPackage.append(tmp);  // движение манипулятора в шарнире 1 ( общий паворот)
         bMotionPackage.append(tmp);  // движение манипулятора в шарнире 2 ( наклон плеча)
         bMotionPackage.append(tmp);  // движение манипулятора в шарнире 3 ( наклон локтя)
         bMotionPackage.append(axisArmRotation); // движение захватом манипулятора
         bMotionPackage.append(axisArmLeftRight); // движение манипулятора в шарнире 5( поворот кисти)
         bMotionPackage.append(axisArmForwardBackward); // движение манипулятора в шарнире 4 ( наклон кисти)
     }

     // управление подсветкой манипулятора кисти 1
     qint8 btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::ARM1_LIGHT), lightMapIR.value(ConstInfo::ARM1_LIGHT));
     bMotionPackage.append(btLight);

     // управление подсветкой манипулятора кисти 2
     btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::ARM2_LIGHT), lightMapIR.value(ConstInfo::ARM2_LIGHT));
     bMotionPackage.append(btLight);

     // управление подсветкой манипулятора на локте
     btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::ELBOW_LIGHT), lightMapIR.value(ConstInfo::ELBOW_LIGHT));
     bMotionPackage.append(btLight);

     return bMotionPackage;

}

QByteArray ControllerState::getMotionPackageArm2()
{
    //Формирование пакета для отправки команд для модулей манипулятора 0хВ2
    QByteArray bMotionPackage;

    // формируем "шапку" пакета
    QByteArray bPkgHead;
    bPkgHead.resize(7);

    // изменяем аутоинкремент
    indexPackage++;
    if (indexPackage < 0)
    {
        indexPackage = 1;
    }

     bPkgHead = pkgHead(ConstInfo::ID_ENGINE, indexPackage, ConstInfo::PKG_TYPE_MANIPULATOR);

     bMotionPackage.append(bPkgHead);

     // формируем основную "начинку" пакета (данные)
     qint8 tmp = 0;
     //if (armMode == ARM_123/*butArmVal == ConstInfo::BUT_ARM456*/)
     /*{
         bMotionPackage.append(axisArmRotation);  // движение манипулятора в шарнире 1 ( общий поворот)
         bMotionPackage.append(axisArmForwardBackward);  // движение манипулятора в шарнире 2 ( наклон плеча)
         bMotionPackage.append(axisArmLeftRight);  // движение манипулятора в шарнире 3 ( наклон локтя)
         bMotionPackage.append(axisRotation); // движение манипулятора в шарнире 4 ( наклон кисти)
         bMotionPackage.append(axisLeftRight); // движение манипулятора в шарнире 5( поворот кисти)
         bMotionPackage.append(axisForwardBackward); // // движение захватом манипулятора
     }*/
     //if (armMode == ARM_234/*butArmVal == ConstInfo::BUT_ARM234*/)
     /*{
         bMotionPackage.append(axisRotation);  // движение манипулятора в шарнире 1 ( общий паворот)
         bMotionPackage.append(axisArmForwardBackward);  // движение манипулятора в шарнире 2 ( наклон плеча)
         bMotionPackage.append(axisArmLeftRight);  // движение манипулятора в шарнире 3 ( наклон локтя)
         bMotionPackage.append(axisArmRotation); // движение манипулятора в шарнире 4 ( наклон кисти)
         bMotionPackage.append(axisLeftRight); // движение манипулятора в шарнире 5( поворот кисти)
         bMotionPackage.append(axisForwardBackward); // // движение захватом манипулятора

     }*/
     //if (armMode == ARM_456/*butArmVal == ConstInfo::BUT_ARM123*/)
     {
         bMotionPackage.append(axisRotation);  // движение манипулятора в шарнире 1 ( общий поворот)
         bMotionPackage.append(axisForwardBackward);  // движение манипулятора в шарнире 2 ( наклон плеча)
         bMotionPackage.append(axisLeftRight);  // движение манипулятора в шарнире 3 ( наклон локтя)
         bMotionPackage.append(axisArmRotation); // движение манипулятора в шарнире 4 ( наклон кисти)
         bMotionPackage.append(axisArmLeftRight); // движение манипулятора в шарнире 5( поворот кисти)
         bMotionPackage.append(axisArmForwardBackward); // // движение захватом манипулятора

     }

     // управление подсветкой манипулятора кисти 1
     qint8 btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::ARM1_LIGHT), lightMapIR.value(ConstInfo::ARM1_LIGHT));
     bMotionPackage.append(btLight);

     // управление подсветкой манипулятора кисти 2
     btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::ARM2_LIGHT), lightMapIR.value(ConstInfo::ARM2_LIGHT));
     bMotionPackage.append(btLight);

     // управление подсветкой манипулятора на локте
     btLight = FuncUtils::twoIntsToByte(lightMap.value(ConstInfo::ELBOW_LIGHT), lightMapIR.value(ConstInfo::ELBOW_LIGHT));
     bMotionPackage.append(btLight);

     return bMotionPackage;

}


// формирование шапки пакета (общая для всех пакетов)
QByteArray ControllerState::pkgHead(int deviceId, short autoInc, qint8 pkgType)
{
    QByteArray bPackageHead;

    bPackageHead.append(FuncUtils::intToByteArray(deviceId));
    bPackageHead.append(FuncUtils::shortToByteArray(autoInc));
    bPackageHead.append(pkgType);
    return bPackageHead;
}

void ControllerState::setArmMode(ArmMode armMode)
{
    this->armMode = armMode;
}

void ControllerState::setMoveMode(MoveMode moveMode)
{
    this->moveMode = moveMode;
}

ArmMode ControllerState::getArmMode()
{
    return armMode;
}

MoveMode ControllerState::getMoveMode()
{
    return moveMode;
}

void ControllerState::nextArmMode()
{
    armMode++;
}

void ControllerState::nextMoveMode()
{
    moveMode++;
}

CameraMode ControllerState::getCameraMode()
{
    return cameraMode;
}

//Camera ControllerState::getMonoCameraNum()
//{
//    return cameraMono.getCamera();
//}
