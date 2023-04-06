#include "ControllerMsgSendThread.h"
#include "Utils/FuncUtils.h"
#include <math.h>

int ControllerMsgSendThread::i_U_rob;
static int i_U;
QByteArray ControllerMsgSendThread::qba_U_;
int ControllerMsgSendThread::i_done;
int ControllerMsgSendThread::angle_flips;
int ControllerMsgSendThread::angle_zero;
int ControllerMsgSendThread::angle_one;
int ControllerMsgSendThread::angle_two;
int ControllerMsgSendThread::angle_three;


ControllerMsgSendThread::ControllerMsgSendThread(int numPort, RobotState *robotState, ControllerState *controllerState, QObject *parent)
    :ThreadPrototype(numPort, parent)
{
    i_U = 0;
    i_done = 0;

//    qDebug() << "ControllerMsgSendThread" << numPort;

    moveToThread(this);

    this->controllerState = controllerState;
    this->robotState = robotState;
    // Кондрат 07.10.2015
    timePeriod = ConstInfo::TIME_PERIOD_DEFAULT;

    sengingTimer = new SendingTimer(this);
    connect(this, SIGNAL(changeSendingInteval(int)), sengingTimer, SLOT(changeSendingInteval(int)));
   // connect(sengingTimer, SIGNAL(doIt(QString)), this, SLOT(doIt(QString)));
    connect(sengingTimer, SIGNAL(doIt(QString)), this, SLOT(doIt(QString)));


    connect(this, SIGNAL(errorConnection(int)), sengingTimer, SLOT(stop()));
    connect(this, SIGNAL(isConnected(int)), sengingTimer, SLOT(start()));

    sengingTimer->moveToThread(this);

}

ControllerMsgSendThread::~ControllerMsgSendThread()
{

}


void ControllerMsgSendThread::mainFunction()
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(processRead()));
//    timePeriod = ConstInfo::TIME_PERIOD_DEFAULT;

//    sengingTimer = new SendingTimer(this);
//    connect(this, SIGNAL(changeSendingInteval(int)), sengingTimer, SLOT(changeSendingInteval(int)));
//    connect(sengingTimer, SIGNAL(doIt(QString)), this, SLOT(doIt(QString)));
//    sengingTimer->moveToThread(this);
//    createSocket();

}

int ControllerMsgSendThread::batt_U_rob()
{
    //qDebug() << "angle_my return : "<< angle_my;
    return i_U_rob;
}

int ControllerMsgSendThread::done()
{
    return i_done;
}

int ControllerMsgSendThread::angle_zero_ogr()
{
    return angle_zero;
}

int ControllerMsgSendThread::angle_one_ogr()
{
    return angle_one;
}

int ControllerMsgSendThread::angle_two_ogr()
{
    return angle_two;
}

int ControllerMsgSendThread::angle_three_ogr()
{
    return angle_three;
}

int ControllerMsgSendThread::angle_flips_ogr()
{
    return angle_flips;
}

void ControllerMsgSendThread::sendInfo()
{
    QByteArray block;
    block = controllerState->getMotionPackage(isTwoJoystick);
    int rez = 0;
    rez = socket->write(block);
    if (rez == -1)
    {
        // ошибка отправки
        emit message(trUtf8("Ошибка отправки пакета движения"), ConstInfo::Error);
    }

    block.clear();
    if (isTwoJoystick){
        block = controllerState->getMotionPackageArm2();
    }
    else {
        block = controllerState->getMotionPackageArm();
    }
    rez = 0;
    rez = socket->write(block);
    qDebug() << "BLOCK : " << block.toHex();
    //qDebug() << "inPkg : " << block;
    if (rez == -1)
    {
        // ошибка отправки
        emit message(trUtf8("Ошибка отправки пакета управления манипулятором"), ConstInfo::Error);
    }



//    if  ( !isTwoJoystick && (  (controllerState->getAxisArmForwardBackward()!=0)||
//            (controllerState->getAxisArmLeftRight()!=0)||(controllerState->getAxisArmRotation()!=0)))
//        // Изменились оси движения манипулятора
//    {
//        block.clear();
//        block = controllerState->getMotionPackageArm();
//        rez = 0;
//        rez = socket->write(block);
//        if (rez == -1)
//        {
//            // ошибка отправки
//            emit message(trUtf8("Ошибка отправки пакета управления манипулятором"), ConstInfo::Error);
//        }
//     }
//            // Режим двух джойстиков
//    if  ( isTwoJoystick && ((controllerState->getAxisArmForwardBackward()!=0)||
//                            (controllerState->getAxisArmLeftRight()!=0)||
//                            (controllerState->getAxisArmRotation()!=0)||
//                            (controllerState->getAxisForwardBackward()!=0)||
//                            (controllerState->getAxisLeftRight()!=0)||
//                            (controllerState->getAxisRotation()!=0))

//         )
//        // Изменились оси движения манипулятора и оси движения платформы Режим
//    {
//        block.clear();
//        block = controllerState->getMotionPackageArm2();
//        rez = 0;
//        rez = socket->write(block);
//        if (rez == -1)
//        {
//            // ошибка отправки
//            emit message(trUtf8("Ошибка отправки пакета управления манипулятором"), ConstInfo::Error);
//        }
//    }

}


