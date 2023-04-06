#include <math.h>
#include "ControllerMsgRcvThread.h"
#include "Utils/FuncUtils.h"

int ControllerMsgRcvThread::i_U_rob;
int ControllerMsgRcvThread::i_done;
int ControllerMsgRcvThread::angle_zero;
int ControllerMsgRcvThread::angle_one;
int ControllerMsgRcvThread::angle_two;
int ControllerMsgRcvThread::angle_three;

static int i_U;
static int i_done;

QByteArray ControllerMsgRcvThread::qba_U_;

ControllerMsgRcvThread::ControllerMsgRcvThread(int numPort, RobotState *robotState, QObject *parent)
    :ThreadPrototype(numPort, parent)
{
//    qDebug() << "ControllerMsgRcvThread" << numPort;
    moveToThread(this);

    this->robotState = robotState;

    inPkg.resize(28);
    rcvByte = 0;

    angle_zero = 0;
    angle_one = 0;
    angle_two = 0;
    angle_three = 0;

    i_done = 0;
}

ControllerMsgRcvThread::~ ControllerMsgRcvThread()
{

}

void ControllerMsgRcvThread::mainFunction()
{
    //createSocket(ConstInfo::PORT_RECEIVE_MSG);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processRead()));
}

void ControllerMsgRcvThread::processRead()
{
    inPkg.fill(0);
    //char * data;
    rcvByte = socket->read(inPkg.data(), 28/*socket->bytesAvailable()*/);
    if (rcvByte > 40)
    {
        analysePackage();
      //  qDebug() << "socket->read" <<  rcvByte << " byte";
    }
}

int ControllerMsgRcvThread::batt_U_rob()
{
    //qDebug() << "angle_my return : "<< angle_my;
    return i_U_rob;
}

