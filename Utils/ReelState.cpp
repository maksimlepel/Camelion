#include "ReelState.h"

ReelState::ReelState(){}

ReelState::~ReelState(){}


void ReelState::setLengthM(qint8 lengthM){
    m_lengthM = lengthM;
}

void ReelState::setLengthCm(qint8 lengthCm){
    m_lengthCm = lengthCm;
}

qint8 ReelState::getLengthM(){
    return lengthM;
}

qint8 ReelState::getLengthCm(){
    return lengthCm;
}
