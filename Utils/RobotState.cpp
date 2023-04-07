#include "RobotState.h"
#include "Utils/ConstInfo.h"
#include "FuncUtils.h"

RobotState::RobotState()
{
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

    batteryLevel = 0;
    signalLevel = 0;


    //
    positionArmMap[ConstInfo::ARMBASE]=0;
    positionArmMap[ConstInfo::ARM123]=0;
    positionArmMap[ConstInfo::ARM234]=0;
    positionArmMap[ConstInfo::ARM345]=0;
    positionArmMap[ConstInfo::TANGAGE]=0;
    positionArmMap[ConstInfo::HEEL]=0;

    reelState = new ReelState();
}




ReelState RobotState::getReelState(){
    return *reelState;
}


int RobotState::getLightLevel(int idLight, bool isIRLight)
{
    if (isIRLight)
    {
        return lightMapIR[idLight];
    }
    else
    {
        return lightMap[idLight];
    }
}

int RobotState::getPositionArm(int idArmJoint)
{

    return positionArmMap[idArmJoint];

}

int RobotState::getBatteryLevel()
{
    return batteryLevel;
}

int RobotState::getSignalLevel()
{
    return signalLevel;
}


void RobotState::setLenthOfReel(QByteArray arrayM, QByteArray arrayCm){
    reelState->setLengthM(FuncUtils::convertToDecimal(arrayM));
    reelState->setLengthM(FuncUtils::convertToDecimal(arrayCm));
}

void RobotState::setLightLevel(int idLight, int lightLevel, bool isIRLight)
{
    if (isIRLight)
    {
        lightMapIR[idLight] = lightLevel;
    }
    else
    {
        lightMap[idLight] = lightLevel;
    }
}


void RobotState::setPositionArm(int idArmJoint, int positionValue)
{
    positionArmMap[idArmJoint] = positionValue;

}

void RobotState::setBatteryLevel(int batteryLevel)
{
    this->batteryLevel = batteryLevel;
}

void RobotState::setSignalLevel(int signaleLevel)
{
    this->signalLevel = signaleLevel;
}