void ControllerMsgRcvThread::analysePackage()
{
    qint8 bb;
    QByteArray bbb;

    //qDebug() << "inPkg : " << inPkg.toHex() << "size : " << inPkg.size();
   // qDebug() << "inPkg : " << inPkg;
    // смотрим тим пакета
    switch (inPkg[6]){
        case ConstInfo::PKG_TYPE_MOBILE_PLATFORM: // ПЛАТФОРМА
        {
            QByteArray bbb;
            // передний свет
            bb = inPkg[19];
            QByteArray lightArr;
            lightArr = FuncUtils::byteToTwoByte(bb);

            int varValue = lightArr[0]; // белый передний свет
            //emit changeLight(ConstInfo::FRONT_LIGHT, varValue,  false);
            analyseLight(varValue, ConstInfo::FRONT_LIGHT, false);

            varValue = lightArr[1]; // ИК передний свет
            //emit changeLight(ConstInfo::FRONT_LIGHT, varValue,  true);
            analyseLight(varValue, ConstInfo::FRONT_LIGHT, true);

            // задний свет
            bb = inPkg[20];
            lightArr = FuncUtils::byteToTwoByte(bb);

            varValue = lightArr[0]; // белый задний свет
            //emit changeLight(ConstInfo::REAR_LIGHT, varValue, false);
            analyseLight(varValue, ConstInfo::REAR_LIGHT, false);

            varValue = lightArr[1]; // ИК задний свет
            //emit changeLight(ConstInfo::REAR_LIGHT, varValue, true);
            analyseLight(varValue, ConstInfo::REAR_LIGHT, true);

            // состояние аккумулятора робота
         /*   bb = inPkg[24];
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

        /*    bbb.append(inPkg[25]);
            bbb.append(inPkg[26]);*/
          /*  bbb.append(0x14);
            bbb.append(0x05);*/
          /*  double d_U = (((inPkg[25] * 256 + inPkg[26]) * 2.5 * 18.87/1023) - 21) *100 / 8;
            varValue = (int) d_U / 29;
            if(varValue != 0){
                emit changeBatteryLevelRob(varValue);
                    qDebug() << "U from MRK : " << bbb.toHex() << "my d_U : " << d_U << "varValue : " << varValue;
            }
            bbb.clear();*/

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

               // qDebug() << "U from MRK rec : " << bbb.toHex() << "my d_U : " << d_U << "varValue : " << varValue;
            }
            bbb.clear();


            bbb.append(inPkg[13]);
            qDebug() << "Angle for flippers rcv : " << bbb.toHex();
            varValue = bbb.toInt();
            if(varValue != 0){
                //emit changeFlippers(varValue * 2);
                qDebug() << "Angle for flippers rec : " << varValue;
            }
            bbb.clear();

            // уровень сигнала
         /*   bb = inPkg[25];
            if ((bb == 0) || (bb == 0xff))
            {
                varValue = 0;
            }
            else
            {
                varValue = (int) floor((float)bb / 20) + 1;
            }*/
          //  emit changeSignalLevelRob(varValue);
            /*if (varValue != robotState.getSignalLevelRob())
            {
                robotState.setSignalLevelRob(varValue);
                context.startActivity(FuncUtils.getByteIntent(ConstInfo.SIGNAL_LEVEL_ROB_VARID,
                                                              (byte) varValue,
                                                              context,
                                                              MainActivity.class));
            }*/
            // Данные инклинометра

            int index_angle;
            index_angle = inPkg[27];
            index_angle = abs(index_angle + 16);
            int angle;
            angle = abs(inPkg[28]);
           // varValue = FuncUtils::byteArrayToShort(bbb);
            if(angle != 0){
                //analyseArmJoint(angle, index_angle);

                switch (index_angle) {
                case 0:
                    angle_zero = angle;
                    break;
                case 1:
                    angle_one = angle;
                    break;
                case 2:
                    angle_two = angle;
                    break;
                case 3:
                    angle_three = angle;
                    break;
                default:
                    break;
                }

           // qDebug() << "Index manip. rec : " << index_angle << "angle : " << angle;
           // qDebug() << "angle_zero rec: " << angle_zero << "angle_one : " << angle_one << "angle_two : " << angle_two << "angle_three : " << angle_three;
            }
            bbb.clear();

            int i_res_done;
            unsigned res_done = (uchar)inPkg[30];
            i_res_done = (int) res_done;
            if (i_res_done == 170){
                i_done = 100;
                //qDebug() << "i_done = " << i_done;

            }else {
                i_done = 0;
            }

           // QByteArray bbb;
           /* bbb.append(inPkg[26]);
            bbb.append(inPkg[27]);
            varValue = FuncUtils::byteArrayToShort(bbb);
            analyseArmJoint(varValue, 4);
            bbb.clear();
            bbb.append(inPkg[28]);
            bbb.append(inPkg[29]);
            varValue = FuncUtils::byteArrayToShort(bbb);
            analyseArmJoint(varValue, 5);*/

            // Положение  дополнительной гусеницы
          //  bbb.clear();
          //  bbb.append(inPkg[12]);
            bbb.append(inPkg[13]);
            varValue = bbb.toInt();
            if(varValue != 0){
//                emit changeFlippers(varValue * 2);
                qDebug() << "Angle for flippers : " << varValue * 2;
            }
            bbb.clear();
            //varValue = FuncUtils::byteArrayToShort(bbb);
           // qDebug() << "Flipers : " << varValue;
           // emit changeFlippers(varValue);

            break;
        }
    case ConstInfo::PKG_TYPE_MANIPULATOR: // МАНИПУЛЯТОР
        {
        //QByteArray bbb;
        bbb.append(inPkg[7]);
        bbb.append(inPkg[8]);
        int varValue = FuncUtils::byteArrayToShort(bbb);
      //  analyseArmJoint(varValue, 0);
        bbb.clear();
        bbb.append(inPkg[9]);
        bbb.append(inPkg[10]);
        varValue = FuncUtils::byteArrayToShort(bbb);
      //  analyseArmJoint(varValue, 1);
        bbb.clear();
        bbb.append(inPkg[11]);
        bbb.append(inPkg[12]);
        varValue = FuncUtils::byteArrayToShort(bbb);
    //    analyseArmJoint(varValue, 2);
        bbb.clear();
        bbb.append(inPkg[13]);
        bbb.append(inPkg[14]);
        varValue = FuncUtils::byteArrayToShort(bbb);
     //   analyseArmJoint(varValue, 3);

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

int ControllerMsgRcvThread::done()
{
    return i_done;
}

int ControllerMsgRcvThread::angle_zero_ogr()
{
    return angle_zero;
}

int ControllerMsgRcvThread::angle_one_ogr()
{
    return angle_one;
}

int ControllerMsgRcvThread::angle_two_ogr()
{
    return angle_two;
}

int ControllerMsgRcvThread::angle_three_ogr()
{
    return angle_three;
}

void ControllerMsgRcvThread::analyseLight(int lightLevel, int idLight, bool isIRLight)
    {
        qDebug() << "Before";
        if (lightLevel != robotState->getLightLevel(idLight, isIRLight)) //сравниваем с тем, что хранится в RobotState
        {
            // отсылаем сообщение, чтобы поменять картинку

            emit changeLight(idLight, lightLevel, isIRLight);
            qDebug() << "After";
            /*context.startActivity(FuncUtils.getLightIntent(idLight,
                                                            lightLevel,
                                                            isIRLight,
                                                            ConstInfo.LIGHT_ROB_INTENT,
                                                            context,
                                                            MainActivity.class));*/
            robotState->setLightLevel(idLight, lightLevel, isIRLight);// устанавливаем новое значение
        }
    }

void ControllerMsgRcvThread::analyseArmJoint(int positionValue, int idArmJoint)
    {
        if (positionValue != robotState->getPositionArm(idArmJoint)) //сравниваем с тем, что хранится в RobotState
        {
            emit changePositionArm(idArmJoint, positionValue);

            robotState->setPositionArm(idArmJoint, positionValue);// устанавливаем новое значение
        }
    }