void ControllerMsgSendThread::recalcTimePeriod()
{
    if (!isCreated)
    {
        return;
    }
    if  (!((controllerState->getAxisForwardBackward()!=0)||(controllerState->getAxisLeftRight()!=0)||
            (controllerState->getAxisRotation()!=0)||(controllerState->getAxisArmForwardBackward()!=0)||
            (controllerState->getAxisArmLeftRight()!=0)||(controllerState->getAxisArmRotation()!=0)))
    {
        // все по нулям, т.е. джойстики не трогают
        if (timePeriod == ConstInfo::TIME_PERIOD)
        {
            // была быстрая отправка - надо включить медленную
            emit changeSendingInteval(ConstInfo::TIME_PERIOD_DEFAULT);
            timePeriod = ConstInfo::TIME_PERIOD_DEFAULT;
        }
        // отправка была медленной - так и остается
    }
    else
    {
        // есть смещение хотя бы по одной оси
        if (timePeriod == ConstInfo::TIME_PERIOD_DEFAULT)
        {
            // была медленная отправка - надо включить быструю
            emit changeSendingInteval(ConstInfo::TIME_PERIOD);
            timePeriod = ConstInfo::TIME_PERIOD;
        }
    }
}



void ControllerMsgSendThread::doIt(QString str)
{
   /* sendInfo();
    emit doItAgain(str); */// сигнал - только для отладки - показать на интерфейсе, как идет отправка пакетов,
                         // потом убрать
}

void ControllerMsgSendThread::sendRightNow()
{
    sengingTimer->sending();
}

void ControllerMsgSendThread::setIsTwoJoystick(bool isTwoJoystick )
 {
   this->isTwoJoystick = isTwoJoystick;

 }

void ControllerMsgSendThread::processRead()
{
    inPkg.fill(0);
    //char * data;
    rcvByte = socket->read(inPkg.data(), 34/*socket->bytesAvailable()*/);
    //inPkg = socket->readAll();

    if (inPkg.length()/*rcvByte*/ > 40)
    {
        analysePackage();
      //  qDebug() << "socket->read" <<  rcvByte << " byte";
    }
}

