#ifndef CONTROLLERMSGSENDTHREAD_H
#define CONTROLLERMSGSENDTHREAD_H

#include <QTimer>
#include <QObject>

#include "Thread/ThreadPrototype.h"
#include "Utils/ControllerState.h"
#include "Thread/SendingTimer.h"
#include "Utils/RobotState.h"
#include "SerialPort/ButtonReelSerialPort.h"
/*
   Класс-поток, постоянно считывающий команды (состояние джойстика и что было нажато) с управляющего модуля, и передающий их роботу
 */

class ControllerMsgSendThread: public ThreadPrototype
{
    Q_OBJECT
public:
    explicit ControllerMsgSendThread(int numPort, RobotState *robotState, ControllerState *controllerState, QObject *parent);
    ~ControllerMsgSendThread();

    void recalcTimePeriod();
    void sendRightNow();
    static int i_U_rob;
    static int i_done;
    int batt_U_rob();

    int done();
    int angle_zero_ogr();
    int angle_one_ogr();
    int angle_two_ogr();
    int angle_three_ogr();
    int angle_flips_ogr();
    static int angle_zero;
    static int angle_one;
    static int angle_two;
    static int angle_three;
    static int angle_flips;

protected:
    virtual void mainFunction();

private:

    RobotState *robotState;

    ButtonReelSerialPort* buttonReelSerialPort;

    ControllerState *controllerState;
    QTimer *timer;

    int timePeriod; // задает период
    int rcvByte; // количество полученных байтов
    QByteArray inPkg;
    static QByteArray qba_U_;

    SendingTimer *sengingTimer;

    bool isTwoJoystick;

    void analysePackage();
    void analyseLight(int lightLevel, int idLight, bool isIRLight);
    //void analyseArmJoint(int positionValue, int idArmJoint);

public  slots:
    void sendInfo();
    void doIt(QString str);
    void setIsTwoJoystick(bool isTwoJoystick);
    void processRead();
    void analyseArmJoint(int positionValue, int idArmJoint);

    //send data from buttonReel serialport to buttonReel
    void sendDataToButtonReel(QByteArray data);

signals:
    void changeSendingInteval(int interval);
    void doItAgain(QString str);

    //signal for change label_8 when data of reelState is changing
    void show();

    void changeLight(int idLight, int lightLevel, bool isIRLight);
    void changePositionArm(int idArmJoint, int positionValue);
    void changeBatteryLevelRob(int batteryLevel);
    void changeSignalLevelRob(int signalLevel);
    void changeFlippers(int angle);

};

#endif // CONTROLLERMSGSENDTHREAD_H
