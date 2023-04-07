#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H

#include <QHash>
#include "ReelState.h"

/**
  ??????? ???????? ??????????, ?????????? ?? ??????
 */
class RobotState
{
public:
    RobotState();

    //Getting object of ReelState
    ReelState getReelState();
    int getLightLevel(int idLight, bool isIRLight);
    int getPositionArm(int idArmJoint);
    int getBatteryLevel();
    int getSignalLevel();

    //Setting length of Reel
    void setLenthOfReel(QByteArray arrayM, QByteArray arrayCm);

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

    ReelState* reelState;

};

#endif // ROBOTSTATE_H