void ControllerMsgSendThread::analysePackage()
{
    qint8 bb;

    //qDebug() << "inPkg send: " << inPkg.toHex() << "size : " << inPkg.size();
    //qDebug() << "inPkg send: " << inPkg;

    // смотрим тим пакета
    switch (inPkg[6]){
        case ConstInfo::PKG_TYPE_MOBILE_PLATFORM: // ПЛАТФОРМА
        {
            int varValue;
            QByteArray bbb;
            emit changeFlippers(2);
            // передний свет
            bb = inPkg[20];
            QByteArray lightArr;
            lightArr = FuncUtils::byteToTwoByte(bb);
            varValue = lightArr[0]; // белый передний свет
            //emit changeLight(ConstInfo::FRONT_LIGHT, varValue,  false);
            analyseLight(varValue, ConstInfo::FRONT_LIGHT, false);

            varValue = lightArr[1]; // ИК передний свет
            //emit changeLight(ConstInfo::FRONT_LIGHT, varValue,  true);
            analyseLight(varValue, ConstInfo::FRONT_LIGHT, true);

            // задний свет
            bb = inPkg[21];
            lightArr = FuncUtils::byteToTwoByte(bb);

            varValue = lightArr[0]; // белый задний свет
            //emit changeLight(ConstInfo::REAR_LIGHT, varValue, false);
            analyseLight(varValue, ConstInfo::REAR_LIGHT, false);

            varValue = lightArr[1]; // ИК задний свет
            //emit changeLight(ConstInfo::REAR_LIGHT, varValue, true);
            analyseLight(varValue, ConstInfo::REAR_LIGHT, true);

            // уровень сигнала
            bb = inPkg[26];
            if ((bb == 0) || (bb == 0xff))
            {
                varValue = 0;
            }
            else
            {
                varValue = (int) floor((float)bb / 20) + 1;
            }
          //  emit changeSignalLevelRob(varValue);
            /*if (varValue != robotState.getSignalLevelRob())
            {
                robotState.setSignalLevelRob(varValue);
                context.startActivity(FuncUtils.getByteIntent(ConstInfo.SIGNAL_LEVEL_ROB_VARID,
                                                              (byte) varValue,
                                                              context,
                                                              MainActivity.class));
            }*/

            bbb.append(inPkg[25]);
            bbb.append(inPkg[26]);
          /*  bbb.append(0x14);
            bbb.append(0x05);*/
            double d_U_ = (inPkg[25] * 256 + inPkg[26]) * 2.5 * 18.87/1023;
            if(d_U_ > 21){
                double d_U = (d_U_ - 21) *100 / 7;
                varValue = (int) d_U;
            }

            if(varValue != 0){
                qba_U_[i_U] = varValue;
                i_U++;
                if(i_U == 49){
                    int sum = 0;
                    for(int j = 0; j<50; j++){
                        sum += qba_U_[j];
                    }
                    varValue = (int) sum/50;
                    i_U = 0;
                    emit changeBatteryLevelRob(varValue);
                    i_U_rob = varValue;
                    qDebug() << "qba_U : " << qba_U_;
                    qba_U_.clear();
                }

               // qDebug() << "U from MRK send: " << bbb.toHex() << "my d_U : " << d_U << "varValue : " << varValue;
            }
            bbb.clear();

            bbb.append(inPkg[13]);
            unsigned res = (uchar)inPkg[13];
            varValue = (int) res;
            qDebug() << "inPkg[13] : " << bbb.toHex();
           // varValue = (int) inPkg[13];
            if(varValue != 0){
                angle_flips = varValue * 2 + 90;
                if(angle_flips > 360) angle_flips = angle_flips - 360;
                //emit changeFlippers(angle_flips);
               // qDebug() << "Angle flippers s : " << angle_flips;
            }
            bbb.clear();

            // состояние аккумулятора робота
           /* bb = inPkg[25];
            varValue = (int) bb;
            emit changeBatteryLevelRob(varValue);*/
            /*if (varValue != robotState.getBatteryLevelRob())
            {
                robotState.setBatteryLevelRob(varValue);
                context.startActivity(FuncUtils.getByteIntent(ConstInfo.BATTARY_LEVEL_ROB_VARID,
                                                              bb,
                                                              context,
                                                              MainActivity.class));
            }*/

            // Данные инклинометра

            int index_angle;
            index_angle = inPkg[27];
            index_angle = abs(index_angle + 16);
            int angle;
            //angle = abs(inPkg[28]);
            unsigned res_2 = (uchar)inPkg[28];
            angle = (int) res_2;

           // varValue = FuncUtils::byteArrayToShort(bbb);
            //analyseArmJoint(angle, index_angle);

            if(angle != 0){
                switch (index_angle) {
                case 0:
                    angle_zero = angle * 2 - 15;
                    //analyseArmJoint(angle_zero, index_angle);
                    break;
                case 1:
                    angle_one = angle - 10;
                    //analyseArmJoint(angle_one, index_angle);
                    break;
                case 2:
                    angle_two = angle*2 - 90; //- 145;
                    //analyseArmJoint(angle_two, index_angle);
                    break;
                case 3:
                    angle_three = angle - 80; // - 20;
                    //analyseArmJoint(angle_three, index_angle);
                    break;
                default:
                    break;
            }

            //angle = abs(inPkg[28]);

            // qDebug() << "Index manip. send: " << index_angle << "angle : " << angle;
            // qDebug() << "angle_zero send : " << angle_zero << "angle_one : " << angle_one << "angle_two : " << angle_two << "angle_three : " << angle_three;
            }
            bbb.clear();

            bbb.append(inPkg[26]);
            bbb.append(inPkg[27]);
            bbb.append(inPkg[28]);
            bbb.append(inPkg[29]);
            bbb.append(inPkg[30]);
            bbb.append(inPkg[31]);
            qDebug() << "inPkg[26-31] : " << bbb.toHex();
            bbb.clear();

            bbb.append(inPkg[30]);
            //qDebug() << "inPkg[30] : " << bbb.toHex();
            int i_res_done;
            unsigned res_done = (uchar)inPkg[30];
            i_res_done = (int) res_done;
            //qDebug() << "i_res_done : " << i_res_done << "res_done : " << res_done;
            if (i_res_done == 170){
                i_done = 100;
                qDebug() << "i_done = " << i_done;
            }else {
                i_done = 0;
            }

            bbb.clear();

           /* bbb.append(inPkg[27]);
            bbb.append(inPkg[28]);
            varValue = FuncUtils::byteArrayToShort(bbb);
            analyseArmJoint(varValue, 4);
            bbb.clear();*/

            bbb.append(inPkg[29]);
            bbb.append(inPkg[30]);
            varValue = FuncUtils::byteArrayToShort(bbb);
            //analyseArmJoint(varValue, 5);

            // Положение  дополнительной гусеницы
            bbb.clear();
          //  bbb.append(inPkg[18]);
           // bbb.append(inPkg[13]);
            //varValue = FuncUtils::byteArrayToShort(bbb);
           // varValue = 2 * bbb.toInt();
          //  emit changeFlippers(varValue);
            break;
        }
    case ConstInfo::PKG_TYPE_MANIPULATOR: // МАНИПУЛЯТОР
        {
        QByteArray bbb;
        bbb.append(inPkg[7]);
        bbb.append(inPkg[8]);
        int varValue = FuncUtils::byteArrayToShort(bbb);
       // analyseArmJoint(varValue, 0);
        bbb.clear();
        bbb.append(inPkg[9]);
        bbb.append(inPkg[10]);
        varValue = FuncUtils::byteArrayToShort(bbb);
      //  analyseArmJoint(varValue, 1);
        bbb.clear();
        bbb.append(inPkg[11]);
        bbb.append(inPkg[12]);
        varValue = FuncUtils::byteArrayToShort(bbb);
     //   analyseArmJoint(varValue, 2);
        bbb.clear();
        bbb.append(inPkg[13]);
        bbb.append(inPkg[14]);
        varValue = FuncUtils::byteArrayToShort(bbb);
      //  analyseArmJoint(varValue, 3);

        // свет манипулятора 1
        bb = inPkg[24];
        QByteArray lightArr;
        lightArr = FuncUtils::byteToTwoByte(bb);

        varValue = lightArr[0]; // белый свет
        analyseLight(varValue, ConstInfo::ARM1_LIGHT, false);

        varValue = lightArr[1]; // ИК свет
        analyseLight(varValue, ConstInfo::ARM1_LIGHT, true);

        // свет манипулятора 2
        bb = inPkg[25];

        lightArr = FuncUtils::byteToTwoByte(bb);

        varValue = lightArr[0]; // белый свет
        analyseLight(varValue, ConstInfo::ARM2_LIGHT, false);

        varValue = lightArr[1]; // ИК свет
        analyseLight(varValue, ConstInfo::ARM2_LIGHT, true);

        // свет локоть
        bb = inPkg[26];

        lightArr = FuncUtils::byteToTwoByte(bb);

        varValue = lightArr[0]; // белый свет
        analyseLight(varValue, ConstInfo::ELBOW_LIGHT, false);

        varValue = lightArr[1]; // ИК свет
        analyseLight(varValue, ConstInfo::ELBOW_LIGHT, true);

            break;
        }

    }

}

void ControllerMsgSendThread::analyseLight(int lightLevel, int idLight, bool isIRLight)
    {

        if (lightLevel != robotState->getLightLevel(idLight, isIRLight)) //сравниваем с тем, что хранится в RobotState
        {
            // отсылаем сообщение, чтобы поменять картинку
            emit changeLight(idLight, lightLevel, isIRLight);
            /*context.startActivity(FuncUtils.getLightIntent(idLight,
                                                            lightLevel,
                                                            isIRLight,
                                                            ConstInfo.LIGHT_ROB_INTENT,
                                                            context,
                                                            MainActivity.class));*/
            robotState->setLightLevel(idLight, lightLevel, isIRLight);// устанавливаем новое значение
        }
    }

void ControllerMsgSendThread::analyseArmJoint(int positionValue, int idArmJoint)
{
    if (robotState != NULL){
        if (positionValue != robotState->getPositionArm(idArmJoint)) //сравниваем с тем, что хранится в RobotState
        {
            emit changePositionArm(idArmJoint, positionValue);

            robotState->setPositionArm(idArmJoint, positionValue);// устанавливаем новое значение
        }
    }
}
