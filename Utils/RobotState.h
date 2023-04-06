#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H

#include <QHash>

/**
  ??????? ???????? ??????????, ?????????? ?? ??????
 */
class RobotState
{
public:
    RobotState();

    int getLightLevel(int idLight, bool isIRLight);
    int getPositionArm(int idArmJoint);
    int getBatteryLevel();
    int getSignalLevel();

    void setLightLevel(int idLight, int lightLevel, bool isIRLight);
    void setPositionArm(int idArmJoint, int positionValue);
    void setBatteryLevel(int batteryLevel);
    void setSignalLevel(int signaleLevel);

private:
    // ????
    QHash<int, int> lightMap;
    // ???? ??
    QHash<int, int> lightMapIR;
    //
    QHash<int, int> positionArmMap;

    int batteryLevel;
    int signalLevel;

};

#endif // ROBOTSTATE_H
