#ifndef CONTROLLERMSGRCVTHREAD_H
#define CONTROLLERMSGRCVTHREAD_H

#include "Thread/ThreadPrototype.h"
#include "Utils/RobotState.h"

#include <QByteArray>

/* считывает сообщения от робота */
class ControllerMsgRcvThread: public ThreadPrototype
{
    Q_OBJECT
public:
    explicit ControllerMsgRcvThread(int numPort, RobotState *robotState, QObject *parent);
    ~ ControllerMsgRcvThread();

    static int i_U_rob;
    static int i_done;
    int batt_U_rob();

    int done();
    int angle_zero_ogr();
    int angle_one_ogr();
    int angle_two_ogr();
    int angle_three_ogr();
    static int angle_zero;
    static int angle_one;
    static int angle_two;
    static int angle_three;

protected:
    virtual void mainFunction();

private:
    RobotState *robotState;
    QByteArray inPkg;
    int rcvByte; // количество полученных байтов
    static QByteArray qba_U_;

    void analysePackage();
    void analyseLight(int lightLevel, int idLight, bool isIRLight);
    void analyseArmJoint(int positionValue, int idArmJoint);

private slots:
    void processRead();

signals:
    void changeLight(int idLight, int lightLevel, bool isIRLight);
    void changePositionArm(int idArmJoint, int positionValue);
    void changeBatteryLevelRob(int batteryLevel);
    void changeSignalLevelRob(int signalLevel);
    void changeFlippers(int angle);
};

#endif // CONTROLLERMSGRCVTHREAD_H
